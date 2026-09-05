PROJECT_DIR = libjpeg-turbo-src
BUILD_DIR = build

include $(N64_INST)/include/n64.mk

N64_CFLAGS += -I. -I$(PROJECT_DIR)/src
CFLAGS += $(N64_CFLAGS) -fno-lto

SRCS = \
	$(PROJECT_DIR)/src/jaricom.c \
	$(PROJECT_DIR)/src/jcomapi.c \
	$(PROJECT_DIR)/src/jdapimin.c \
	$(PROJECT_DIR)/src/jdapistd.c \
	$(PROJECT_DIR)/src/jdarith.c \
	$(PROJECT_DIR)/src/jdatasrc.c \
	$(PROJECT_DIR)/src/jdcoefct.c \
	$(PROJECT_DIR)/src/jdcolor.c \
	$(PROJECT_DIR)/src/jddctmgr.c \
	$(PROJECT_DIR)/src/jdhuff.c \
	$(PROJECT_DIR)/src/jdinput.c \
	$(PROJECT_DIR)/src/jdmainct.c \
	$(PROJECT_DIR)/src/jdmarker.c \
	$(PROJECT_DIR)/src/jdmaster.c \
	$(PROJECT_DIR)/src/jdmerge.c \
	$(PROJECT_DIR)/src/jdpostct.c \
	$(PROJECT_DIR)/src/jdsample.c \
	$(PROJECT_DIR)/src/jerror.c \
	$(PROJECT_DIR)/src/jidctflt.c \
	$(PROJECT_DIR)/src/jidctfst.c \
	$(PROJECT_DIR)/src/jidctint.c \
	$(PROJECT_DIR)/src/jmemmgr.c \
	$(PROJECT_DIR)/src/jmemnobs.c \
	$(PROJECT_DIR)/src/jquant1.c \
	$(PROJECT_DIR)/src/jquant2.c \
	$(PROJECT_DIR)/src/jutils.c \
	$(PROJECT_DIR)/src/jdphuff.c \
	$(PROJECT_DIR)/src/jidctred.c

OBJS = $(SRCS:%.c=$(BUILD_DIR)/%.o)
LIB = $(BUILD_DIR)/libjpeg.a

$(BUILD_DIR)/%.o: %.c
	@mkdir -p $(dir $@)
	@echo "    [CC] $<"
	$(N64_CC) $(CFLAGS) -c -o $@ $<

$(LIB): $(OBJS)
	@mkdir -p $(dir $@)
	@echo "    [AR] $@"
	$(N64_AR) rcs $@ $^

all: $(LIB)
.PHONY: all

clean:
	rm -rf $(BUILD_DIR)
.PHONY: clean
