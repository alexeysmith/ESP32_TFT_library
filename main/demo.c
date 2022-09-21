#include <stdio.h>
#include "sdkconfig.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_system.h"
#include "esp_spi_flash.h"

// i2c
#include "driver/i2c.h"

#include "driver/gpio.h"
#include "esp_heap_caps.h"


#include "tftspi.h"
#include "tft.h"

// ==========================================================
// Define which spi bus to use TFT_VSPI_HOST or TFT_HSPI_HOST
#define SPI_BUS TFT_HSPI_HOST
// ==========================================================


uint32_t _lineHeight = 32; // полная ширина линии
uint32_t _lineFontOffsetY = 2; // смещение от верха линии
uint32_t _lineFontOffsetX = 2; // смещение первого символа строки слева
void writeLine(int n, char * text)
{
    //TFT_fillRect(0,n * _lineHeight, 240,_lineHeight,_bg);
    TFT_print(text, _lineFontOffsetX, _lineFontOffsetY + n * _lineHeight);
}


// Pins used: miso=32, mosi=12, sck=25, cs=33


void app_main(void)
{
    esp_err_t ret;
    // ===================================================
    // ==== Set display type                         =====
    tft_disp_type = DISP_TYPE_ST7789V;

    // ===================================================
	// === Set display resolution if NOT using default ===
	_width = 240;  // smaller dimension
	_height = 240; // DEFAULT_TFT_DISPLAY_HEIGHT; // larger dimension

    // ===================================================
	// ==== Set maximum spi clock for display read    ====
	//      operations, function 'find_rd_speed()'    ====
	//      can be used after display initialization  ====
	max_rdclock = 1000000; /// 8 mil default!!!
	// ===================================================

    // ====================================================================
    // === Pins MUST be initialized before SPI interface initialization ===
    // ====================================================================
    TFT_PinsInit();

    // ====  CONFIGURE SPI DEVICES(s)  ====================================================================================

    spi_lobo_device_handle_t spi;
	
    spi_lobo_bus_config_t buscfg={
        .miso_io_num=PIN_NUM_MISO,				// set SPI MISO pin
        .mosi_io_num=PIN_NUM_MOSI,				// set SPI MOSI pin
        .sclk_io_num=PIN_NUM_CLK,				// set SPI CLK pin
        .quadwp_io_num=-1,
        .quadhd_io_num=-1,
		.max_transfer_sz = 6*1024,
    };
    spi_lobo_device_interface_config_t devcfg={
        .clock_speed_hz=1000000,                // Initial clock out at 8 MHz
        .mode=2,                                // SPI mode 0 (2??)
        .spics_io_num=-1,                       // we will use external CS pin
		.spics_ext_io_num=PIN_NUM_CS,           // external CS pin
		.flags=LB_SPI_DEVICE_HALFDUPLEX,        // ALWAYS SET  to HALF DUPLEX MODE!! for display spi
    };

    vTaskDelay(500 / portTICK_RATE_MS);
	printf("\r\n==============================\r\n");
    printf("TFT display DEMO, LoBo 11/2017\r\n");
	printf("==============================\r\n");
    printf("Pins used: miso=%d, mosi=%d, sck=%d, cs=%d\r\n", PIN_NUM_MISO, PIN_NUM_MOSI, PIN_NUM_CLK, PIN_NUM_CS);
#if USE_TOUCH > TOUCH_TYPE_NONE
    printf(" Touch CS: %d\r\n", PIN_NUM_TCS);
#endif
	printf("==============================\r\n\r\n");


    // ==================================================================
	// ==== Initialize the SPI bus and attach the LCD to the SPI bus ====
    ret=spi_lobo_bus_add_device(SPI_BUS, &buscfg, &devcfg, &spi);
    assert(ret==ESP_OK);
	printf("SPI: display device added to spi bus (%d)\r\n", SPI_BUS);
	disp_spi = spi;

	// ==== Test select/deselect ====
	ret = spi_lobo_device_select(spi, 1);
    assert(ret==ESP_OK);
	ret = spi_lobo_device_deselect(spi);
    assert(ret==ESP_OK);

	printf("SPI: attached display device, speed=%u\r\n", spi_lobo_get_speed(spi));
	printf("SPI: bus uses native pins: %s\r\n", spi_lobo_uses_native_pins(spi) ? "true" : "false");

    // ================================
	// ==== Initialize the Display ====

	printf("SPI: display init...\r\n");
	TFT_display_init();
    printf("OK\r\n");

    // ---- Detect maximum read speed ----
	max_rdclock = find_rd_speed();
	printf("SPI: Max rd speed = %u\r\n", max_rdclock);

    // ==== Set SPI clock used for display operations ====
	spi_lobo_set_speed(spi, DEFAULT_SPI_CLOCK);
	printf("SPI: Changed speed to %u\r\n", spi_lobo_get_speed(spi));

    printf("\r\n---------------------\r\n");
	printf("Graphics demo started\r\n");
	printf("---------------------\r\n");


    font_rotate = 0;
	text_wrap = 0;
	font_transparent = 0; // Если надо печатать шрифт без background-color
	font_forceFixed = 0; // ??
	gray_scale = 0;
    TFT_setGammaCurve(DEFAULT_GAMMA_CURVE);
	TFT_setRotation(PORTRAIT_FLIP); // PORTRAIT - зеркально отображает
	TFT_setFont(DEFAULT_FONT, NULL);
	TFT_resetclipwin();

    //TFT_setRotation(0);
    TFT_setFont(DEJAVU24_FONT, NULL);

    // Fill black, red text
    //uint8_t imode = 0;
    TFT_invertDisplay(1);
    uint8_t disp_rot = PORTRAIT_FLIP;
    TFT_fillScreen(TFT_BLACK);
    char buf[100];
    int index = 0;
    while(1) {
        //TFT_fillScreen(TFT_BLUE);
        _fg = TFT_GREEN;
        _bg = TFT_BLACK;
        sprintf(buf, "Index: %d  ", index);
        writeLine(0, buf);
        writeLine(1, "Second string");
        sprintf(buf, "Third %d string             ", index);
        writeLine(2, buf);
        writeLine(3, "Four string");
        writeLine(4, "444444444444444444444");
        writeLine(5, "555555555555555555");
        writeLine(6, "66666666666666666");
        writeLine(7, "77777777777777777");
        writeLine(8, "88888888888888888");
        vTaskDelay(100 / portTICK_PERIOD_MS);
        index++;
        if (index > 30)index=0;
    }






    printf("Hello world!\n");


    /* Print chip information */
    esp_chip_info_t chip_info;
    esp_chip_info(&chip_info);
    printf("This is %s chip with %d CPU core(s), WiFi%s%s, ",
            CONFIG_IDF_TARGET,
            chip_info.cores,
            (chip_info.features & CHIP_FEATURE_BT) ? "/BT" : "",
            (chip_info.features & CHIP_FEATURE_BLE) ? "/BLE" : "");
    printf("silicon revision %d, ", chip_info.revision);

    printf("%dMB %s flash\n", spi_flash_get_chip_size() / (1024 * 1024),
            (chip_info.features & CHIP_FEATURE_EMB_FLASH) ? "embedded" : "external");

    printf("Minimum free heap size: %d bytes\n", esp_get_minimum_free_heap_size());

    for (int i = 1000; i >= 0; i--) {
        printf("Restarting in %d seconds...\n", i);
        vTaskDelay(5000 / portTICK_PERIOD_MS);
        disp_rot++;
        disp_rot &= 3;
        //TFT_setRotation(disp_rot);
        TFT_print("ABCDEF GH", CENTER, CENTER);
    }
    printf("Restarting now.\n");
    fflush(stdout);
    esp_restart();
}
