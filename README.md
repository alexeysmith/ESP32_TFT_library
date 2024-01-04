
### TFT library for ESP32

---

**This library must be built with the latest esp-idf master branch and xtensa toolchain**

If you are using the esp-idf v5.1.2

---

#### Графическая библиотека для ST7789V и возможно других

Тестировалось на китайской версии экрана ST7789V, по сравнению с оригинальной библиотекой
изменена отрисовка текста для уменьшения мерцания.

---


#### How to build

Configure your esp32 build environment as for **esp-idf examples**

Clone the repository

`git clone https://github.com/alexeysmith/ESP32_TFT_library.git`

Конфигурация в меню ESP32 TFT Library

`idf.py menuconfig`

Необходимо задать номера PIN и расширение экрана.

ST7789V выводы:
1) GND - земля
2) VCC - +3.3V (можно соединить с соответствующим пином на плате ESP32)
3) SCL - CLK PIN, по-умолчанию 25-й вывод платы ESP32 (тактовый сигнал для дисплея)
4) SDA - MOSI PIN, по-умолчанию 12-й вывод платы ESP32 (DATA PIN, входные данные для дисплея)
5) RES - RESET PIN, по-умолчанию 26-й вывод платы ESP32 (Может быть использован для сброса)
6) DC  - DATA/Command Control PIN, по-умолчанию 27-й вывод
7) BLK - Backlight control pin (если подключен к земле, то подсветки нет, можно не подключать).


Время от включения микроконтроллера до передачи управления в app_main составляет 364мс
Если учитывать инициализацию дисплея, дальнейший код сможет выполняться через 627мс

Для подключения библиотеки в свою программу необходимо скопировать папку components/tft


---