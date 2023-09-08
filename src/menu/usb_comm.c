// NOTE: This code doesn't implement EverDrive-64 USB protocol.
//       Main use of these functions is to aid menu development
//       (for example replace files on the SD card or reboot menu).

#include <fatfs/ff.h>
#include <string.h>
#include <usb.h>

#include "usb_comm.h"
#include "utils/utils.h"


#define MAX_FILE_SIZE   MiB(4)


typedef struct {
    const char *id;
    void (*op) (menu_t *menu);
} usb_comm_command_t;


static int usb_comm_get_char (void) {
    char c;

    if (USBHEADER_GETSIZE(usb_poll()) <= 0) {
        return -1;
    }

    usb_read(&c, sizeof(c));

    return (int) (c);
}

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

static void usb_comm_send_error (const char *message) {
    usb_purge();
    usb_write(DATATYPE_TEXT, message, strlen(message));
}


static void command_reboot (menu_t *menu) {
    menu->next_mode = MENU_MODE_BOOT;

    menu->boot_params->device_type = BOOT_DEVICE_TYPE_ROM;
    menu->boot_params->tv_type = BOOT_TV_TYPE_PASSTHROUGH;
    menu->boot_params->detect_cic_seed = true;
};

static void command_send_file (menu_t *menu) {
    char path[256];
    char length[8];

    FIL f;
    int remaining;
    uint8_t data[8192];
    UINT bytes_written;

    if (usb_comm_read_string(path, sizeof(path), ' ')) {
        return usb_comm_send_error("Invalid path argument\n");
    }

    if (usb_comm_get_char() != '@') {
        return usb_comm_send_error("Invalid argument\n");
    }

    if (usb_comm_read_string(length, sizeof(length), '@')) {
        return usb_comm_send_error("Invalid file length argument\n");
    }

    if (f_open(&f, path, FA_CREATE_ALWAYS | FA_WRITE) != FR_OK) {
        return usb_comm_send_error("Couldn't create file\n");
    }

    remaining = atoi(length);

    if (remaining > MAX_FILE_SIZE) {
        return usb_comm_send_error("File size too big\n");
    }

    while (remaining > 0) {
        int block_size = MIN(remaining, sizeof(data));
        usb_read(data, block_size);
        if (f_write(&f, data, block_size, &bytes_written) != FR_OK) {
            f_close(&f);
            return usb_comm_send_error("Couldn't write data to the file\n");
        }
        if (bytes_written != block_size) {
            f_close(&f);
            return usb_comm_send_error("Couldn't write all required data to the file\n");
        }
        remaining -= block_size;
    }

    if (f_close(&f) != FR_OK) {
        return usb_comm_send_error("Couldn't flush data to the file\n");
    }

    if (usb_comm_get_char() != '\0') {
        return usb_comm_send_error("Invalid token at the end of data stream\n");
    }
}

static usb_comm_command_t commands[] = {
    { .id = "reboot", .op = command_reboot },
    { .id = "send-file", .op = command_send_file },
    { .id = NULL },
};


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
