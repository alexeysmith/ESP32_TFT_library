#include <stdio.h>
#include "sdkconfig.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_system.h"
#include "esp_spi_flash.h"

#include "esp_log.h"

// i2c
#include "driver/i2c.h"

#include "driver/gpio.h"
#include "esp_heap_caps.h"

#include "tftspi.h"
#include "tft.h"









void init_tft()
{
    // 1) Основные настройки [312ms]
    max_rdclock = 8000000;      // 8 млн, которое задано по умолчанию - иногда не работает
    gray_scale = 0;             // Если задано 1, то всё будет в оттенках серого
    font_rotate = 0;
	text_wrap = 0;
	font_transparent = 0; // Если надо печатать шрифт без background-color
	font_forceFixed = 0; // ??

    // 2) Размеры и тип экрана, цвета по-умолчанию
    tft_disp_type = DISP_TYPE_ST7789V;
    _width = 240;               // ширина
	_height = 240;              // высота
    _fg = TFT_GREEN;            // Цвет текста
    _bg = TFT_BLACK;            // Цвет фона при печати текста (пока не может быть transparent)

    // 3) Инициализация pins [+1ms]
    TFT_PinsInit();
    // 4) Инициализация spi и экрана
    TFT_spi_init(TFT_HSPI_HOST);
    // 5) Настройки по-умолчанию
	TFT_setGammaCurve(DEFAULT_GAMMA_CURVE);
    TFT_resetclipwin();
    TFT_setFont(DEJAVU24_FONT, NULL);
    TFT_fillScreen(_bg);
}



void app_main(void)
{
    

    // Инициализация
    init_tft();

    char buf[100];
    int index = 0;
    int x = 0;
    int y = 0;
    int initMs = (int)esp_log_timestamp();
    while(1) {
        // 1) Screen 01 - text
        TFT_fillScreen(_bg);
        sprintf(buf, "Init end, %dms", initMs);
        TFT_WriteLine(1, buf);
        TFT_WriteLine(0, "Screen 01");
        index = 0;
        while(1) {
            _fg = TFT_GREEN;
            _bg = TFT_BLACK;
            sprintf(buf, "Index: %d  ", index);
            TFT_WriteLine(2, buf);
            TFT_WriteLine(3, "4th string");
            TFT_WriteLine(4, "Very very long long string string string");
            TFT_WriteLine(5, "----------------------------------");
            TFT_WriteLine(6, "77777777777777777777777777777");
            TFT_WriteLine(7, "8888888888888888888888888888");
            vTaskDelay(10 / portTICK_PERIOD_MS);
            index++;
            if (index > 25) {
                break;
            }
        }
        // 2) Screen 02 - move text
        TFT_fillScreen(_bg);
        TFT_WriteLine(0, "Screen 02 - move text");
        index = 0;
        while(1) {
            TFT_print("TexT 1 (No Clear)", index + 2, index + 30);
            if (index > 0) {
                TFT_print("      ", index - 1 + 2, index - 1 + 70);
            }
            TFT_print("TexT 2 (Clear)", index + 2, index + 70);
            vTaskDelay(10 / portTICK_PERIOD_MS);
            index++;
            if (index > 90) {
                break;
            }
        }
        // 3) Screen 03 - pixels
        TFT_fillScreen(_bg);
        TFT_WriteLine(0, "Screen 03 - pixels");
        index = 0;
        while(1) {
            x = rand() % 240;
            y = rand() % 240;
            TFT_drawPixel(x, y, _fg, 1);
            vTaskDelay(1 / portTICK_PERIOD_MS);
            index++;
            if (index > 3000) {
                break;
            }
        }
        // 4) Screen 04 - lines
        TFT_fillScreen(_bg);
        TFT_WriteLine(0, "Screen 04 - lines");
        index = 0;
        while(1) {
            vTaskDelay(4 / portTICK_PERIOD_MS);
            TFT_drawLine(50,50 + index,190,50 + 190 - index, _fg);
            TFT_drawFastHLine(10,50 + index,30, TFT_RED);
            index++;
            if (index > 190) {
                break;
            }
        }
        // 5) Screen 05 - rect
        TFT_fillScreen(_bg);
        TFT_WriteLine(0, "Screen 05 - rect");
        index = 0;
        while(1) {
            TFT_drawRect(40,40,150,40,TFT_BLUE);
            TFT_fillRect(40,100,150,40, TFT_CYAN);
            TFT_drawRoundRect(40,160,150,40,8,TFT_BLUE);
            vTaskDelay(10 / portTICK_PERIOD_MS);
            index++;
            if (index > 40) {
                break;
            }
        }
        // 5) Screen 06 - rect
        TFT_fillScreen(_bg);
        TFT_WriteLine(0, "Screen 06 - circles");
        index = 0;
        while(1) {
            TFT_drawCircle(70,80,40,TFT_RED);
            TFT_fillCircle(180,80,40,TFT_RED);
            TFT_fillEllipse(120, 180, 100, 40, TFT_BLUE, TFT_ELLIPSE_UPPER_LEFT | TFT_ELLIPSE_LOWER_RIGHT);
            vTaskDelay(10 / portTICK_PERIOD_MS);
            index++;
            if (index > 40) {
                break;
            }
        }
    }


}
