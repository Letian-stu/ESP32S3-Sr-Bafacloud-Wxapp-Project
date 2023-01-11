#ifndef GUI_ANIM_H
#define GUI_ANIM_H

#include "lvgl/lvgl.h"

void obj_set_anim(lv_obj_t *obj, int32_t start, int32_t end, uint32_t time, uint32_t delay, lv_anim_exec_xcb_t exec_cb, lv_anim_ready_cb_t ready_cb, lv_anim_path_cb_t path_cb);

#endif

