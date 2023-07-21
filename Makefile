PROJECT_NAME = N64FlashcartMenu

.DEFAULT_GOAL := $(PROJECT_NAME)

SOURCE_DIR = src
ASSETS_DIR = assets
BUILD_DIR = build
OUTPUT_DIR = output

include $(N64_INST)/include/n64.mk

N64_CFLAGS += -iquote $(SOURCE_DIR) $(FLAGS)
N64_LDFLAGS += --wrap asset_load

SRCS = \
	main.c \
	boot/boot.c \
	boot/crc32.c \
	boot/ipl2.S \
	flashcart/flashcart.c \
	flashcart/sc64/sc64_internal.c \
	flashcart/sc64/sc64.c \
	libs/mini.c/src/mini.c \
	menu/actions.c \
	menu/assets.c \
	menu/menu.c \
	menu/mp3player.c \
	menu/path.c \
	menu/rom_database.c \
	menu/settings.c \
	menu/views/browser.c \
	menu/views/credits.c \
	menu/views/error.c \
	menu/views/fault.c \
	menu/views/file_info.c \
	menu/views/fragments/fragments.c \
	menu/views/fragments/widgets.c \
	menu/views/load.c \
	menu/views/music_player.c \
	menu/views/startup.c \
	menu/views/system_info.c \
	utils/fs.c

ASSETS = \
	FiraMono-Bold.ttf

$(BUILD_DIR)/FiraMono-Bold.o: MKFONT_FLAGS+=-c 0 --size 16 -r 20-7F -r 2000-206F -r 2190-21FF

$(BUILD_DIR)/%.o: $(ASSETS_DIR)/%.ttf
	@echo "    [FONT] $@"
	@$(N64_MKFONT) $(MKFONT_FLAGS) -o $(ASSETS_DIR) "$<"
	@$(N64_OBJCOPY) -I binary -O elf32-bigmips -B mips4300 $(basename $<).font64 $@
	@rm $(basename $<).font64

OBJS = $(addprefix $(BUILD_DIR)/, $(addsuffix .o,$(basename $(SRCS) $(ASSETS))))

$(BUILD_DIR)/$(PROJECT_NAME).elf: $(OBJS)

$(PROJECT_NAME).z64: N64_ROM_TITLE=$(PROJECT_NAME)

$(PROJECT_NAME): $(PROJECT_NAME).z64
	$(shell mkdir -p $(OUTPUT_DIR))
	$(shell mv $(PROJECT_NAME).z64 $(OUTPUT_DIR))

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
