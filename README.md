
### TFT library for ESP32

---

**This library must be built with the latest esp-idf master branch and xtensa toolchain**

If you are using the esp-idf v5.0

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

---