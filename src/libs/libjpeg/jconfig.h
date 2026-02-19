/* jconfig.h — IJG libjpeg v10 configuration for mips64-elf / N64 (libdragon)
 * Hand-written for a modern freestanding C99 environment.
 * No configure or cmake required.
 */

#define HAVE_PROTOTYPES
#define HAVE_UNSIGNED_CHAR
#define HAVE_UNSIGNED_SHORT
#define HAVE_STDDEF_H
#define HAVE_STDLIB_H
#undef  NEED_BSD_STRINGS
#undef  NEED_SYS_TYPES_H
#undef  NEED_FAR_POINTERS
#undef  NEED_SHORT_EXTERNAL_NAMES
#undef  INCOMPLETE_TYPES_BROKEN

#ifdef JPEG_INTERNALS
#undef RIGHT_SHIFT_IS_UNSIGNED
#endif
