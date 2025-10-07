#include <miniz.h>
#include <miniz_zip.h>
#include <sys/utime.h>
#include "../sound.h"

#include "utils/fs.h"
#include "views.h"

static mz_zip_archive_file_stat st;

/**
 * Write a chunk of extracted data to a target file and update the extraction progress UI.
 *
 * @param pOpaque Pointer to the target FILE opened for writing; used as the write destination.
 * @param ofs     Offset (bytes) from the start of the uncompressed file used to compute progress.
 * @param pBuf    Buffer containing up to `n` bytes of data to write.
 * @param n       Number of bytes available in `pBuf` to write.
 * @returns       The number of bytes actually written to the target file.
 *
 * Notes:
 * - Progress is computed as `ofs / st.m_uncomp_size` where `st` is the global zip entry stat.
 * - When a display is available, the function updates the UI loader to reflect current progress.
 */
static size_t mz_zip_file_write_callback(void *pOpaque, mz_uint64 ofs, const void *pBuf, size_t n) {
    float progress = ofs / (float)st.m_uncomp_size;
    surface_t *d = (progress >= 1.0f) ? display_get() : display_try_get();
    n = fwrite(pBuf, 1, n, (FILE *)pOpaque);

    if (d) {
        rdpq_attach(d, NULL);

        ui_components_background_draw();

        ui_components_loader_draw(progress, "Extracting file...");  

        rdpq_detach_show();
    }

    return n;
}

/**
 * Extract the currently viewed archive entry to disk and update the browser state.
 *
 * Attempts to write the file described by the global `st` (using its filename and index)
 * into the current browser directory. On success, sets the browser's selected file to the
 * extracted path, preserves the file modification time, and switches the UI to the browser view.
 * On failure, displays an appropriate error message and updates the browser selection when
 * the target file already exists.
 *
 * @param menu Menu state used for directory resolution, error reporting, browser selection,
 *             and mode switching; this function mutates fields on `menu->browser` and `menu->next_mode`.
 */
static void extract(menu_t *menu) {
    path_t *path = path_clone(menu->browser.directory);
    path_pop(path);
    path_push(path, st.m_filename);

    path_t *dir = path_clone(path);
    path_pop(dir);

    if (file_exists(path_get(path))) {
        menu->browser.select_file = path_clone(path);
        menu_show_error(menu, "File already exists");
    } else if (directory_create(path_get(dir))) {
        menu_show_error(menu, "Failed to create directory");
    } else {
        FILE *file = fopen(path_get(path), "wb");
        if (file) {
            struct utimbuf mtime = { st.m_time, st.m_time };
            if (!mz_zip_reader_extract_to_callback(
                &menu->browser.zip,
                st.m_file_index,
                mz_zip_file_write_callback,
                file, 0)) {
                menu_show_error(menu, "Failed to extract file");
            }
            fclose(file);
            utime(path_get(path), &mtime);
            menu->browser.select_file = path_clone(path);
            menu->next_mode = MENU_MODE_BROWSER;
        } else {
            menu_show_error(menu, "Failed to create file");
        }
    }

    path_free(path);
    path_free(dir);
}

/**
 * Handle user input to start extracting the selected file or to navigate back.
 *
 * If the Enter action is pressed and the current archive entry is a supported file
 * (not a directory), plays the enter sound and sets the menu's load_pending.extract_file
 * flag to request extraction. If the Back action is pressed, plays the exit sound
 * and switches the menu to the browser view.
 *
 * @param menu Pointer to the menu state.
 */
static void process (menu_t *menu) {
    if (menu->actions.enter && !st.m_is_directory && st.m_is_supported) {
        sound_play_effect(SFX_ENTER);
        menu->load_pending.extract_file = true;
    } else if (menu->actions.back) {
        sound_play_effect(SFX_EXIT);
        menu->next_mode = MENU_MODE_BROWSER;
    }
}

/**
 * Render the extract-file view onto the provided surface.
 *
 * Draws the view background and either a progress loader when an extraction is pending
 * or the full file details layout including filename, file metadata (directory flag,
 * encryption flag, timestamps, sizes, CRC), an optional comment block, and the actions bar.
 * The function attaches to the given display surface before rendering and detaches/show
 * the updated surface when done.
 *
 * @param menu Current menu state and view context.
 * @param d Surface to draw onto.
 */
static void draw (menu_t *menu, surface_t *d) {
    rdpq_attach(d, NULL);

    ui_components_background_draw();

    if (menu->load_pending.extract_file) {
        ui_components_loader_draw(0.0f, NULL);
    } else {
        ui_components_layout_draw();

        file_info_t info = {
            st.m_is_directory,
            true,
            st.m_is_encrypted,
            st.m_time,
            st.m_uncomp_size,
            st.m_comp_size,
            st.m_crc32,
            false,
            false
        };
        ui_components_file_info_draw(st.m_filename, &info);

        if (st.m_comment_size > 0) {
            ui_components_main_text_draw(
                STL_DEFAULT,
                ALIGN_LEFT, VALIGN_BOTTOM,
                " Comment:\n"
                "\n"
                "%s",
                st.m_comment
            );
        }

        ui_components_actions_bar_text_draw(
            STL_DEFAULT,
            ALIGN_LEFT, VALIGN_TOP,
            st.m_is_supported ? "A: Extract\nB: Exit" : "\nB: Exit"
        );
    }

    rdpq_detach_show();
}


/**
 * Populate global file metadata for the currently selected archive entry.
 *
 * Attempts to retrieve the ZIP entry's file stat into the module-global `st`.
 * If retrieval fails, an error message is shown via the provided menu.
 *
 * @param menu Pointer to the current menu state containing the archive and selected entry.
 */
void view_extract_file_init (menu_t *menu) {
    if (!mz_zip_reader_file_stat(&menu->browser.zip, menu->browser.entry->index, &st)) {
        menu_show_error(menu, "Couldn't obtain archived file information");
    }
}

/**
 * Main display loop for the extract-file view: handles input, renders the view,
 * and performs a pending file extraction.
 *
 * If menu->load_pending.extract_file is set, the flag is cleared and extraction
 * is initiated before returning.
 *
 * @param menu Current menu state and view context.
 * @param display Surface to draw the view onto.
 */
void view_extract_file_display (menu_t *menu, surface_t *display) {
    process(menu);

    draw(menu, display);

    if (menu->load_pending.extract_file) {
        menu->load_pending.extract_file = false;
        extract(menu);
    }
}