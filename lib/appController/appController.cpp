
/**
 * @file appController.c
 *
 */

/*********************
 *      INCLUDES
 *********************/
#include "appController.h"
#include <Arduino.h>
#include "lvgl.h"
#include "sketchpad.h"

/*********************
 *      DEFINES
 *********************/

/**********************
 *      TYPEDEFS
 **********************/

/**********************
 *  STATIC PROTOTYPES
 **********************/

static lv_obj_t *_sketchpad;
static uint8_t _drawline_flag = 0;
static void sketchpad_toolbar_event_cb(lv_event_t *e);
static void toolbar_set_event_cb(lv_event_t *e);
/**********************
 * STATIC FUNCTIONS
 **********************/

/**********************
 *  LIB API
 **********************/
void app_controller_init(void)
{

    lv_draw_buf_t *draw_buf = lv_draw_buf_create(240, 300, LV_COLOR_FORMAT_ARGB8888, LV_STRIDE_AUTO);
    _sketchpad = lv_100ask_sketchpad_create(lv_screen_active());
    lv_obj_set_scrollbar_mode(_sketchpad, LV_SCROLLBAR_MODE_OFF); // See no different in PC, but may have some difference in the TFT display
    lv_obj_set_scroll_snap_x(_sketchpad, LV_SCROLL_SNAP_NONE);
    lv_obj_set_scroll_snap_y(_sketchpad, LV_SCROLL_SNAP_NONE);
    lv_obj_clear_flag(_sketchpad, LV_OBJ_FLAG_SCROLL_ELASTIC); // Disable elastic scrolling>>not work
    lv_obj_clear_flag(_sketchpad, LV_OBJ_FLAG_SCROLL_CHAIN);   // THis work

    lv_canvas_set_draw_buf(_sketchpad, draw_buf);
    // void* ptr3 = draw_buf->data;

    // Serial.printf("Init: draw buffer address = %08x\r\n", ptr3);
    lv_canvas_fill_bg(_sketchpad, lv_color_hex3(0xccc), LV_OPA_COVER);
    lv_obj_align(_sketchpad, LV_ALIGN_BOTTOM_MID, 0, 0);

    static lv_coord_t sketchpad_toolbar_cw = LV_100ASK_SKETCHPAD_TOOLBAR_OPT_DELETE;
    lv_obj_t *color = lv_label_create(lv_screen_active());
    lv_label_set_text(color, LV_SYMBOL_TRASH);
    lv_obj_add_flag(color, LV_OBJ_FLAG_CLICKABLE);
    lv_obj_align(color, LV_ALIGN_TOP_MID, -10, 0);
    lv_obj_add_event_cb(color, sketchpad_toolbar_event_cb, LV_EVENT_ALL, &sketchpad_toolbar_cw);
    lv_area_t area;
    lv_obj_get_click_area(color, &area);
    Serial.printf("Tash Click area: x1=%d, y1=%d, x2=%d, y2=%d\r\n", area.x1, area.y1, area.x2, area.y2);
    lv_obj_set_ext_click_area(color, 5);
    lv_obj_get_click_area(color, &area);
    Serial.printf("Extended Tash Click area: x1=%d, y1=%d, x2=%d, y2=%d\r\n", area.x1, area.y1, area.x2, area.y2);

    static lv_coord_t sketchpad_toolbar_width = LV_100ASK_SKETCHPAD_TOOLBAR_OPT_WIDTH;
    lv_obj_t *size = lv_label_create(lv_screen_active());
    lv_label_set_text(size, LV_SYMBOL_EJECT);
    lv_obj_add_flag(size, LV_OBJ_FLAG_CLICKABLE);
    lv_obj_align(size, LV_ALIGN_TOP_MID, 50, 0);
    lv_obj_add_event_cb(size, sketchpad_toolbar_event_cb, LV_EVENT_ALL, &sketchpad_toolbar_width);
    lv_obj_get_click_area(size, &area);
    Serial.printf("size Click area: x1=%d, y1=%d, x2=%d, y2=%d\r\n", area.x1, area.y1, area.x2, area.y2);
    lv_obj_set_ext_click_area(size, 5);
    lv_obj_get_click_area(size, &area);
    Serial.printf("Extended size Click area: x1=%d, y1=%d, x2=%d, y2=%d\r\n", area.x1, area.y1, area.x2, area.y2);
}

static void sketchpad_toolbar_event_cb(lv_event_t *e)
{
    lv_coord_t *toolbar_opt = (lv_coord_t *)lv_event_get_user_data(e);
    lv_event_code_t code = lv_event_get_code(e);
    lv_obj_t *obj = (lv_obj_t *)lv_event_get_target(e);
    lv_100ask_sketchpad_t *_sketchpad_t = (lv_100ask_sketchpad_t *)_sketchpad;

    if (code == LV_EVENT_CLICKED)
    {
        if ((*toolbar_opt) == LV_100ASK_SKETCHPAD_TOOLBAR_OPT_DELETE)
        {
            Serial.println("Delete button clicked");
            lv_canvas_fill_bg(_sketchpad, lv_color_hex3(0xccc), LV_OPA_COVER);
        }
        else if ((*toolbar_opt) == LV_100ASK_SKETCHPAD_TOOLBAR_OPT_WIDTH)
        {
            Serial.println("Width button clicked");
            static lv_coord_t sketchpad_toolbar_width = LV_100ASK_SKETCHPAD_TOOLBAR_OPT_WIDTH;
            lv_obj_t *slider = lv_slider_create(lv_screen_active());
            lv_obj_set_width(slider, lv_pct(90));
            lv_slider_set_value(slider, (int32_t)(_sketchpad_t->line_rect_dsc.width), LV_ANIM_OFF);
            // lv_obj_align_to(slider, obj, LV_ALIGN_OUT_TOP_MID, 0, 0);
            lv_obj_align(slider, LV_ALIGN_CENTER, 0, 0);
            lv_obj_add_event_cb(slider, toolbar_set_event_cb, LV_EVENT_ALL, &sketchpad_toolbar_width);
            lv_obj_move_foreground(slider);
        }
    }
}
static void toolbar_set_event_cb(lv_event_t *e)
{
    lv_coord_t *toolbar_opt = (lv_coord_t *)lv_event_get_user_data(e);
    lv_event_code_t code = lv_event_get_code(e);
    lv_obj_t *obj = (lv_obj_t *)lv_event_get_target(e);
    lv_100ask_sketchpad_t *sketchpad = (lv_100ask_sketchpad_t *)_sketchpad;

    if (code == LV_EVENT_RELEASED)
    {
        if (*(toolbar_opt) == LV_100ASK_SKETCHPAD_TOOLBAR_OPT_WIDTH)
        {
            lv_obj_del(obj);
        }
    }
    else if (code == LV_EVENT_VALUE_CHANGED)
    {
        if ((*toolbar_opt) == LV_100ASK_SKETCHPAD_TOOLBAR_OPT_WIDTH)
        {
            sketchpad->line_rect_dsc.width = lv_slider_get_value(obj);
        }
    }
}

// Loop that runs on main loop of the Ardunio
void app_controller_run(void)
{
}