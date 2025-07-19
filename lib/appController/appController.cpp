
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

#define CANVAS_WIDTH 200
#define CANVAS_HEIGHT 300

static lv_obj_t *_sketchpad;
static uint8_t _drawline_flag = 0;
/**
 * Draw a line to the canvas
 */
// void lv_example_canvas_7(void)
// {
//     /*Create a buffer for the canvas*/
//     // LV_DRAW_BUF_DEFINE(draw_buf, CANVAS_WIDTH, CANVAS_HEIGHT, LV_COLOR_FORMAT_ARGB8888);
//     lv_draw_buf_t *draw_buf = lv_draw_buf_create(240, 320, LV_COLOR_FORMAT_ARGB8888, LV_STRIDE_AUTO);
//     /*Create a canvas and initialize its palette*/
//     lv_obj_t *canvas = lv_canvas_create(lv_screen_active());
//     lv_canvas_set_draw_buf(canvas, draw_buf);
//     lv_canvas_fill_bg(canvas, lv_color_hex3(0xccc), LV_OPA_COVER);
//     lv_obj_center(canvas);

//     lv_layer_t layer;
//     lv_canvas_init_layer(canvas, &layer);

//     lv_draw_line_dsc_t dsc;
//     lv_draw_line_dsc_init(&dsc);
//     dsc.color = lv_palette_main(LV_PALETTE_RED);
//     dsc.width = 4;
//     dsc.round_end = 1;
//     dsc.round_start = 1;
//     dsc.p1.x = 15;
//     dsc.p1.y = 15;
//     dsc.p2.x = 35;
//     dsc.p2.y = 10;
//     lv_draw_line(&layer, &dsc);

//     lv_canvas_finish_layer(canvas, &layer);
// }

/**********************
 * STATIC FUNCTIONS
 **********************/

static void event_handler(lv_event_t *e)
{
    // lv_obj_t *obj = (lv_obj_t *)lv_event_get_target(e);

    _drawline_flag = 1;
    LV_LOG_USER("LV_EVENT_VALUE_CHANGED");
}
/**********************
 *  LIB API
 **********************/
void app_controller_init(void)
{
    // LV_DRAW_BUF_DEFINE(draw_buf, 240, 320, LV_COLOR_FORMAT_ARGB8888);

    lv_draw_buf_t *draw_buf = lv_draw_buf_create(200, 200, LV_COLOR_FORMAT_ARGB8888, LV_STRIDE_AUTO);
    _sketchpad = lv_100ask_sketchpad_create(lv_screen_active());
    // lv_obj_set_scrollbar_mode(sketchpad, LV_SCROLLBAR_MODE_OFF); // See no different in PC, but may have some difference in the TFT display
    // lv_obj_set_scroll_snap_x(sketchpad, LV_SCROLL_SNAP_NONE);
    // lv_obj_set_scroll_snap_y(sketchpad, LV_SCROLL_SNAP_NONE);
    // lv_obj_clear_flag(sketchpad, LV_OBJ_FLAG_SCROLL_ELASTIC); // Disable elastic scrolling>>not work
    // lv_obj_clear_flag(sketchpad, LV_OBJ_FLAG_SCROLL_CHAIN); // THis work

    lv_canvas_set_draw_buf(_sketchpad, draw_buf);
    void* ptr3 = draw_buf->data;

    Serial.printf("Init: draw buffer address = %08x\r\n", ptr3);
    lv_canvas_fill_bg(_sketchpad, lv_color_hex3(0xccc), LV_OPA_COVER);
    lv_obj_center(_sketchpad);
    lv_obj_add_event_cb(_sketchpad, event_handler, LV_EVENT_VALUE_CHANGED, NULL);
    // lv_example_canvas_7();
}

// Loop that runs on main loop of the Ardunio
void app_controller_run(void)
{
    if (_drawline_flag)
    {
        lv_layer_t layer;
        lv_canvas_init_layer(_sketchpad, &layer);
        void* ptr3 =((lv_100ask_sketchpad_t *)_sketchpad)->draw_buf->data;

          Serial.printf("_drawline_flag: draw buffer address = %08x\r\n", ptr3);

        lv_draw_line_dsc_t dsc;
        lv_draw_line_dsc_init(&dsc);
        dsc.color = lv_palette_main(LV_PALETTE_RED);
        dsc.width = 4;
        dsc.round_end = 1;
        dsc.round_start = 1;
        dsc.p1.x = 15;
        dsc.p1.y = 15;
        dsc.p2.x = 35;
        dsc.p2.y = 10;
        lv_draw_line(&layer, &dsc);

        lv_canvas_finish_layer(_sketchpad, &layer);
        Serial.println("Drawn a line");
        _drawline_flag = 0;
    }
}