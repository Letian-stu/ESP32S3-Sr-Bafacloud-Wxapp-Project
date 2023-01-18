#include <stdio.h>
#include "string.h"
#include "lvgl/lvgl.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "gui_guider.h"
#include "gui_anim.h"


void setup_clock_screen(lv_ui *ui,uint32_t delay)
{
    ui->page = lv_obj_create(ui->bg);
    lv_obj_set_size(ui->page, 280, 240);
    lv_obj_align(ui->page, LV_ALIGN_CENTER, 0, 0);
    lv_obj_set_scrollbar_mode(ui->page, LV_SCROLLBAR_MODE_OFF);

    ui->clock_label = lv_label_create(ui->page);
    lv_obj_align(ui->clock_label, LV_ALIGN_CENTER, 0, 0);
    lv_label_set_text(ui->clock_label, "Clock");

    page_screen_anim(ui->page, -240, 0, 500, delay, (lv_anim_exec_xcb_t)lv_obj_set_y, lv_anim_path_bounce);
}
