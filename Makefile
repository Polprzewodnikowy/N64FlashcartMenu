.DEFAULT_GOAL := all

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

sc64menu.n64: $(EXE_NAME).z64
	ls -R
	echo test input: $<
	echo test output: $@
	python3 ./tools/sc64/minify.py $(BUILD_DIR)/$(EXE_NAME).elf ./$(BUILD_DIR)/$< ./$(OUTPUT_DIR)/$@

all: sc64menu.n64
.PHONY: all

clean:
	$(shell rm -rf ./$(BUILD_DIR) ./$(OUTPUT_DIR) sc64menu.n64 $(EXE_NAME).z64)
.PHONY: clean

-include $(wildcard $(BUILD_DIR)/*.d)
