
PROJECT_NAME = N64FlashcartMenu

.DEFAULT_GOAL := $(PROJECT_NAME)

SOURCE_DIR = src
BUILD_DIR = build
OUTPUT_DIR = output

include $(N64_INST)/include/n64.mk

N64_CFLAGS += -iquote $(SOURCE_DIR)

SRCS = \
	main.c \
	boot/boot.c \
	boot/crc32.c \
	boot/ipl2.S \
	flashcart/flashcart.c \
	flashcart/sc64/sc64_internal.c \
	flashcart/sc64/sc64.c \
	libs/toml/toml.c \
	menu/actions.c \
	menu/menu.c \
	menu/path.c \
	menu/rom_database.c \
	menu/settings.c \
	menu/views/browser.c \
	menu/views/credits.c \
	menu/views/error.c \
	menu/views/file_info.c \
	menu/views/system_info.c \
	menu/views/init.c \
	menu/views/load.c \
	utils/fs.c

OBJS = $(addprefix $(BUILD_DIR)/, $(addsuffix .o,$(basename $(SRCS))))

$(BUILD_DIR)/$(PROJECT_NAME).elf: $(OBJS)
$(BUILD_DIR)/$(PROJECT_NAME).dfs: $(wildcard rom_dfs/*)

$(PROJECT_NAME).z64: N64_ROM_TITLE=$(PROJECT_NAME)

$(PROJECT_NAME): $(PROJECT_NAME).z64
	$(shell mkdir -p $(OUTPUT_DIR))
	$(shell mv $(PROJECT_NAME).z64 $(OUTPUT_DIR))

$(PROJECT_NAME).z64: $(BUILD_DIR)/$(PROJECT_NAME).dfs

sc64_minify: $(PROJECT_NAME)
	$(shell python3 ./tools/sc64/minify.py $(BUILD_DIR)/$(PROJECT_NAME).elf $(OUTPUT_DIR)/$(PROJECT_NAME).z64 $(OUTPUT_DIR)/sc64menu.n64)

all: sc64_minify
.PHONY: all

clean:
	$(shell rm -rf ./$(BUILD_DIR) ./$(OUTPUT_DIR))
.PHONY: clean

run: $(PROJECT_NAME)
	./remotedeploy.sh
#   FIXME: improve ability to deploy.
#   if devcontainer, use remotedeploy.sh, else
# 	  $(shell sc64deployer --boot direct-rom %~dp0$(OUTPUT_DIR))\$(PROJECT_NAME).z64)
.PHONY: run

run-debug: $(PROJECT_NAME)
	./remotedeploy.sh -d
.PHONY: run-debug

# test:
#   TODO: run tests

-include $(wildcard $(BUILD_DIR)/*.d)
