EXE_NAME = N64FlashcartMenu

SOURCE_DIR = src
BUILD_DIR = build

include $(N64_INST)/include/n64.mk

N64_CFLAGS += -iquote $(SOURCE_DIR)

SRCS = \
	boot/boot.c \
	boot/crc32.c \
	boot/ipl2.S \
	flashcart/flashcart.c \
	flashcart/sc64/sc64_internal.c \
	flashcart/sc64/sc64.c \
	menu/menu.c \
	utils/fs.c \
	main.c

OBJS = $(addprefix $(BUILD_DIR)/, $(addsuffix .o,$(basename $(SRCS))))

$(BUILD_DIR)/$(EXE_NAME).elf: $(OBJS)

$(EXE_NAME).z64: N64_ROM_TITLE=$(EXE_NAME)

all: $(EXE_NAME).z64
	$(shell mv $(EXE_NAME).z64 $(BUILD_DIR))
.PHONY: all

clean:
	$(shell rm -rf ./$(BUILD_DIR))
.PHONY: clean

-include $(wildcard $(BUILD_DIR)/*.d)
