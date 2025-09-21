#include <sys/stat.h>
#include "../sound.h"

#include "utils/fs.h"
#include "views.h"


static const char *n64_rom_extensions[] = { "z64", "n64", "v64", "rom", NULL };
static const char *text_extensions[] = { "txt", NULL };
static const char *config_extensions[] = { "ini", "cfg", "yml", "yaml", "toml", NULL };
static const char *save_extensions[] = { "sav", "eep", "eeprom", "sra", "srm", "ram", "fla", "flashram", NULL };
static const char *patch_extensions[] = { "aps", "bps", "ips", "pps", "ups", "xdelta", NULL };
static const char *archive_extensions[] = { "zip", "rar", "7z", "tar", "gz", NULL };
static const char *image_extensions[] = { "png", "jpg", "gif", NULL };
static const char *music_extensions[] = { "mp3", "wav", "ogg", "wma", "flac", NULL };
static const char *controller_pak_extensions[] = { "mpk", "pak", NULL };
static const char *emulator_extensions[] = { "nes", "smc", "gb", "gbc", "sms", "gg", "chf", NULL };
static const char *cheat_extensions[] = {"cht", "cheats", "datel", "gameshark", NULL};
static const char *controller_pak_note_extensions[] = { "mpkn", NULL };

static struct stat st;
static bool is_memory_pak_dump;
static bool is_memory_pak_dump_note;


/**
 * Determine a human-readable file type string for a filename and update
 * controller-pak related state flags.
 *
 * Checks the filename's extension against several known categories (ROMs,
 * text, config, saves, patches, archives, images, music, controller pak,
 * controller pak notes, emulator ROMs, cheats) and returns a corresponding
 * short type string. Matching is performed in a fixed priority order; the
 * first matching category determines the returned string.
 *
 * Side effects:
 * - Resets and then sets the globals `is_memory_pak_dump` or
 *   `is_memory_pak_dump_note` when a controller pak file or controller pak
 *   note file is detected respectively.
 *
 * @param name Filename (or path) to inspect; used only for its extension.
 * @param is_directory If true, treats the entry as a directory and returns
 *        an empty string.
 * @return Pointer to a constant null-terminated string describing the type:
 *         - "" for directories
 *         - " Type: <...>\n" for known file types
 *         - " Type: Unknown file\n" if no category matches.
 *         The returned pointer refers to internal constant strings and MUST
 *         NOT be freed by the caller.
 */
static char *format_file_type (char *name, bool is_directory) {
    is_memory_pak_dump = false;
    is_memory_pak_dump_note = false;

    if (is_directory) {
        return "";
    } if (file_has_extensions(name, n64_rom_extensions)) {
        return " Type: N64 ROM\n";
    } else if (file_has_extensions(name, text_extensions)) {
        return " Type: Text file\n";
    } else if (file_has_extensions(name, config_extensions)) {
        return " Type: Config file\n";
    } else if (file_has_extensions(name, save_extensions)) {
        return " Type: N64 save\n";
    } else if (file_has_extensions(name, patch_extensions)) {
        return " Type: ROM patch\n";
    } else if (file_has_extensions(name, archive_extensions)) {
        return " Type: Archive\n";
    } else if (file_has_extensions(name, image_extensions)) {
        return " Type: Image file\n";
    } else if (file_has_extensions(name, music_extensions)) {
        return " Type: Music file\n";
    } else if (file_has_extensions(name, controller_pak_extensions)) {
        is_memory_pak_dump = true;
        return " Type: Controller Pak file\n";
    } else if (file_has_extensions(name, controller_pak_note_extensions)) {
        is_memory_pak_dump_note = true;
        return " Type: Note of Controller Pak file\n";
    } else if (file_has_extensions(name, emulator_extensions)) {
        return " Type: Emulator ROM file\n";
    } else if (file_has_extensions(name, cheat_extensions)) {
        return " Type: Cheats file\n";
    }
    return " Type: Unknown file\n";
}


/**
 * Process user actions for the file-info view, updating the menu mode and playing sounds.
 *
 * When Enter is pressed, transitions to a Controller Pak dump info mode if the selected
 * file is a memory pak dump or a memory pak note (controlled by the file-scoped flags
 * `is_memory_pak_dump` and `is_memory_pak_dump_note`). When Back is pressed, transitions
 * back to the browser mode. Side effects: plays enter/exit sound effects and sets
 * menu->next_mode.
 *
 * @param menu Current menu context (reads input actions and updates menu->next_mode).
 */
static void process (menu_t *menu) {
    if (is_memory_pak_dump && menu->actions.enter) {
        sound_play_effect(SFX_ENTER);
        menu->next_mode = MENU_MODE_CONTROLLER_PAK_DUMP_INFO;
    } else if (is_memory_pak_dump_note  && menu->actions.enter) {
        sound_play_effect(SFX_ENTER);
        menu->next_mode = MENU_MODE_CONTROLLER_PAK_DUMP_NOTE_INFO;
    } else if (menu->actions.back) {
        sound_play_effect(SFX_EXIT);
        menu->next_mode = MENU_MODE_BROWSER;
    }
}

/**
 * Render the "Entry Information" view onto the provided surface.
 *
 * Draws the entry name, size, attributes, inferred file type (via format_file_type),
 * last-modified timestamp, and a context-sensitive actions bar. The actions bar
 * changes if the entry is recognized as a Controller Pak dump or a Controller Pak
 * dump note (controlled by the external flags `is_memory_pak_dump` and
 * `is_memory_pak_dump_note`).
 *
 * @param menu Pointer to the current menu state used to obtain the selected entry.
 * @param d Surface to render the UI onto.
 */
static void draw (menu_t *menu, surface_t *d) {
    rdpq_attach(d, NULL);

    ui_components_background_draw();

    ui_components_layout_draw();

    ui_components_main_text_draw(
        STL_DEFAULT,
        ALIGN_CENTER, VALIGN_TOP,
        "ENTRY INFORMATION\n"
        "\n"
        "%s",
        menu->browser.entry->name
    );

    ui_components_main_text_draw(
        STL_DEFAULT,
        ALIGN_LEFT, VALIGN_TOP,
        "\n"
        "\n"
        "\n"
        "\n"
        " Size: %d bytes\n"
        " Attributes: %s %s\n"
        "%s"
        " Modified: %s",
        st.st_size,
        S_ISDIR(st.st_mode) ? "Directory" : "File",
        st.st_mode & S_IWUSR ? "" : "(Read only)",
        format_file_type(menu->browser.entry->name, S_ISDIR(st.st_mode)),
        ctime(&st.st_mtime)
    );

    if (is_memory_pak_dump) {
        ui_components_actions_bar_text_draw(STL_DEFAULT,
            ALIGN_LEFT, VALIGN_TOP,
            "A: Restore to Controller Pak\n"
            "B: Back"
        );
    } else if (is_memory_pak_dump_note) {
        ui_components_actions_bar_text_draw(STL_DEFAULT,
            ALIGN_LEFT, VALIGN_TOP,
            "A: Restore note to Controller Pak\n"
            "B: Back"
        );
    } else {
        ui_components_actions_bar_text_draw(STL_DEFAULT,
            ALIGN_LEFT, VALIGN_TOP,
            "\n"
            "B: Exit"
        );
    }

    rdpq_detach_show();
}


/**
 * Initialize file-info view state and load file metadata for the selected entry.
 *
 * Resets internal memory-pak flags, constructs the filesystem path for the
 * currently selected browser entry, and fills the global `st` struct via
 * stat(2). If stat fails, displays an error message on the provided menu.
 *
 * @param menu The menu context containing the browser directory and selected entry.
 */
void view_file_info_init (menu_t *menu) {
    is_memory_pak_dump = false;
    is_memory_pak_dump_note = false;
    path_t *path = path_clone_push(menu->browser.directory, menu->browser.entry->name);

    if (stat(path_get(path), &st)) {
        menu_show_error(menu, "Couldn't obtain file information");
    }

    path_free(path);
}

/**
 * Display the file information view and handle input.
 *
 * Processes pending user actions for this view and renders the file information
 * UI onto the given display surface.
 */
void view_file_info_display (menu_t *menu, surface_t *display) {
    process(menu);
    draw(menu, display);
}
