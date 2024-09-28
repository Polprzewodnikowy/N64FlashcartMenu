PROJECT_NAME = N64FlashcartMenu

.DEFAULT_GOAL := all

SOURCE_DIR = src
ASSETS_DIR = assets
FILESYSTEM_DIR = filesystem
BUILD_DIR = build
OUTPUT_DIR = output

MENU_VERSION ?= "Rolling release"
BUILD_TIMESTAMP = "$(shell TZ='UTC' date "+%Y-%m-%d %H:%M:%S %:z")"

include $(N64_INST)/include/n64.mk

N64_ROM_SAVETYPE = none
N64_ROM_RTC = 1
N64_ROM_REGIONFREE = 1
N64_ROM_REGION = E

N64_CFLAGS += -iquote $(SOURCE_DIR) -iquote $(ASSETS_DIR) -I $(SOURCE_DIR)/libs -flto=auto $(FLAGS)

SRCS = \
	main.c \
	boot/boot.c \
	boot/cheats.c \
	boot/cic.c \
	boot/reboot.S \
	flashcart/64drive/64drive_ll.c \
	flashcart/64drive/64drive.c \
	flashcart/flashcart_utils.c \
	flashcart/flashcart.c \
	flashcart/sc64/sc64_ll.c \
	flashcart/sc64/sc64.c \
	libs/libspng/spng/spng.c \
	libs/mini.c/src/mini.c \
	libs/miniz/miniz_tdef.c \
	libs/miniz/miniz_tinfl.c \
	libs/miniz/miniz_zip.c \
	libs/miniz/miniz.c \
	menu/actions.c \
	menu/cart_load.c \
	menu/components/background.c \
	menu/components/boxart.c \
	menu/components/common.c \
	menu/components/context_menu.c \
	menu/components/file_list.c \
	menu/disk_info.c \
	menu/fonts.c \
	menu/hdmi.c \
	menu/menu.c \
	menu/mp3_player.c \
	menu/path.c \
	menu/png_decoder.c \
	menu/rom_info.c \
	menu/settings.c \
	menu/sound.c \
	menu/usb_comm.c \
	menu/views/browser.c \
	menu/views/credits.c \
	menu/views/error.c \
	menu/views/fault.c \
	menu/views/file_info.c \
	menu/views/image_viewer.c \
	menu/views/text_viewer.c \
	menu/views/load_disk.c \
	menu/views/load_emulator.c \
	menu/views/load_rom.c \
	menu/views/music_player.c \
	menu/views/startup.c \
	menu/views/system_info.c \
	menu/views/settings_editor.c \
	menu/views/rtc.c \
	menu/views/flashcart_info.c \
	utils/fs.c

FONTS = \
	FiraMonoBold.ttf

SOUNDS = \
	cursorsound.wav \
	back.wav \
	enter.wav \
	error.wav \
	settings.wav

OBJS = $(addprefix $(BUILD_DIR)/, $(addsuffix .o,$(basename $(SRCS))))
MINIZ_OBJS = $(filter $(BUILD_DIR)/libs/miniz/%.o,$(OBJS))
SPNG_OBJS = $(filter $(BUILD_DIR)/libs/libspng/%.o,$(OBJS))
DEPS = $(OBJS:.o=.d)

FILESYSTEM = \
	$(addprefix $(FILESYSTEM_DIR)/, $(notdir $(FONTS:%.ttf=%.font64))) \
	$(addprefix $(FILESYSTEM_DIR)/, $(notdir $(SOUNDS:%.wav=%.wav64)))

$(MINIZ_OBJS): N64_CFLAGS+=-DMINIZ_NO_TIME -fcompare-debug-second
$(SPNG_OBJS): N64_CFLAGS+=-isystem $(SOURCE_DIR)/libs/miniz -DSPNG_USE_MINIZ -fcompare-debug-second
$(FILESYSTEM_DIR)/FiraMonoBold.font64: MKFONT_FLAGS+=-c 1 --size 16 -r 20-7F -r 80-1FF -r 2026-2026 --ellipsis 2026,1
$(FILESYSTEM_DIR)/%.wav64: AUDIOCONV_FLAGS=--wav-compress 1

$(@info $(shell mkdir -p ./$(FILESYSTEM_DIR) &> /dev/null))

$(FILESYSTEM_DIR)/%.font64: $(ASSETS_DIR)/%.ttf
	@echo "    [FONT] $@"
	@$(N64_MKFONT) $(MKFONT_FLAGS) -o $(FILESYSTEM_DIR) "$<"

$(FILESYSTEM_DIR)/%.wav64: $(ASSETS_DIR)/%.wav
	@echo "    [AUDIO] $@"
	@$(N64_AUDIOCONV) $(AUDIOCONV_FLAGS) -o $(FILESYSTEM_DIR) "$<"

$(BUILD_DIR)/$(PROJECT_NAME).dfs: $(FILESYSTEM)

$(BUILD_DIR)/menu/views/credits.o: .FORCE
$(BUILD_DIR)/menu/views/credits.o: FLAGS+=-DMENU_VERSION=\"$(MENU_VERSION)\" -DBUILD_TIMESTAMP=\"$(BUILD_TIMESTAMP)\"

$(BUILD_DIR)/$(PROJECT_NAME).elf: $(OBJS)

disassembly: $(BUILD_DIR)/$(PROJECT_NAME).elf
	@$(N64_OBJDUMP) -S $< > $(BUILD_DIR)/$(PROJECT_NAME).lst
.PHONY: disassembly

$(PROJECT_NAME).z64: N64_ROM_TITLE=$(PROJECT_NAME)
$(PROJECT_NAME).z64: $(BUILD_DIR)/$(PROJECT_NAME).dfs

$(@info $(shell mkdir -p ./$(OUTPUT_DIR) &> /dev/null))

$(OUTPUT_DIR)/$(PROJECT_NAME).n64: $(PROJECT_NAME).z64
	@mv $< $@

64drive: $(OUTPUT_DIR)/$(PROJECT_NAME).n64
	@cp $< $(OUTPUT_DIR)/menu.bin
.PHONY: 64drive

ed64: $(OUTPUT_DIR)/$(PROJECT_NAME).n64
	@cp $< $(OUTPUT_DIR)/OS64.v64
.PHONY: ed64

ed64-clone: $(OUTPUT_DIR)/$(PROJECT_NAME).n64
	@cp $< $(OUTPUT_DIR)/OS64P.v64
.PHONY: ed64-clone

sc64: $(OUTPUT_DIR)/$(PROJECT_NAME).n64
	@cp $< $(OUTPUT_DIR)/sc64menu.n64
.PHONY: sc64

all: $(OUTPUT_DIR)/$(PROJECT_NAME).n64 64drive ed64 ed64-clone sc64
.PHONY: all

clean:
	@rm -f ./$(FILESYSTEM)
	@find ./$(FILESYSTEM_DIR) -type d -empty -delete
	@rm -rf ./$(BUILD_DIR) ./$(OUTPUT_DIR)
.PHONY: clean

format:
	@find ./$(SOURCE_DIR) \
		-path \./$(SOURCE_DIR)/libs -prune \
		-o -iname *.c -print \
		-o -iname *.h -print \
		| xargs clang-format -i

run: $(OUTPUT_DIR)/$(PROJECT_NAME).n64
ifeq ($(OS),Windows_NT)
	./localdeploy.bat
else
	./remotedeploy.sh
endif
.PHONY: run

run-debug: $(OUTPUT_DIR)/$(PROJECT_NAME).n64
ifeq ($(OS),Windows_NT)
	./localdeploy.bat /d
else
	./remotedeploy.sh -d
endif
.PHONY: run-debug

# test:
#   TODO: run tests

.FORCE:

-include $(DEPS)
