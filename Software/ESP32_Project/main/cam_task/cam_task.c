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
    // .fb_location = CAMERA_FB_IN_PSRAM,

    .jpeg_quality = 32, // 0-63, for OV series camera sensors, lower number means higher quality
    .fb_count = 1,                  // When jpeg mode is used, if fb_count more than one, the driver will work in continuous mode.
};

lv_img_dsc_t img_dsc = {
    .header.always_zero = 0,
    .header.w = 320,
    .header.h = 240,
    .data_size = 320 * 240 * 2,
    .header.cf = LV_IMG_CF_TRUE_COLOR,
    .data = NULL,
};

camera_fb_t *pic;
cam_mode_t cam_mode;

esp_err_t cam_config_init(void)
{
    esp_err_t err = esp_camera_init(&camera_config);
    if (err != ESP_OK)
    {
        ESP_LOGE(TAG, "Camera Init Failed");
        return ESP_FAIL;
    }
    return ESP_OK;
}

/**
 * @description: 
 * @param {_Bool} Mode
 * @return {*}
 */
static esp_err_t cam_take_pic_config(cam_mode_t mode)
{
    // //err
    // sensor_t *camera_cfg = esp_camera_sensor_get();
    // if(mode == http_stream_mode)
    // {
    //     camera_cfg->set_pixformat(camera_cfg, PIXFORMAT_JPEG );
    //     camera_cfg->set_framesize(camera_cfg, FRAMESIZE_SVGA );
    // }
    // else if(mode == lvgl_show_mode)
    // {
    //     camera_cfg->set_pixformat(camera_cfg, PIXFORMAT_RGB565 );
    //     camera_cfg->set_framesize(camera_cfg, FRAMESIZE_QVGA );        
    // }
    // else if(mode == take_pic_mode)
    // {
    //     camera_cfg->set_pixformat(camera_cfg, PIXFORMAT_JPEG );
    //     camera_cfg->set_framesize(camera_cfg, FRAMESIZE_QVGA ); 
    // }
    esp_camera_deinit();
    if(mode == http_stream_mode)
    {
        camera_config.pixel_format = PIXFORMAT_JPEG;
        camera_config.frame_size = FRAMESIZE_QVGA;
    }
    else if(mode == lvgl_show_mode)
    {
        camera_config.pixel_format = PIXFORMAT_RGB565;
        camera_config.frame_size = FRAMESIZE_QVGA;
    }
    else if(mode == take_pic_mode)
    {
        camera_config.pixel_format = PIXFORMAT_JPEG;
        camera_config.frame_size = FRAMESIZE_QVGA;
    }
    cam_config_init();
    return ESP_OK;
}

void cam_show_task(void *p)
{
    BaseType_t err;
    static char file_name[32];
    static uint32_t picnum = 0;
    static size_t picwritelen = 0;
    ESP_LOGI(TAG, "cam task init");
    while (1)
    {
        pic = esp_camera_fb_get();

        img_dsc.data = pic->buf;
        lv_img_set_src(guider_ui.img_cam, &img_dsc);

        err = xSemaphoreTake(takepic_Handle, 0);
        if (err == pdTRUE)
        {
            esp_camera_fb_return(pic);

            cam_take_pic_config(take_pic_mode);

            pic = esp_camera_fb_get();
            // First create a file.
            sprintf(file_name, "S:/picture/num%dpic.jpg", picnum);
            ESP_LOGE(TAG, "create file %s", file_name);
            
            lv_fs_file_t img;
            err = lv_fs_open(&img, file_name, LV_FS_MODE_WR);
            if(err != LV_FS_RES_OK)
            {
                ESP_LOGE(TAG, "Failed to open file for writing");
            }
            else
            {
                lv_fs_write(&img, pic->buf, pic->len, &picwritelen);
                if (picwritelen != pic->len)
                {
                    ESP_LOGE(TAG, "img Write err");
                }
                else
                {
                    ESP_LOGI(TAG, "write buff len %d byte", picwritelen);
                    picnum++;
                }
                lv_fs_close(&img);
                ESP_LOGW(TAG, "img %s written",file_name);
            }
            esp_camera_fb_return(pic);

            // FILE *img = fopen(file_name, "w");
            // if (img == NULL)
            // {
            //     ESP_LOGE(TAG, "Failed to open file for writing");
            // }
            // else
            // {
            //     picwritelen = fwrite(pic->buf, sizeof(pic->buf), pic->len, img);
            //     if (picwritelen != pic->len)
            //     {
            //         ESP_LOGE(TAG, "img Write err");
            //     }
            //     else
            //     {
            //         ESP_LOGI(TAG, "write buff len %d byte", picwritelen);
            //         picnum++;
            //     }
            //     fclose(img);
            //     ESP_LOGI(TAG, "img written");
            // }


            cam_take_pic_config(lvgl_show_mode);
        }
        else
        {
            esp_camera_fb_return(pic);
        }

        // int64_t fr_end = esp_timer_get_time();
        // int64_t frame_time = fr_end - last_frame;
        // last_frame = fr_end;
        // frame_time /= 1000;
        // ESP_LOGI("cam", "MJPG:  %ums (%.1ffps)", (uint32_t)frame_time, 1000.0 / (uint32_t)frame_time);
    }
}
