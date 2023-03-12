.DEFAULT_GOAL := menu

EXE_NAME = N64FlashcartMenu

SOURCE_DIR = src
BUILD_DIR = build
OUTPUT_DIR = output

include $(N64_INST)/include/n64.mk

N64_CFLAGS += -iquote $(SOURCE_DIR)

SRCS = \
	boot/boot.c \
	boot/crc32.c \
	boot/ipl2.S \
	flashcart/flashcart.c \
	flashcart/sc64/sc64_internal.c \
	flashcart/sc64/sc64.c \
	menu/menu_main.c \
	menu/menu_info.c \
	menu/menu_fileinfo.c \
	menu/settings.c \
	menu/rom_database.c \
	utils/fs.c \
	libs/toml/toml.c \
	main.c

OBJS = $(addprefix $(BUILD_DIR)/, $(addsuffix .o,$(basename $(SRCS))))

$(BUILD_DIR)/$(EXE_NAME).elf: $(OBJS)

$(EXE_NAME).z64: N64_ROM_TITLE=$(EXE_NAME)

 menu: $(EXE_NAME).z64
	$(shell mkdir -p $(OUTPUT_DIR))
	$(shell mv $(EXE_NAME).z64 $(OUTPUT_DIR)) 

sc64_minify:
	$(shell python3 ./tools/sc64/minify.py $(BUILD_DIR)/$(EXE_NAME).elf $(OUTPUT_DIR)/N64FlashcartMenu.z64 $(OUTPUT_DIR)/sc64menu.n64)

all: menu sc64_minify
.PHONY: all

clean:
	$(shell rm -rf ./$(BUILD_DIR))
	$(shell rm -rf ./$(OUTPUT_DIR))
.PHONY: clean

# run:

# test:

-include $(wildcard $(BUILD_DIR)/*.d)
