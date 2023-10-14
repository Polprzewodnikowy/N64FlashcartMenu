/**
 * @file constants.h
 * @brief Menu components constants
 * @ingroup menu
 */

#ifndef COMPONENTS_CONSTANTS_H__
#define COMPONENTS_CONSTANTS_H__


#define DISPLAY_WIDTH                   (640)
#define DISPLAY_HEIGHT                  (480)

#define DISPLAY_CENTER_X                (DISPLAY_WIDTH / 2)
#define DISPLAY_CENTER_Y                (DISPLAY_HEIGHT / 2)

#define OVERSCAN_WIDTH                  (32)
#define OVERSCAN_HEIGHT                 (24)

#define VISIBLE_AREA_X0                 (OVERSCAN_WIDTH)
#define VISIBLE_AREA_Y0                 (OVERSCAN_HEIGHT)
#define VISIBLE_AREA_X1                 (DISPLAY_WIDTH - OVERSCAN_WIDTH)
#define VISIBLE_AREA_Y1                 (DISPLAY_HEIGHT - OVERSCAN_HEIGHT)

#define VISIBLE_AREA_WIDTH              (VISIBLE_AREA_X1 - VISIBLE_AREA_X0)
#define VISIBLE_AREA_HEIGHT             (VISIBLE_AREA_Y1 - VISIBLE_AREA_Y0)

#define BORDER_THICKNESS                (4)

#define LAYOUT_ACTIONS_SEPARATOR_Y      (400)

#define SEEKBAR_HEIGHT                  (24)
#define SEEKBAR_WIDTH                   (524)
#define SEEKBAR_X                       (DISPLAY_CENTER_X - (SEEKBAR_WIDTH / 2))
#define SEEKBAR_Y                       (VISIBLE_AREA_Y1 - SEEKBAR_HEIGHT - 80)

#define LOADER_WIDTH                    (320)
#define LOADER_HEIGHT                   (24)
#define LOADER_X                        (DISPLAY_CENTER_X - (LOADER_WIDTH / 2))
#define LOADER_Y                        (DISPLAY_CENTER_Y - (LOADER_HEIGHT / 2))

#define MESSAGEBOX_MAX_WIDTH            (360)
#define MESSAGEBOX_MARGIN               (32)

#define TEXT_MARGIN_HORIZONTAL          (10)
#define TEXT_MARGIN_VERTICAL            (7)

#define BOXART_WIDTH                    (158)
#define BOXART_HEIGHT                   (112)
#define BOXART_X                        (VISIBLE_AREA_X1 - BOXART_WIDTH - 24)
#define BOXART_Y                        (LAYOUT_ACTIONS_SEPARATOR_Y - BOXART_HEIGHT - 24)

#define FILE_LIST_SCROLLBAR_WIDTH       (12)
#define FILE_LIST_SCROLLBAR_HEIGHT      (LAYOUT_ACTIONS_SEPARATOR_Y - OVERSCAN_HEIGHT)
#define FILE_LIST_SCROLLBAR_X           (VISIBLE_AREA_X1 - FILE_LIST_SCROLLBAR_WIDTH)
#define FILE_LIST_SCROLLBAR_Y           (VISIBLE_AREA_Y0)

#define FILE_LIST_ENTRIES               (20)
#define FILE_LIST_MAX_WIDTH             (480)
#define FILE_LIST_HIGHLIGHT_WIDTH       (VISIBLE_AREA_X1 - VISIBLE_AREA_X0 - FILE_LIST_SCROLLBAR_WIDTH)
#define FILE_LIST_HIGHLIGHT_X           (VISIBLE_AREA_X0)


#define BACKGROUND_EMPTY_COLOR          RGBA32(0x00, 0x00, 0x00, 0xFF)
#define BACKGROUND_OVERLAY_COLOR        RGBA32(0x00, 0x00, 0x00, 0xA0)

#define BORDER_COLOR                    RGBA32(0xFF, 0xFF, 0xFF, 0xFF)

#define PROGRESSBAR_BG_COLOR            RGBA32(0x00, 0x00, 0x00, 0xFF)
#define PROGRESSBAR_DONE_COLOR          RGBA32(0x3B, 0x7C, 0xF5, 0xFF)

#define SCROLLBAR_BG_COLOR              RGBA32(0x3F, 0x3F, 0x3F, 0xFF)
#define SCROLLBAR_INACTIVE_COLOR        RGBA32(0x5F, 0x5F, 0x5F, 0xFF)
#define SCROLLBAR_POSITION_COLOR        RGBA32(0x7F, 0x7F, 0x7F, 0xFF)

#define DIALOG_BG_COLOR                 RGBA32(0x00, 0x00, 0x00, 0xFF)

#define BOXART_LOADING_COLOR            RGBA32(0x3F, 0x3F, 0x3F, 0xFF)

#define FILE_LIST_HIGHLIGHT_COLOR       RGBA32(0x3F, 0x3F, 0x3F, 0xFF)

#define CONTEXT_MENU_HIGHLIGHT_COLOR    RGBA32(0x3F, 0x3F, 0x3F, 0xFF)


#endif
