/**
 * @file usb_comm.c
 * @brief USB communication component implementation
 * @ingroup ui_components
 */

// NOTE: This code doesn't implement EverDrive-64 USB protocol.
//       Main use of these functions is to aid menu development
//       (for example replace files on the SD card or reboot menu).

#include <stdio.h>
#include <string.h>

#include <usb.h>

#include "usb_comm.h"
#include "utils/utils.h"

#define MAX_FILE_SIZE   MiB(4)

/** @brief The supported USB commands structure. */
typedef struct {
    /** @brief The command identifier. */
    const char *id;

    /** @brief The command operation. */
    void (*op) (menu_t *menu);
} usb_comm_command_t;

/**
 * @brief Get a character from the USB input.
 * 
 * @return int The character read, or -1 if no character is available.
 */
static int usb_comm_get_char (void) {
    char c;

    if (USBHEADER_GETSIZE(usb_poll()) <= 0) {
        return -1;
    }

    usb_read(&c, sizeof(c));

    return (int) (c);
}

/**
 * @brief Read a string from the USB input.
 * 
 * @param string Buffer to store the string.
 * @param length Maximum length of the string.
 * @param end Character indicating the end of the string.
 * @return true if the string was read successfully, false otherwise.
 */
static bool usb_comm_read_string (char *string, int length, char end) {
    for (int i = 0; i < length; i++) {
        int c = usb_comm_get_char();

        if (c < 0) {
            return true;
        }

        string[i] = (char) (c);

        if (c == '\0' || c == end) {
            string[i] = '\0';
            break;
        }

        if (i == (length - 1)) {
            return true;
        }
    }

    return false;
}

/**
 * @brief Send an error message over USB.
 * 
 * @param message The error message.
 */
static void usb_comm_send_error (const char *message) {
    usb_purge();
    usb_write(DATATYPE_TEXT, message, strlen(message));
}

/**
 * @brief Reboot the system.
 * 
 * @param menu Pointer to the menu structure.
 */
static void command_reboot (menu_t *menu) {
    menu->next_mode = MENU_MODE_BOOT;

    menu->boot_params->device_type = BOOT_DEVICE_TYPE_ROM;
    menu->boot_params->tv_type = BOOT_TV_TYPE_PASSTHROUGH;
    menu->boot_params->detect_cic_seed = true;
    menu->boot_params->cheat_list = NULL;
}

/**
 * @brief Receive a file over USB and save it to the storage.
 * 
 * @param menu Pointer to the menu structure.
 */
static void command_receive_file (menu_t *menu) {
    FILE *f;
    char buffer[256];
    uint8_t data[8192];
    char length[8];

    if (usb_comm_read_string(buffer, sizeof(buffer), ' ')) {
        return usb_comm_send_error("Invalid path argument\n");
    }

    if (usb_comm_get_char() != '@') {
        return usb_comm_send_error("Invalid argument\n");
    }

    if (usb_comm_read_string(length, sizeof(length), '@')) {
        return usb_comm_send_error("Invalid file length argument\n");
    }

    path_t *path = path_init(menu->storage_prefix, buffer);

    if ((f = fopen(path_get(path), "wb")) == NULL) {
        path_free(path);
        return usb_comm_send_error("Couldn't create file\n");
    }
    setbuf(f, NULL);
    path_free(path);

    int remaining = atoi(length);

    if (remaining > MAX_FILE_SIZE) {
        return usb_comm_send_error("File size too big\n");
    }

    while (remaining > 0) {
        int block_size = MIN(remaining, sizeof(data));
        usb_read(data, block_size);
        if (fwrite(data, 1, block_size, f) != block_size) {
            fclose(f);
            return usb_comm_send_error("Couldn't write all required data to the file\n");
        }
        remaining -= block_size;
    }

    if (fclose(f)) {
        return usb_comm_send_error("Couldn't flush data to the file\n");
    }

    if (usb_comm_get_char() != '\0') {
        return usb_comm_send_error("Invalid token at the end of data stream\n");
    }
}

static usb_comm_command_t commands[] = {
    { .id = "reboot", .op = command_reboot },
    { .id = "send-file", .op = command_receive_file }, // Note that this is a crossover with the `id` related to the PC commands.
    { .id = NULL },
};

/**
 * @brief Poll the USB input for commands.
 * 
 * @param menu Pointer to the menu structure.
 */
void usb_comm_poll (menu_t *menu) {
    uint32_t header = usb_poll();

    if (USBHEADER_GETTYPE(header) != DATATYPE_TEXT) {
        usb_purge();
        return;
    }

    if (USBHEADER_GETSIZE(header) > 0) {
        char cmd_id[32];

        if (usb_comm_read_string(cmd_id, sizeof(cmd_id), ' ')) {
            usb_comm_send_error("Command id too long\n");
        } else {
            usb_comm_command_t *cmd = commands;

            while (cmd->id != NULL) {
                if (strcmp(cmd->id, cmd_id) == 0) {
                    cmd->op(menu);
                    break;
                }
                cmd++;
            }

            usb_purge();

            if (cmd->id == NULL) {
                usb_comm_send_error("Unknown command\n");
            }
        }
    }
}
