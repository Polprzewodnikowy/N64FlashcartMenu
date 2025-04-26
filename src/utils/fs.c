#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <ctype.h>

#include "fs.h"
#include "utils.h"


char *strip_fs_prefix (char *path) {
    const char *prefix = ":/";
    char *found = strstr(path, prefix);
    if (found) {
        return (found + strlen(prefix) - 1);
    }
    return path;
}

char *file_basename (char *path) {
    char *base = strrchr(path, '/');
    return base ? base + 1 : path;
}

bool file_exists (char *path) {
    struct stat st;
    int error = stat(path, &st);
    return ((error == 0) && S_ISREG(st.st_mode));
}

int64_t file_get_size (char *path) {
    struct stat st;
    if (stat(path, &st)) {
        return -1;
    }
    return (int64_t) (st.st_size);
}

bool file_allocate (char *path, size_t size) {
    FILE *f;
    if ((f = fopen(path, "wb")) == NULL) {
        return true;
    }
    if (fseek(f, size, SEEK_SET)) {
        fclose(f);
        return true;
    }
    if (ftell(f) != size) {
        fclose(f);
        return true;
    }
    if (fclose(f)) {
        return true;
    }
    return false;
}

bool file_fill (char *path, uint8_t value) {
    FILE *f;
    bool error = false;
    uint8_t buffer[FS_SECTOR_SIZE * 8];
    size_t bytes_to_write;

    for (int i = 0; i < sizeof(buffer); i++) {
        buffer[i] = value;
    }

    if ((f = fopen(path, "rb+")) == NULL) {
        return true;
    }

    setbuf(f, NULL);

    fseek(f, 0, SEEK_END);
    size_t size = ftell(f);
    fseek(f, 0, SEEK_SET);

    for (size_t i = 0; i < size; i += sizeof(buffer)) {
        bytes_to_write = MIN(size - ftell(f), sizeof(buffer));
        if (fwrite(buffer, 1, bytes_to_write, f) != bytes_to_write) {
            error = true;
            break;
        }
    }

    if (fclose(f)) {
        error = true;
    }

    return error;
}

bool file_has_extensions (char *path, const char *extensions[]) {
    char *ext = strrchr(path, '.');

    if (ext == NULL) {
        return false;
    }

    while (*extensions != NULL) {
        if (strcasecmp(ext + 1, *extensions) == 0) {
            return true;
        }
        extensions++;
    }

    return false;
}

bool check_cpaksmpk(const char *filepath) {
    // Get the filename from the path
    const char *filename = strrchr(filepath, '/');
    if (filename) {
        filename++; // skip the '/'
    } else {
        filename = filepath; // no slash, the whole thing is the filename
    }

    // Check if it starts with "CPAK_"
    if (strncmp(filename, "CPAK_", 5) != 0) {
        return false; // doesn't start with CPAK_
    }

    // Find ".smpk_" in the filename
    const char *smpk_pos = strstr(filename, ".smpk_");
    if (!smpk_pos) {
        return false; // ".smpk_" not found
    }

    // Move pointer after ".smpk_"
    smpk_pos += strlen(".smpk_");

    // After ".smpk_", all characters must be digits
    while (*smpk_pos) {
        if (!isdigit((unsigned char)*smpk_pos)) {
            return false; // not a digit
        }
        smpk_pos++;
    }

    return true; // all checks passed
}


bool directory_exists (char *path) {
    struct stat st;
    int error = stat(path, &st);
    return ((error == 0) && S_ISDIR(st.st_mode));
}

bool directory_create (char *path) {
    bool error = false;

    if (directory_exists(path)) {
        return false;
    }

    char *directory = strdup(path);
    char *separator = strip_fs_prefix(directory);

    if (separator != directory) {
        separator++;
    }

    do {
        separator = strchr(separator, '/');

        if (separator != NULL) {
            *separator++ = '\0';
        }

        if (directory[0] != '\0') {
            if (mkdir(directory, 0777) && (errno != EEXIST)) {
                error = true;
                break;
            }
        }

        if (separator != NULL) {
            *(separator - 1) = '/';
        }
    } while (separator != NULL);

    free(directory);

    return error;
}
