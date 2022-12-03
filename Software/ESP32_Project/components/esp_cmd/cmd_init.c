#include "cmd_init.h"

#define TAG "cmd"

// void Init_Cmd(void)
// {
//     esp_console_repl_t *repl = NULL;
//     esp_console_repl_config_t repl_config = ESP_CONSOLE_REPL_CONFIG_DEFAULT();
//     /* Prompt to be printed before each line.
//      * This can be customized, made dynamic, etc.
//      */
//     repl_config.prompt = CONFIG_IDF_TARGET ">";
//     repl_config.max_cmdline_length = 1024;
//     /* Register commands */
//     esp_console_register_help_command();
//     register_system();
//     register_wifi();
//     register_nvs();
//     register_cmd_ip();
//     esp_console_dev_uart_config_t hw_config = ESP_CONSOLE_DEV_UART_CONFIG_DEFAULT();
//     ESP_ERROR_CHECK(esp_console_new_repl_uart(&hw_config, &repl_config, &repl));
//     ESP_ERROR_CHECK(esp_console_start_repl(repl));
//     ESP_LOGI(TAG, "console register success");
// }





