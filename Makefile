PROJECT_NAME = N64FlashcartMenu

.DEFAULT_GOAL := all

SOURCE_DIR = src
ASSETS_DIR = assets
BUILD_DIR = build
OUTPUT_DIR = output

include $(N64_INST)/include/n64.mk

N64_CFLAGS += -iquote $(SOURCE_DIR) -I $(SOURCE_DIR)/libs -flto=auto $(FLAGS)

SRCS = \
	main.c \
	boot/boot.c \
	boot/crc32.c \
	boot/reboot.S \
	flashcart/flashcart_utils.c \
	flashcart/flashcart.c \
	flashcart/sc64/sc64_internal.c \
	flashcart/sc64/sc64.c \
	libs/libspng/spng/spng.c \
	libs/mini.c/src/mini.c \
	libs/miniz/miniz_tdef.c \
	libs/miniz/miniz_tinfl.c \
	libs/miniz/miniz_zip.c \
	libs/miniz/miniz.c \
	menu/actions.c \
	menu/components/background.c \
	menu/components/boxart.c \
	menu/components/common.c \
	menu/components/file_list.c \
	menu/fonts.c \
	menu/menu.c \
	menu/mp3_player.c \
	menu/path.c \
	menu/png_decoder.c \
	menu/rom_database.c \
	menu/settings.c \
	menu/views/browser.c \
	menu/views/credits.c \
	menu/views/error.c \
	menu/views/fault.c \
	menu/views/file_info.c \
	menu/views/image_viewer.c \
	menu/views/load.c \
	menu/views/load_emulator.c \
	menu/views/music_player.c \
	menu/views/startup.c \
	menu/views/system_info.c \
	utils/fs.c

ASSETS = \
	FiraMono-Bold.ttf

OBJS = $(addprefix $(BUILD_DIR)/, $(addsuffix .o,$(basename $(SRCS) $(ASSETS))))
MINIZ_OBJS = $(filter $(BUILD_DIR)/libs/miniz/%.o,$(OBJS))
SPNG_OBJS = $(filter $(BUILD_DIR)/libs/libspng/%.o,$(OBJS))
DEPS = $(OBJS:.o=.d)

$(MINIZ_OBJS): N64_CFLAGS+=-DMINIZ_NO_TIME -fcompare-debug-second
$(SPNG_OBJS): N64_CFLAGS+=-isystem $(SOURCE_DIR)/libs/miniz -DSPNG_USE_MINIZ -fcompare-debug-second
$(BUILD_DIR)/FiraMono-Bold.o: MKFONT_FLAGS+=-c 0 --size 16 -r 20-7F -r 2026-2026 --ellipsis 2026,1

$(BUILD_DIR)/%.o: $(ASSETS_DIR)/%.ttf
	@echo "    [FONT] $@"
	@$(N64_MKFONT) $(MKFONT_FLAGS) -o $(ASSETS_DIR) "$<"
	@$(N64_OBJCOPY) -I binary -O elf32-bigmips -B mips4300 $(basename $<).font64 $@
	@rm $(basename $<).font64

$(BUILD_DIR)/$(PROJECT_NAME).elf: $(OBJS)

disassembly: $(BUILD_DIR)/$(PROJECT_NAME).elf
	@$(N64_OBJDUMP) -S $< > $(BUILD_DIR)/$(PROJECT_NAME).lst
.PHONY: disassembly

$(PROJECT_NAME).z64: N64_ROM_TITLE=$(PROJECT_NAME)

$(@info $(shell mkdir -p ./$(OUTPUT_DIR) &> /dev/null))

$(OUTPUT_DIR)/$(PROJECT_NAME).n64: $(PROJECT_NAME).z64
	$(shell mv $< $@)

$(BUILD_DIR)/$(PROJECT_NAME)_stripped.n64: $(OUTPUT_DIR)/$(PROJECT_NAME).n64
	$(shell python3 ./tools/strip_debug_data.py $(BUILD_DIR)/$(PROJECT_NAME).elf $< $@)
	@$(N64_CHKSUM) $@ > /dev/null

64drive: $(OUTPUT_DIR)/$(PROJECT_NAME).n64
	$(shell cp $< $(OUTPUT_DIR)/menu.bin)
.PHONY: 64drive

ed64: $(BUILD_DIR)/$(PROJECT_NAME)_stripped.n64
	$(shell cp $< $(OUTPUT_DIR)/OS64.v64)
.PHONY: ed64

ed64-clone: $(BUILD_DIR)/$(PROJECT_NAME)_stripped.n64
	$(shell cp $< $(OUTPUT_DIR)/OS64P.v64)
.PHONY: ed64-clone

sc64: $(BUILD_DIR)/$(PROJECT_NAME)_stripped.n64
	$(shell cp $< $(OUTPUT_DIR)/sc64menu.n64)
.PHONY: sc64

all: $(OUTPUT_DIR)/$(PROJECT_NAME).n64 64drive ed64 ed64-clone sc64
.PHONY: all

clean:
	$(shell rm -rf ./$(BUILD_DIR) ./$(OUTPUT_DIR))
.PHONY: clean

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

-include $(DEPS)
