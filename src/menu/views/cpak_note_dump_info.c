#include <stdbool.h>
#include <stdio.h>
#include <libdragon.h>
#include "views.h"
#include "../sound.h"
#include "../fonts.h"
#include <fatfs/ff.h>

static uint16_t __get_header_checksum( uint16_t *block );
static int __validate_header( uint8_t *sector );
static uint8_t __get_toc_checksum( uint8_t *sector );
static int __validate_toc( uint8_t *sector );
static int __n64_to_utf8( uint8_t c, char *out );
static int __utf8_to_n64(const char *in, uint8_t *out);
static int __validate_region( uint8_t region );
static int __read_note( uint8_t *tnote, entry_structure_t *note );
static int __write_note( entry_structure_t *note, uint8_t *out_note );
static int __get_num_pages( uint8_t *sector, int inode );
static int __get_free_space (uint8_t *sector);
static int __get_note_block (uint8_t *sector, int inode, int block);
static int __get_valid_toc (int controller);
int get_mempak_entry_mod( int controller, int entry, entry_structure_t *entry_data );
int write_mempak_data_based_on_entry (int controller, entry_structure_t *entry, uint8_t *data);
int write_mempak_address_mod (int controller, uint16_t address, uint8_t *data);
int write_mempak_sector_mod (int controller, int sector, uint8_t *sector_data);
int read_mempak_sector_mod( int controller, int sector, uint8_t *sector_data );
int read_mempak_address_mod( int controller, uint16_t address, uint8_t *data );
static uint16_t __calc_address_crc( uint16_t address );
static uint8_t __calc_data_crc (uint8_t *data);

char * CPAK_ENTRIES_PATH = "sd:/aio/cpak_saves/entries";
char * CPAK_ENTRIES_PATH_NO_PRE = "/aio/cpak_saves/entries";
char cpak_note_path[255];
short controller_selected_for_restore_note;
char failure_message_note[255];
bool start_note_restore;

bool has_cpak_note(int controller) {
    
    joypad_accessory_type_t val =  joypad_get_accessory_type(controller);

    return val == JOYPAD_ACCESSORY_TYPE_CONTROLLER_PAK;
}

char* find_corresponding_file(const char* selected_file) {
    static char corresponding_file[256];
    strcpy(corresponding_file, selected_file);
    
    char *insert_pos = strstr(corresponding_file, "CPAK");
    if (insert_pos != NULL) {
        if (insert_pos[4] == 'E') {
            // Remove 'E'
            memmove(&insert_pos[4], &insert_pos[5], strlen(insert_pos) - 4);
        } else {
            // Add 'E'
            memmove(&insert_pos[5], &insert_pos[4], strlen(insert_pos) - 3);
            insert_pos[4] = 'E';
        }
    }
    return corresponding_file;
}

bool restore_controller_pak_note(int controller) {
    sprintf(failure_message_note, " ");

    if (!has_cpak_note(controller)) {
        //"No controller pak detected!"
        sprintf(failure_message_note, "No controller pak detected on controller %d!", controller + 1);
        return false;
    }

    uint8_t* data = malloc(MEMPAK_BLOCK_SIZE * 128 * sizeof(uint8_t));

    char filename[255];
    char filename_e[255];

    sprintf(filename, "%s", cpak_note_path);
    sprintf(filename_e, "%s", find_corresponding_file(cpak_note_path));

    debugf("Filename: %s\n", filename);
    debugf("Filename_e: %s\n", filename_e);

    /*****************************************/

    //loop to each entry to find an "invalid" one
    for (int i = 0; i < 16; i++) {

        entry_structure_t entry;
        int val = get_mempak_entry_mod(controller, i, &entry);

        if (!entry.valid) {

            int free_space = get_mempak_free_space(controller);

            if (entry.blocks > free_space){
                sprintf(failure_message_note, "Not enough space on controller %d!\n(Required: %d / Available: %d)", controller + 1, entry.blocks, free_space);
                return false;
            } else {
                val = get_mempak_entry_mod(controller, i, &entry);
            }

            delete_mempak_entry(controller, &entry);
            FILE *fpe = fopen(filename_e, "r");

            if (!fpe) {
                debugf("ERROR: Unable to open: %s\n", filename_e);
                return false;
            }

            char buffer[128];

            if (fgets(buffer, sizeof(buffer), fpe) != NULL) entry.blocks = (uint8_t)strtoul(buffer, NULL, 10);
            if (fgets(buffer, sizeof(buffer), fpe) != NULL) entry.game_id = (uint16_t)strtoul(buffer, NULL, 10);
            if (fgets(buffer, sizeof(buffer), fpe) != NULL) entry.inode = (uint16_t)strtoul(buffer, NULL, 10);
            if (fgets(buffer, sizeof(buffer), fpe) != NULL) {
                // Ensure null termination for name
                strncpy(entry.name, buffer, sizeof(entry.name) - 1);
                entry.name[sizeof(entry.name) - 1] = '\0';
                // Remove newline character if present
                char *newline = strchr(entry.name, '\n');
                if (newline) {
                    *newline = '\0';
                }
            }
            if (fgets(buffer, sizeof(buffer), fpe) != NULL) entry.region = (uint8_t)strtoul(buffer, NULL, 10);
            if (fgets(buffer, sizeof(buffer), fpe) != NULL) entry.valid = (uint8_t)strtoul(buffer, NULL, 10);
            if (fgets(buffer, sizeof(buffer), fpe) != NULL) entry.vendor = (uint32_t)strtoul(buffer, NULL, 10);

            fclose(fpe);

            debugf("Entry before write: %s\n", entry.name);
            debugf("Entry blocks: %d\n", entry.blocks);
            debugf("Entry inode: %d\n", entry.inode);
            debugf("Entry region: %d\n", entry.region);
            debugf("Entry game_id: %d\n", entry.game_id);
            debugf("Entry vendor: %ld\n", entry.vendor);
            debugf("Entry entry_id: %d\n", entry.entry_id);
            debugf("Entry valid: %d\n", entry.valid);
            
            FILE *fp = fopen(filename, "rb");

            if (!fp) {
                debugf("ERROR: Unable to open: %s\n", filename);
                return false;
            }

            uint8_t* data = calloc(entry.blocks * MEMPAK_BLOCK_SIZE, sizeof(uint8_t));
            size_t read_size = fread(data, 1, entry.blocks * MEMPAK_BLOCK_SIZE, fp);
            fclose(fp);

            val = write_mempak_data_based_on_entry(controller, &entry, data);
            debugf("Result of writing: %d\n", val);

            free(data);
            debugf("Entry after write: %s\n", entry.name);
            debugf("Entry blocks: %d\n", entry.blocks);
            debugf("Entry inode: %d\n", entry.inode);
            debugf("Entry region: %d\n", entry.region);
            debugf("Entry game_id: %d\n", entry.game_id);
            debugf("Entry vendor: %ld\n", entry.vendor);
            debugf("Entry entry_id: %d\n", entry.entry_id);
            debugf("Entry valid: %d\n", entry.valid);
            break;
        } 
    }

    /*****************************************/

    free(data);
    return true;
}

static void process (menu_t *menu) {
    if (menu->actions.go_left) {
        sound_play_effect(SFX_CURSOR);
        controller_selected_for_restore_note = ((controller_selected_for_restore_note - 1) + 4) % 4;
    } else if (menu->actions.go_right) {
        sound_play_effect(SFX_CURSOR);
        controller_selected_for_restore_note = ((controller_selected_for_restore_note + 1) + 4) % 4;
    } else if (menu->actions.back) {
        sound_play_effect(SFX_EXIT);
        menu->next_mode = MENU_MODE_BROWSER;
    } else if (menu->actions.enter) {
        sound_play_effect(SFX_ENTER);
        start_note_restore = true;
    }
}

static void draw (menu_t *menu, surface_t *d) {

    rdpq_attach(d, NULL);

    ui_components_background_draw();

    ui_components_layout_draw();

    ui_components_main_text_draw(STL_DEFAULT,
        ALIGN_CENTER, VALIGN_TOP,
        "Controller Pak dump:\n"
    );
    
    ui_components_main_text_draw(STL_GREEN,
        ALIGN_CENTER, VALIGN_TOP,
        "\n"
        "%s\n"
        "\n"
        "\n"
        "\n",
        cpak_note_path
    );

    ui_components_main_text_draw(STL_ORANGE,
        ALIGN_CENTER, VALIGN_TOP,
        "\n"
        "\n"
        "\n"
        "\n"
        "%s\n",
        failure_message_note
    );

    
    ui_components_messagebox_draw(
        "Do you want to restore this note dump to the controller Pak?\n\n"
        "Controller selected: %d\n\n"
        "A: Yes  B: No \n"
        "<- / ->: Change controller",
        controller_selected_for_restore_note + 1
    );

    if (start_note_restore) {
        rdpq_detach_show();
        if (restore_controller_pak_note(controller_selected_for_restore_note)) {
            menu->next_mode = MENU_MODE_BROWSER;
        } 
        start_note_restore = false;
        return;
        
    }

    rdpq_detach_show();
}

void view_controller_pak_note_dump_info_init (menu_t *menu) {

    path_t *path = path_clone_push(menu->browser.directory, menu->browser.entry->name);

    sprintf(cpak_note_path, "%s", path_get(path));
    start_note_restore = false;
    sprintf(failure_message_note, " ");

}

void view_controller_pak_note_dump_info_display (menu_t *menu, surface_t *display) {
    process(menu);
    draw(menu, display);
}







































/** @brief This block is empty */
#define BLOCK_EMPTY         0x03
/** @brief This is the last block in the note */
#define BLOCK_LAST          0x01
/** @brief First valid block that can contain user data */
#define BLOCK_VALID_FIRST   0x05
/** @brief Last valid block that can contain user data */
#define BLOCK_VALID_LAST    0x7F
/** @} */

/**
 * @brief Calculate the checksum of a header
 *
 * @param[in] block
 *            A block at the start of a header
 *
 * @return The 16 bit checksum over the header
 */
static uint16_t __get_header_checksum( uint16_t *block )
{
  uint32_t sum = 0;

  for ( int i = 0; i < 14; i++ )
  {
    sum += *(block++);
  }

  return sum & 0xFFFF;
}

/**
 * @brief Check a mempak header for validity
 *
 * @param[in] sector
 *            A sector containing a mempak header
 *
 * @retval 0 if the header is valid
 * @retval -1 if the header is invalid
 */
static int __validate_header( uint8_t *sector )
{
    uint16_t checksum = 0;
    uint8_t current_block = 0x0;

    if( !sector ) { return -1; }

    /* Header is first sector */
    if( memcmp( &sector[0x20], &sector[0x60], 16 ) != 0 ) { return -1; }
    if( memcmp( &sector[0x80], &sector[0xC0], 16 ) != 0 ) { return -1; }
    if( memcmp( &sector[0x20], &sector[0x80], 16 ) != 0 ) { return -1; }

    /* Check 4 checksums of copied header data */
    current_block = 0x20;
    checksum = __get_header_checksum((uint16_t *)&sector[current_block]);
    if( (checksum != *(uint16_t *)(&sector[current_block + 0x1C])) || (checksum != 0xFFF2 - *(uint16_t *)(&sector[current_block + 0x1E])) ) { return -1; }
    current_block = 0x60;
    checksum = __get_header_checksum((uint16_t *)&sector[current_block]);
    if( (checksum != *(uint16_t *)(&sector[current_block + 0x1C])) || (checksum != 0xFFF2 - *(uint16_t *)(&sector[current_block + 0x1E])) ) { return -1; }
    current_block = 0x80;
    checksum = __get_header_checksum((uint16_t *)&sector[current_block]);
    if( (checksum != *(uint16_t *)(&sector[current_block + 0x1C])) || (checksum != 0xFFF2 - *(uint16_t *)(&sector[current_block + 0x1E])) ) { return -1; }
    current_block = 0xC0;
    checksum = __get_header_checksum((uint16_t *)&sector[current_block]);
    if( (checksum != *(uint16_t *)(&sector[current_block + 0x1C])) || (checksum != 0xFFF2 - *(uint16_t *)(&sector[current_block + 0x1E])) ) { return -1; }

    return 0;
}

/**
 * @brief Calculate the checksum over a TOC sector
 *
 * @param[in] sector
 *            A sector containing a TOC
 *
 * @return The 8 bit checksum over the TOC
 */
static uint8_t __get_toc_checksum( uint8_t *sector )
{
    uint32_t sum = 0;

    /* Rudimentary checksum */
    for( int i = 5; i < 128; i++ )
    {
        sum += sector[(i << 1) + 1];
    }

    return sum & 0xFF;
}

/**
 * @brief Check a mempak TOC sector for validity
 *
 * @param[in] sector
 *            A sector containing a TOC
 *
 * @retval 0 if the TOC is valid
 * @retval -1 if the TOC is invalid
 */
static int __validate_toc( uint8_t *sector )
{
    /* True checksum is sum % 256 */
    if( __get_toc_checksum( sector ) == sector[1] )
    {
        return 0;
    }
    else
    {
        return -1;
    }

    return -1;
}

/**
 * @brief Convert a Controller Pak character to UTF-8
 *
 * The codepage used by the controller pak contains a subset of ASCII and
 * some Katakana.
 *
 * @param[in] c
 *            A character read from a Controller Pak entry title
 * @param[out] out
 *            Output buffer to write the bytes to (at least 3 bytes).
 *
 * @return    The number of bytes written to the output buffer
 */
static int __n64_to_utf8( uint8_t c, char *out )
{
    /* Miscelaneous chart */
    switch( c )
    {
        case 0x00:
            *out++ = 0; return 1;
        case 0x0F:
            *out++ = ' '; return 1;
        case 0x34:
            *out++ = '!'; return 1;
        case 0x35:
            *out++ = '\"'; return 1;
        case 0x36:
            *out++ = '#'; return 1;
        case 0x37:
            *out++ = '`'; return 1;
        case 0x38:
            *out++ = '*'; return 1;
        case 0x39:
            *out++ = '+'; return 1;
        case 0x3A:
            *out++ = ','; return 1;
        case 0x3B:
            *out++ = '-'; return 1;
        case 0x3C:
            *out++ = '.'; return 1;
        case 0x3D:
            *out++ = '/'; return 1;
        case 0x3E:
            *out++ = ':'; return 1;
        case 0x3F:
            *out++ = '='; return 1;
        case 0x40:
            *out++ = '?'; return 1;
        case 0x41:
            *out++ = '@'; return 1;
    }

    /* Numbers */
    if( c >= 0x10 && c <= 0x19 )
    {
        *out++ = '0' + (c - 0x10);
        return 1;
    }

    /* Uppercase ASCII */
    if( c >= 0x1A && c <= 0x33 )
    {
        *out++ = 'A' + (c - 0x1A);
        return 1;
    }

    /* Katakana and CJK symbols */
    if( c >= 0x42 && c <= 0x94 )
    {
        const int cjk_base = 0x3000;
        static uint8_t cjk_map[83] = { 2, 155, 156, 161, 163, 165, 167, 169, 195, 227, 229, 231, 242, 243, 162, 164, 166, 168, 170, 171, 173, 175, 177, 179, 181, 183, 185, 187, 189, 191, 193, 196, 198, 200, 202, 203, 204, 205, 206, 207, 210, 213, 216, 219, 222, 223, 224, 225, 226, 228, 230, 232, 233, 234, 235, 236, 237, 239, 172, 174, 176, 178, 180, 182, 184, 186, 188, 190, 192, 194, 197, 199, 201, 208, 211, 214, 217, 220, 209, 212, 215, 218, 221 };
        uint16_t codepoint = cjk_base + cjk_map[c - 0x42];
        *out++ = 0xE0 | ((codepoint >> 12) & 0x0F);
        *out++ = 0x80 | ((codepoint >> 6) & 0x3F);
        *out++ = 0x80 | (codepoint & 0x3F);
        return 3;
    }

    /* Default to space for unprintables */
    *out++ = ' ';
    return 1;
}

#include <stdint.h>

/**
 * @brief Convert a UTF-8 sequence (ASCII subset + Katakana/CJK) back to a Controller Pak code
 *
 * @param[in]  in      Pointer to UTF-8 bytes
 * @param[out] out     Pointer to uint8_t where the code will be stored
 *
 * @return Number of bytes consumed from `in` (1 or 3)
 */
static int __utf8_to_n64(const char *in, uint8_t *out)
{
    uint32_t codepoint = 0;
    int consumed = 0;

    unsigned char b0 = (unsigned char)in[0];
    if (b0 < 0x80) {
        // ASCII (including control null)
        codepoint = b0;
        consumed = 1;
    }
    else {
        // expect exactly three-byte sequence for our CJK/Katakana block
        unsigned char b1 = (unsigned char)in[1];
        unsigned char b2 = (unsigned char)in[2];
        if ((b0 & 0xF0) == 0xE0 &&
            (b1 & 0xC0) == 0x80 &&
            (b2 & 0xC0) == 0x80)
        {
            codepoint  = ((b0 & 0x0F) << 12);
            codepoint |= ((b1 & 0x3F) << 6);
            codepoint |=  (b2 & 0x3F);
            consumed = 3;
        } else {
            // invalid UTF-8 for our purposes → treat as space
            *out = 0x0F;
            return 1;
        }
    }

    // 1) Null
    if (codepoint == 0x0000) {
        *out = 0x00;
        return consumed;
    }
    // 2) Space
    if (codepoint == ' ') {
        *out = 0x0F;
        return consumed;
    }

    // 3) Miscellaneous single-byte punctuation
    switch (codepoint) {
        case '!':  *out = 0x34; return consumed;
        case '\"': *out = 0x35; return consumed;
        case '#':  *out = 0x36; return consumed;
        case '`':  *out = 0x37; return consumed;
        case '*':  *out = 0x38; return consumed;
        case '+':  *out = 0x39; return consumed;
        case ',':  *out = 0x3A; return consumed;
        case '-':  *out = 0x3B; return consumed;
        case '.':  *out = 0x3C; return consumed;
        case '/':  *out = 0x3D; return consumed;
        case ':':  *out = 0x3E; return consumed;
        case '=':  *out = 0x3F; return consumed;
        case '?':  *out = 0x40; return consumed;
        case '@':  *out = 0x41; return consumed;
    }

    // 4) Digits '0'–'9' → 0x10–0x19
    if (codepoint >= '0' && codepoint <= '9') {
        *out = 0x10 + (codepoint - '0');
        return consumed;
    }

    // 5) Uppercase ASCII 'A'–'Z' → 0x1A–0x33
    if (codepoint >= 'A' && codepoint <= 'Z') {
        *out = 0x1A + (codepoint - 'A');
        return consumed;
    }

    // 6) Katakana/CJK block
    //    mirror the same mapping array you used for UTF-8 output:
    {
        static const uint8_t cjk_map[83] = {
            2,155,156,161,163,165,167,169,195,227,229,231,242,243,162,164,166,168,170,171,173,175,177,179,181,183,185,187,189,191,193,
            196,198,200,202,203,204,205,206,207,210,213,216,219,222,223,224,225,226,228,230,232,233,234,235,236,237,239,172,174,176,178,180,
            182,184,186,188,190,192,194,197,199,201,208,211,214,217,220,209,212,215,218,221
        };
        const uint16_t base = 0x3000;
        for (int i = 0; i < 83; i++) {
            if (codepoint == (base + cjk_map[i])) {
                *out = (uint8_t)(0x42 + i);
                return consumed;
            }
        }
    }

    // 7) Fallback → space
    *out = 0x0F;
    return consumed;
}


/**
 * @brief Check a region read from a mempak entry for validity
 *
 * @param[in] region
 *            A region read from a mempak entry
 *
 * @retval 0 if the region is valid
 * @retval -1 if the region is invalid
 */
static int __validate_region( uint8_t region )
{
    switch( region )
    {
        case 0x00:
        case 0x37:
        case 0x41:
        case 0x44:
        case 0x45:
        case 0x46:
        case 0x49:
        case 0x4A:
        case 0x50:
        case 0x53:
        case 0x55:
        case 0x58:
        case 0x59:
            /* Acceptible region */
            return 0;
    }

    /* Invalid region */
    return -3;
}

/**
 * @brief Parse a note structure from a TOC
 *
 * Given a note block read from a mempak TOC, parse and return a structure
 * representing the data.
 *
 * @param[in]  tnote
 *             32 bytes read from a mempak TOC
 * @param[out] note
 *             Parsed note structure
 *
 * @retval 0 note was parsed properly
 * @retval -1 parameters were invalid
 * @retval -2 note inode out of bounds
 * @retval -3 note region invalid
 */
static int __read_note( uint8_t *tnote, entry_structure_t *note )
{
    if( !tnote || !note ) { return -1; }

    /* Easy stuff */
    note->vendor = (tnote[0] << 16) | (tnote[1] << 8) | (tnote[2]);
    note->region = tnote[3];

    /* Important stuff */
    note->game_id = (tnote[4] << 8) | (tnote[5]);
    note->inode = (tnote[6] << 8) | (tnote[7]);

    /* Don't know number of blocks */
    note->blocks = 0;
    note->valid = 0;
    note->entry_id = 255;

    /* Translate n64 to ascii */
    memset( note->name, 0, sizeof( note->name ) );

    int nidx = 0;
    for( int i = 0; i < 16; i++ )
    {
        if ( tnote[0x10 + i] == 0 ) break;
        nidx += __n64_to_utf8( tnote[0x10 + i], &note->name[nidx] );
    }

    /* Separator between name and extension */
    note->name[nidx++] = '.';

    for( int i = 0; i < 4; i++ )
    {
        if ( tnote[0xC + i] == 0 ) break;
        nidx += __n64_to_utf8( tnote[0xC + i], &note->name[nidx] );
    }

    /* String terminator */
    note->name[nidx++] = 0;

    /* Validate entries */
    if( note->inode < BLOCK_VALID_FIRST || note->inode > BLOCK_VALID_LAST )
    {
        /* Invalid inode */
        return -2;
    }

    if( __validate_region( note->region ) )
    {
        /* Invalid region */
        return -3;
    }

    /* Checks out */
    note->valid = 1;
    return 0;
}

/**
 * @brief Create a note structure for a mempak TOC
 *
 * Given a valid note structure, format it for writing to a mempak TOC
 *
 * @param[in]  note
 *             Valid note structure to convert
 * @param[out] out_note
 *             32 bytes ready to be written to a mempak TOC
 *
 * @retval 0 if the note was converted properly
 * @retval -1 if the parameters were invalid
 */
static int __write_note(entry_structure_t *note, uint8_t *out_note)
{
    if (!note || !out_note) return -1;

    /* 1) zero the 32-byte pak entry */
    memset(out_note, 0, 32);

    /* 2) easy header bits */
    out_note[0] = (note->vendor >> 16) & 0xFF;
    out_note[1] = (note->vendor >>  8) & 0xFF;
    out_note[2] =  note->vendor        & 0xFF;
    out_note[3] = note->region;
    out_note[4] = (note->game_id >> 8) & 0xFF;
    out_note[5] =  note->game_id       & 0xFF;
    out_note[6] = (note->inode   >> 8) & 0xFF;
    out_note[7] =  note->inode         & 0xFF;

    /* 3) official-save constants */
    out_note[8] = 0x02;
    out_note[9] = 0x03;

    /* 4) work on a local, NUL-terminated copy of the name */
    char tname[sizeof(note->name) + 1];
    memcpy(tname, note->name, sizeof(note->name));
    tname[sizeof(note->name)] = '\0';  /* ensure termination */

    /* 5) find a ‘.’ for extension, if any */
    char *dot = NULL;
    for (char *p = tname + strlen(tname); p > tname; --p) {
        if (*p == '.') { dot = p; break; }
    }
    if (dot) {
        /* convert the first codepoint after the dot */
        uint8_t extcode;
        int consumed = __utf8_to_n64(dot + 1, &extcode);
        out_note[0x0C] = extcode;
        /* zap the dot + its bytes so they won’t show up in the name */
        dot[0] = '\0';
        for (int i = 1; i <= consumed; i++) {
            if (dot[i] == '\0') break;
            dot[i] = '\0';
        }
    }

    /* 6) walk the remaining UTF-8 name, emit up to 16 N64 codes */
    const char *in = tname;
    int written = 0;
    while (written < 16 && *in) {
        uint8_t code;
        int len = __utf8_to_n64(in, &code);
        out_note[0x10 + written] = code;
        in += len;
        written++;
    }
    /* 7) pad with spaces (0x0F) */
    for (; written < 16; written++) {
        out_note[0x10 + written] = 0x0F;
    }

    return 0;
}

/**
 * @brief Return number of pages a note occupies
 *
 * Given a starting inode and a TOC sector, walk the linked list for a note
 * and return the number of pages/blocks/sectors a note occupies.
 *
 * @param[in] sector
 *            A TOC sector
 * @param[in] inode
 *            A starting inode
 *
 * @retval -2 The file contained free blocks
 * @retval -3 The filesystem was invalid
 * @return The number of blocks in a note
 */
static int __get_num_pages( uint8_t *sector, int inode )
{
    if( inode < BLOCK_VALID_FIRST || inode > BLOCK_VALID_LAST ) { return -1; }

    int tally = 0;
    int last = inode;
    int rcount = 0;

    /* If we go over this, something is wrong */
    while( rcount < 123 )
    {
        switch( sector[(last << 1) + 1] )
        {
            case BLOCK_LAST:
                /* Last block */
                return tally + 1;
            case BLOCK_EMPTY:
                /* Error, can't have free blocks! */
                return -2;
            default:
                last = sector[(last << 1) + 1];
                tally++;

                /* Failed to point to valid next block */
                if( last < BLOCK_VALID_FIRST || last > BLOCK_VALID_LAST ) { return -3; }
                break;
        }

        rcount++;
    }

    /* Invalid filesystem */
    return -3;
}

/**
 * @brief Get number of free blocks on a mempak
 *
 * @param[in] sector
 *            A valid TOC block to examine
 *
 * @return The number of free blocks
 */
static int __get_free_space( uint8_t *sector )
{
    int space = 0;

    for( int i = BLOCK_VALID_FIRST; i <= BLOCK_VALID_LAST; i++ )
    {
        if( sector[(i << 1) + 1] == BLOCK_EMPTY )
        {
            space++;
        }
    }

    return space;
}

/**
 * @brief Get the inode of the n'th block in a note
 *
 * @param[in] sector
 *            A valid TOC sector
 * @param[in] inode
 *            The starting inode of the note
 * @param[in] block
 *            The block offset (starting from 0) to retrieve
 *
 * @retval -2 if there were free blocks in the file
 * @retval -3 if the filesystem was invalid
 * @return The inode of the n'th block
 */
static int __get_note_block( uint8_t *sector, int inode, int block )
{
    if( inode < BLOCK_VALID_FIRST || inode > BLOCK_VALID_LAST ) { return -1; }
    if( block < 0 || block > 123 ) { return -1; }

    int tally = block + 1;
    int last = inode;

    /* Only going through until we hit the requested node */
    while( tally > 0 )
    {
        /* Count down to zero, when zero is hit, this is the node we want */
        tally--;

        if( !tally )
        {
            return last;
        }
        else
        {
            switch( sector[(last << 1) + 1] )
            {
                case BLOCK_LAST:
                    /* Last block, couldn't find block number */
                    return -2;
                case BLOCK_EMPTY:
                    /* Error, can't have free blocks! */
                    return -2;
                default:
                    last = sector[(last << 1) + 1];

                    /* Failed to point to valid next block */
                    if( last < 5 || last >= 128 ) { return -3; }
                    break;
            }
        }
    }

    /* Invalid filesystem */
    return -3;
}

/**
 * @brief Retrieve the sector number of the first valid TOC found
 *
 * @param[in] controller
 *            The controller (0-3) to inspect for a valid TOC
 *
 * @retval -2 the mempak was not inserted or was bad
 * @retval -3 the mempak was unformatted or the header was invalid
 * @retval 1 the first sector has a valid TOC
 * @retval 2 the second sector has a valid TOC
 */
static int __get_valid_toc( int controller )
{
    /* We will need only one sector at a time */
    uint8_t data[MEMPAK_BLOCK_SIZE];

    /* First check to see that the header block is valid */
    if( read_mempak_sector_mod( controller, 0, data ) )
    {
        /* Couldn't read header */
        return -2;
    }

    if( __validate_header( data ) )
    {
        /* Header is invalid or unformatted */
        return -3;
    }

    /* Try to read the first TOC */
    if( read_mempak_sector_mod( controller, 1, data ) )
    {
        /* Couldn't read header */
        return -2;
    }

    if( __validate_toc( data ) )
    {
        /* First TOC is bad.  Maybe the second works? */
        if( read_mempak_sector_mod( controller, 2, data ) )
        {
            /* Couldn't read header */
            return -2;
        }

        if( __validate_toc( data ) )
        {
            /* Second TOC is bad, nothing good on this memcard */
            return -3;
        }
        else
        {
            /* Found a good TOC! */
            return 2;
        }
    }
    else
    {
        /* Found a good TOC! */
        return 1;
    }
}


/*
This is a modified version of the function provided by the libdragon library.
It has the only difference that it doesn't override some crucial datas on the entry took from the dumps.
*/

int write_mempak_data_based_on_entry( int controller, entry_structure_t *entry, uint8_t *data )
{
    uint8_t sector[MEMPAK_BLOCK_SIZE];
    uint8_t tmp_data[32];
    int toc;

    /* Sanity checking on input data */
    if( !entry || !data ) { return -1; }
    if( entry->blocks < 1 ) { return -1; }
    if( __validate_region( entry->region ) ) { return -1; }
    if( strlen( entry->name ) == 0 ) { return -1; }

    /* Grab valid TOC */
    if( (toc = __get_valid_toc( controller )) <= 0 )
    {
        /* Bad mempak or was removed, return */
        return -2;
    }

    /* Grab the valid TOC to get free space */
    if( read_mempak_sector_mod( controller, toc, sector ) )
    {
        /* Couldn't read TOC */
        return -2;
    }

    /* Verify that we have enough free space */
    if( __get_free_space( sector ) < entry->blocks )
    {
        /* Not enough space for note */
        return -4;
    }

    /* Find blocks in TOC to allocate */
    int tally = entry->blocks;
    uint8_t last = BLOCK_LAST;

    for( int i = BLOCK_VALID_FIRST; i <= BLOCK_VALID_LAST; i++ )
    {
        if( sector[(i << 1) + 1] == BLOCK_EMPTY )
        {
            /* We can use this block */
            tally--;

            /* Point this towards the next block */
            sector[(i << 1) + 1] = last;

            /* This block is now the last block */
            last = i;
        }

        /* If we found all blocks, we can exit out early */
        if( !tally ) { break; }
    }

    if( tally > 0 )
    {
        /* Even though we had free space, couldn't get all blocks? */
        return -4;
    }
    else
    {
        /* Last now contains our inode */
        entry->inode = last;
        entry->valid = 0;
        //entry->vendor = 0;                                    has been commented, because interfer with real datas.

        /* A value observed in most games */
        //entry->game_id = 0x4535;                              has been commented, because interfer with real datas.
    }

    /* Loop through allocated blocks and write data to sectors */
    for( int i = 0; i < entry->blocks; i++ )
    {
        int block = __get_note_block( sector, entry->inode, i );

        if( write_mempak_sector_mod( controller, block, data + (i * MEMPAK_BLOCK_SIZE) ) )
        {
            /* Couldn't write a sector */
            return -3;
        }
    }

    /* Find an empty entry to store to */
    for( int i = 0; i < 16; i++ )
    {
        entry_structure_t tmp_entry;

        if( read_mempak_address_mod( controller, (3 * MEMPAK_BLOCK_SIZE) + (i * 32), tmp_data ) )
        {
            /* Couldn't read note database */
            return -2;
        }

        /* See if we can write to this note */
        __read_note( tmp_data, &tmp_entry );
        if( tmp_entry.valid == 0 )
        {
            entry->entry_id = i;
            entry->valid = 1;
            break;
        }
    }

    if( entry->valid == 0 )
    {
        /* Couldn't find an entry */
        return -5;
    }

    /* Update CRC on newly updated TOC */
    sector[1] = __get_toc_checksum( sector );

    /* Write back to alternate TOC first before erasing the known valid one */
    if( write_mempak_sector_mod( controller, ( toc == 1 ) ? 2 : 1, sector ) )
    {
        /* Failed to write alternate TOC */
        return -2;
    }

    /* Write back to good TOC now that alternate is updated */
    if( write_mempak_sector_mod( controller, toc, sector ) )
    {
        /* Failed to write alternate TOC */
        return -2;
    }

    /* Convert entry structure to proper entry data */
    __write_note( entry, tmp_data );

    /* Store entry to empty slot on mempak */
    if( write_mempak_address_mod( controller, (3 * MEMPAK_BLOCK_SIZE) + (entry->entry_id * 32), tmp_data ) )
    {
        /* Couldn't update note database */
        return -2;
    }

    return 0;
}



int write_mempak_address_mod( int controller, uint16_t address, uint8_t *data )
{
    uint8_t output[64];
    uint8_t SI_write_mempak_block[64];
    int ret;

    /* Controller must be in range */
    if( controller < 0 || controller > 3 ) { return -1; }

    /* Last byte must be 0x01 to signal to the SI to process data */
    memset( SI_write_mempak_block, 0, 64 );
    SI_write_mempak_block[56] = 0xfe;
    SI_write_mempak_block[63] = 0x01;

    /* Start command at the correct channel to write from the right mempak */
    SI_write_mempak_block[controller]     = 0x23;
    SI_write_mempak_block[controller + 1] = 0x01;
    SI_write_mempak_block[controller + 2] = 0x03;

    /* Calculate CRC on address */
    uint16_t write_address = __calc_address_crc( address );
    SI_write_mempak_block[controller + 3] = (write_address >> 8) & 0xFF;
    SI_write_mempak_block[controller + 4] = write_address & 0xFF;

    /* Place the data to be written */
    memcpy( &SI_write_mempak_block[controller + 5], data, 32 );

    /* Leave room for CRC to come back */
    SI_write_mempak_block[controller + 5 + 32] = 0xFF;

    joybus_exec( SI_write_mempak_block, &output );

    /* Calculate CRC on output */
    uint8_t crc = __calc_data_crc( &output[controller + 5] );

    if( crc == output[controller + 5 + 32] )
    {
        /* Data was written successfully */
        ret = 0;
    }
    else
    {
        if( crc == (output[controller + 5 + 32] ^ 0xFF) )
        {
            /* Pak not present! */
            ret = -2;
        }
        else
        {
            /* Pak returned bad data */
            ret = -3;
        }
    }

    return ret;
}







int get_mempak_entry_mod( int controller, int entry, entry_structure_t *entry_data )
{
    uint8_t data[MEMPAK_BLOCK_SIZE];
    int toc;

    if( entry < 0 || entry > 15 ) { return -1; }
    if( entry_data == 0 ) { return -1; }

    /* Make sure Controller Pak is valid */
    if( (toc = __get_valid_toc( controller )) <= 0 )
    {
        /* Bad Controller Pak or was removed, return */
        return -2;
    }

    /* Entries are spread across two sectors, but we can luckly grab just one
       with a single Controller Pak read */
    if( read_mempak_address_mod( controller, (3 * MEMPAK_BLOCK_SIZE) + (entry * 32), data ) )
    {
        /* Couldn't read note database */
        return -2;
    }

    if( __read_note( data, entry_data ) )
    {
        /* Note is most likely empty, don't bother getting length */
        return 0;
    }

    /* Grab the TOC sector */
    if( read_mempak_sector_mod( controller, toc, data ) )
    {
        /* Couldn't read TOC */
        return -2;
    }

    /* Get the length of the entry */
    int blocks = __get_num_pages( data, entry_data->inode );

    if( blocks > 0 )
    {
        /* Valid entry */
        entry_data->blocks = blocks;
        entry_data->entry_id = entry;
        return 0;
    }
    else
    {
        /* Invalid TOC */
        entry_data->valid = 0;
        return 0;
    }
}



int write_mempak_sector_mod( int controller, int sector, uint8_t *sector_data )
{
    if( sector < 0 || sector >= 128 ) { return -1; }
    if( sector_data == 0 ) { return -1; }

    /* Sectors are 256 bytes, a mempak writes 32 bytes at a time */
    for( int i = 0; i < 8; i++ )
    {
        if( write_mempak_address_mod( controller, (sector * MEMPAK_BLOCK_SIZE) + (i * 32), sector_data + (i * 32) ) )
        {
            /* Failed to read a block */
            return -2;
        }
    }

    return 0;
}

int read_mempak_sector_mod( int controller, int sector, uint8_t *sector_data )
{
    if( sector < 0 || sector >= 128 ) { return -1; }
    if( sector_data == 0 ) { return -1; }

    /* Sectors are 256 bytes, a mempak reads 32 bytes at a time */
    for( int i = 0; i < 8; i++ )
    {
        if( read_mempak_address_mod( controller, (sector * MEMPAK_BLOCK_SIZE) + (i * 32), sector_data + (i * 32) ) )
        {
            /* Failed to read a block */
            return -2;
        }
    }

    return 0;
}

int read_mempak_address_mod( int controller, uint16_t address, uint8_t *data )
{
    uint8_t output[64];
    uint8_t SI_read_mempak_block[64];
    int ret;

    /* Controller must be in range */
    if( controller < 0 || controller > 3 ) { return -1; }

    /* Last byte must be 0x01 to signal to the SI to process data */
    memset( SI_read_mempak_block, 0, 64 );
    SI_read_mempak_block[56] = 0xfe;
    SI_read_mempak_block[63] = 0x01;

    /* Start command at the correct channel to read from the right mempak */
    SI_read_mempak_block[controller]     = 0x03;
    SI_read_mempak_block[controller + 1] = 0x21;
    SI_read_mempak_block[controller + 2] = 0x02;

    /* Calculate CRC on address */
    uint16_t read_address = __calc_address_crc( address );
    SI_read_mempak_block[controller + 3] = (read_address >> 8) & 0xFF;
    SI_read_mempak_block[controller + 4] = read_address & 0xFF;

    /* Leave room for 33 bytes (32 bytes + CRC) to come back */
    memset( &SI_read_mempak_block[controller + 5], 0xFF, 33 );

    joybus_exec( SI_read_mempak_block, &output );

    /* Copy data correctly out of command */
    memcpy( data, &output[controller + 5], 32 );

    /* Validate CRC */
    uint8_t crc = __calc_data_crc( &output[controller + 5] );

    if( crc == output[controller + 5 + 32] )
    {
        /* Data was read successfully */
        ret = 0;
    }
    else
    {
        if( crc == (output[controller + 5 + 32] ^ 0xFF) )
        {
            /* Pak not present! */
            ret = -2;
        }
        else
        {
            /* Pak returned bad data */
            ret = -3;
        }
    }

    return ret;
}

/**
 * @brief Calculate the 5 bit CRC on a mempak address
 *
 * This function, given an address intended for a mempak read or write, will
 * calculate the CRC on the address, returning the corrected address | CRC.
 *
 * @param[in] address
 *            The mempak address to calculate CRC over
 *
 * @return The mempak address | CRC
 */
static uint16_t __calc_address_crc( uint16_t address )
{
    /* CRC table */
    uint16_t xor_table[16] = { 0x0, 0x0, 0x0, 0x0, 0x0, 0x15, 0x1F, 0x0B, 0x16, 0x19, 0x07, 0x0E, 0x1C, 0x0D, 0x1A, 0x01 };
    uint16_t crc = 0;

    /* Make sure we have a valid address */
    address &= ~0x1F;

    /* Go through each bit in the address, and if set, xor the right value into the output */
    for( int i = 15; i >= 5; i-- )
    {
        /* Is this bit set? */
        if( ((address >> i) & 0x1) )
        {
           crc ^= xor_table[i];
        }
    }

    /* Just in case */
    crc &= 0x1F;

    /* Create a new address with the CRC appended */
    return address | crc;
}

/**
 * @brief Calculate the 8 bit CRC over a 32-byte block of data
 *
 * This function calculates the 8 bit CRC appropriate for checking a 32-byte
 * block of data intended for or retrieved from a mempak.
 *
 * @param[in] data
 *            Pointer to 32 bytes of data to run the CRC over
 *
 * @return The calculated 8 bit CRC over the data
 */
static uint8_t __calc_data_crc( uint8_t *data )
{
    uint8_t ret = 0;

    for( int i = 0; i <= 32; i++ )
    {
        for( int j = 7; j >= 0; j-- )
        {
            int tmp = 0;

            if( ret & 0x80 )
            {
                tmp = 0x85;
            }

            ret <<= 1;

            if( i < 32 )
            {
                if( data[i] & (0x01 << j) )
                {
                    ret |= 0x1;
                }
            }

            ret ^= tmp;
        }
    }

    return ret;
}