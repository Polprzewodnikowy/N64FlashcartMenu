/**
 * @file constants.h
 * @brief Menu components constants
 * @ingroup menu
 */

#ifndef COMPONENTS_CONSTANTS_H__
#define COMPONENTS_CONSTANTS_H__

/** @brief The height of the tabs in the main menu. */
#define TAB_HEIGHT                      (20)

/** @brief The thickness of borders. */
#define BORDER_THICKNESS                (4)

/** @brief The display width. */
#define DISPLAY_WIDTH                   (640)
/** @brief The display height. */
#define DISPLAY_HEIGHT                  (480)

/** @brief The centre of the display on the X axis. */
#define DISPLAY_CENTER_X                (DISPLAY_WIDTH / 2)
/** @brief The centre of the display on the Y axis. */
#define DISPLAY_CENTER_Y                (DISPLAY_HEIGHT / 2)

/** @brief The overscan on the X axis. */
#define OVERSCAN_WIDTH                  (32)
/** @brief The overscan on the Y axis. */
#define OVERSCAN_HEIGHT                 (24)

/** @brief The start position of the visible display on the X axis. */
#define VISIBLE_AREA_X0                 (OVERSCAN_WIDTH)
/** @brief The start position of the visible display on the Y axis. */
#define VISIBLE_AREA_Y0                 (OVERSCAN_HEIGHT)
/** @brief The end position of the visible display on the X axis. */
#define VISIBLE_AREA_X1                 (DISPLAY_WIDTH - OVERSCAN_WIDTH)
/** @brief The end position of the visible display on the Y axis. */
#define VISIBLE_AREA_Y1                 (DISPLAY_HEIGHT - OVERSCAN_HEIGHT)

/** @brief The width of the visible display. */
#define VISIBLE_AREA_WIDTH              (VISIBLE_AREA_X1 - VISIBLE_AREA_X0)
/** @brief The height of the visible display. */
#define VISIBLE_AREA_HEIGHT             (VISIBLE_AREA_Y1 - VISIBLE_AREA_Y0)

/** @brief The layout actions separator Y position. */
#define LAYOUT_ACTIONS_SEPARATOR_Y      (400)

/** @brief The seek bar height. */
#define SEEKBAR_HEIGHT                  (24)
/** @brief The seek bar width. */
#define SEEKBAR_WIDTH                   (524)
/** @brief The seek bar position on the X axis. */
#define SEEKBAR_X                       (DISPLAY_CENTER_X - (SEEKBAR_WIDTH / 2))
/** @brief The seek bar position on the Y axis. */
#define SEEKBAR_Y                       (VISIBLE_AREA_Y1 - SEEKBAR_HEIGHT - 80)

/** @brief The loader bar width. */
#define LOADER_WIDTH                    (320)
/** @brief The loader bar height. */
#define LOADER_HEIGHT                   (24)
/** @brief The loader bar position on the X axis. */
#define LOADER_X                        (DISPLAY_CENTER_X - (LOADER_WIDTH / 2))
/** @brief The loader bar position on the Y axis. */
#define LOADER_Y                        (DISPLAY_CENTER_Y - (LOADER_HEIGHT / 2))

/** @brief The maximum width of a message box. */
#define MESSAGEBOX_MAX_WIDTH            (360)
/** @brief The margin of a message box. */
#define MESSAGEBOX_MARGIN               (32)

/** @brief The horizontal text margin. */
#define TEXT_MARGIN_HORIZONTAL          (10)
/** @brief The vertical text margin. */
#define TEXT_MARGIN_VERTICAL            (6)
/** @brief The vertical text offset. */
#define TEXT_OFFSET_VERTICAL            (1)
/** @brief The text line spacing adjustment. */
#define TEXT_LINE_SPACING_ADJUST        (0)

/** @brief The boxart picture width. */
#define BOXART_WIDTH                    (158)
/** @brief The boxart picture height. */
#define BOXART_HEIGHT                   (112)

/** @brief The boxart picture width (64DD). */
#define BOXART_WIDTH_DD                 (129)
/** @brief The boxart picture height (64DD). */
#define BOXART_HEIGHT_DD                (112)

/** @brief The boxart picture maximum width. */
#define BOXART_WIDTH_MAX                (158)
/** @brief The boxart picture maximum height. */
#define BOXART_HEIGHT_MAX               (158)

/** @brief The box art position on the X axis. */
#define BOXART_X                        (VISIBLE_AREA_X1 - BOXART_WIDTH - 24)
/** @brief The box art position on the Y axis. */
#define BOXART_Y                        (LAYOUT_ACTIONS_SEPARATOR_Y - BOXART_HEIGHT - 24)
/** @brief The box art position on the X axis for Japanese caratules. */
#define BOXART_X_JP                     (VISIBLE_AREA_X1 - BOXART_WIDTH_MAX + 21)
/** @brief The box art position on the Y axis for Japanese caratules. */
#define BOXART_Y_JP                     (LAYOUT_ACTIONS_SEPARATOR_Y - BOXART_HEIGHT_MAX - 24)

/** @brief The box art position on the X axis for 64DD caratules. */
#define BOXART_X_DD                     (VISIBLE_AREA_X1 - BOXART_WIDTH_DD - 23)
/** @brief The box art position on the Y axis for 64DD caratules. */
#define BOXART_Y_DD                     (LAYOUT_ACTIONS_SEPARATOR_Y - BOXART_HEIGHT_DD - 24)

/** @brief The scroll bar width. */
#define LIST_SCROLLBAR_WIDTH            (12)
/** @brief The scroll bar height. */
#define LIST_SCROLLBAR_HEIGHT           (LAYOUT_ACTIONS_SEPARATOR_Y - OVERSCAN_HEIGHT - TAB_HEIGHT - BORDER_THICKNESS)
/** @brief The scroll bar position on the X axis. */
#define LIST_SCROLLBAR_X                (VISIBLE_AREA_X1 - LIST_SCROLLBAR_WIDTH)
/** @brief The scroll bar position on the Y axis. */
#define LIST_SCROLLBAR_Y                (VISIBLE_AREA_Y0 + TAB_HEIGHT + BORDER_THICKNESS)

/** @brief The maximum amount of file list entries. */
#define LIST_ENTRIES                    (18)
/** @brief The maximum width available for a file list entry. */
#define FILE_LIST_MAX_WIDTH             (480)
/** @brief The file list highlight width. */
#define FILE_LIST_HIGHLIGHT_WIDTH       (VISIBLE_AREA_X1 - VISIBLE_AREA_X0 - LIST_SCROLLBAR_WIDTH)
/** @brief The file list highlight X position. */
#define FILE_LIST_HIGHLIGHT_X           (VISIBLE_AREA_X0)

/** @brief The default background colour. */
#define BACKGROUND_EMPTY_COLOR          RGBA32(0x00, 0x00, 0x00, 0xFF)
/** @brief The default background overlay colour. */
#define BACKGROUND_OVERLAY_COLOR        RGBA32(0x00, 0x00, 0x00, 0xA0)

/** @brief The border colour. */
#define BORDER_COLOR                    RGBA32(0xFF, 0xFF, 0xFF, 0xFF)

/** @brief The progress bar background colour. */
#define PROGRESSBAR_BG_COLOR            RGBA32(0x00, 0x00, 0x00, 0xFF)
/** @brief The progress bar progressed colour. */
#define PROGRESSBAR_DONE_COLOR          RGBA32(0x3B, 0x7C, 0xF5, 0xFF)

/** @brief The scroll bar background colour. */
#define SCROLLBAR_BG_COLOR              RGBA32(0x3F, 0x3F, 0x3F, 0xFF)
/** @brief The scroll bar inactive colour. */
#define SCROLLBAR_INACTIVE_COLOR        RGBA32(0x5F, 0x5F, 0x5F, 0xFF)
/** @brief The scroll bar position colour. */
#define SCROLLBAR_POSITION_COLOR        RGBA32(0x7F, 0x7F, 0x7F, 0xFF)

/** @brief The dialogue background colour. */
#define DIALOG_BG_COLOR                 RGBA32(0x00, 0x00, 0x00, 0xFF)

/** @brief The boxart loading colour. */
#define BOXART_LOADING_COLOR            RGBA32(0x3F, 0x3F, 0x3F, 0xFF)

/** @brief The file list highlight colour. */
#define FILE_LIST_HIGHLIGHT_COLOR       RGBA32(0x3F, 0x3F, 0x3F, 0xFF)

/** @brief The menu highlight colour. */
#define CONTEXT_MENU_HIGHLIGHT_COLOR    RGBA32(0x3F, 0x3F, 0x3F, 0xFF)

/** @brief The tab inactive border colour. */
#define TAB_INACTIVE_BORDER_COLOR       RGBA32(0x5F, 0x5F, 0x5F, 0xFF)
/** @brief The tab active border colour. */
#define TAB_ACTIVE_BORDER_COLOR         RGBA32(0xFF, 0xFF, 0xFF, 0xFF)
/** @brief The tab inactive background colour. */
#define TAB_INACTIVE_BACKGROUND_COLOR   RGBA32(0x3F, 0x3F, 0x3F, 0xFF)
/** @brief The tab active background colour. */
#define TAB_ACTIVE_BACKGROUND_COLOR     RGBA32(0x6F, 0x6F, 0x6F, 0xFF)

#endif /* COMPONENTS_CONSTANTS_H__ */
