#include "cam_task.h"

#define TAG "cam"

static camera_config_t camera_config = {
    .pin_pwdn = CAM_PIN_PWDN,
    .pin_reset = CAM_PIN_RESET,
    .pin_xclk = CAM_PIN_XCLK,
    .pin_sccb_sda = CAM_PIN_SIOD,
    .pin_sccb_scl = CAM_PIN_SIOC,

    .pin_d7 = CAM_PIN_D7,
    .pin_d6 = CAM_PIN_D6,
    .pin_d5 = CAM_PIN_D5,
    .pin_d4 = CAM_PIN_D4,
    .pin_d3 = CAM_PIN_D3,
    .pin_d2 = CAM_PIN_D2,
    .pin_d1 = CAM_PIN_D1,
    .pin_d0 = CAM_PIN_D0,
    .pin_vsync = CAM_PIN_VSYNC,
    .pin_href = CAM_PIN_HREF,
    .pin_pclk = CAM_PIN_PCLK,

    .xclk_freq_hz = 20000000,
    .ledc_timer = LEDC_TIMER_0,
    .ledc_channel = LEDC_CHANNEL_0,

    .pixel_format = PIXFORMAT_RGB565,
    .frame_size = FRAMESIZE_QVGA, // QQVGA-UXGA, For ESP32, do not use sizes above QVGA when not JPEG. The performance of the ESP32-S series has improved a lot, but JPEG mode always gives better frame rates.

    .jpeg_quality = 6, // 0-63, for OV series camera sensors, lower number means higher quality
    .fb_count = 1,                  // When jpeg mode is used, if fb_count more than one, the driver will work in continuous mode.
};

void cam_config_init(void)
{
    esp_err_t err = esp_camera_init(&camera_config);
    if (err != ESP_OK)
    {
        ESP_LOGE(TAG, "Camera Init Failed");
        return ESP_FAIL;
    }
    return ESP_OK;
}

lv_img_dsc_t img_dsc = {
    .header.always_zero = 0,
    .header.w = 320,
    .header.h = 240,
    .data_size = 320 * 240 * 2,
    .header.cf = LV_IMG_CF_TRUE_COLOR,
    .data = NULL,
};
camera_fb_t *pic;

void cam_show_task(void *p)
{
    while (1)
    {
        // static int64_t last_frame = 0;
        // if (!last_frame)
        // {
        //     last_frame = esp_timer_get_time();
        // }

        pic = esp_camera_fb_get();
        img_dsc.data = pic->buf;
        lv_img_set_src(guider_ui.img_cam, &img_dsc);
        esp_camera_fb_return(pic);


        // int64_t fr_end = esp_timer_get_time();
        // int64_t frame_time = fr_end - last_frame;
        // last_frame = fr_end;
        // frame_time /= 1000;
        // ESP_LOGI("cam", "MJPG:  %ums (%.1ffps)", (uint32_t)frame_time, 1000.0 / (uint32_t)frame_time);
    }
}
