# OLED Driver

Small raw SSD1306-compatible OLED driver for the built-in 128x64 display on Heltec WiFi LoRa 32 V3.

The driver does not depend on Adafruit GFX/SSD1306. It keeps a 1024-byte framebuffer in RAM, draws into it, then sends the framebuffer to the OLED over I2C.

Files:

```text
src/oled_driver.h
src/oled_driver.cpp
```

## Basic Usage

Include the driver:

```cpp
#include "oled_driver.h"
```

Initialize once in `setup()`:

```cpp
if (!oled_init(17, 18, 21)) {
    Serial.println("OLED init failed");
    for (;;);
}
```

Draw a frame:

```cpp
oled_clear();
oled_draw_rect(0, 0, OLED_SCREEN_WIDTH, OLED_SCREEN_HEIGHT);
oled_draw_text(5, 5, "HELLO");
oled_draw_char(40, 30, '*');
oled_flush();
```

Important: drawing functions only change the RAM buffer. The image appears on the OLED only after `oled_flush()`.

## Screen

```cpp
OLED_SCREEN_WIDTH  // 128
OLED_SCREEN_HEIGHT // 64
```

Coordinates start at the top-left corner:

```text
(0, 0)                 (127, 0)
  +-----------------------+
  |                       |
  |                       |
  +-----------------------+
(0, 63)                (127, 63)
```

Out-of-bounds pixels are ignored.

## API

### `oled_init`

```cpp
bool oled_init(int sda_pin, int scl_pin, int rst_pin);
```

Starts I2C, resets the OLED, and sends SSD1306 init commands.

For Heltec WiFi LoRa 32 V3:

```cpp
oled_init(17, 18, 21);
```

Returns:

- `true`: OLED accepted init commands
- `false`: OLED did not respond

### `oled_clear`

```cpp
void oled_clear();
```

Clears the RAM framebuffer.

This does not immediately clear the OLED. Call `oled_flush()` afterward:

```cpp
oled_clear();
oled_flush();
```

### `oled_flush`

```cpp
void oled_flush();
```

Sends the whole framebuffer to the OLED.

The framebuffer is 1024 bytes:

```text
128 * 64 / 8 = 1024
```

Usually call it once per frame, after all drawing is done.

### `oled_set_pixel`

```cpp
void oled_set_pixel(int x, int y, bool on = true);
```

Turns one pixel on or off.

Examples:

```cpp
oled_set_pixel(10, 20);        // on
oled_set_pixel(10, 20, false); // off
```

### `oled_draw_line`

```cpp
void oled_draw_line(int x0, int y0, int x1, int y1, bool on = true);
```

Draws a line using Bresenham-style integer rasterization.

Examples:

```cpp
oled_draw_line(0, 0, 127, 63);
oled_draw_line(0, 63, 127, 0);
oled_draw_line(0, 32, 127, 32);
```

### `oled_draw_rect`

```cpp
void oled_draw_rect(int x, int y, int width, int height, bool on = true);
```

Draws only the rectangle outline.

Examples:

```cpp
oled_draw_rect(0, 0, 128, 64);
oled_draw_rect(10, 10, 30, 20);
```

### `oled_fill_rect`

```cpp
void oled_fill_rect(int x, int y, int width, int height, bool on = true);
```

Draws a filled rectangle.

Examples:

```cpp
oled_fill_rect(10, 10, 4, 4);
oled_fill_rect(20, 20, 30, 12);
```

Useful for sprites, snake body segments, buttons, and progress bars.

### `oled_draw_char`

```cpp
void oled_draw_char(int x, int y, char c, bool on = true, int scale = 1);
```

Draws one ASCII character using a built-in 5x7 bitmap font.

Supported range:

```text
ASCII 32..127
```

Examples:

```cpp
oled_draw_char(10, 10, '*');
oled_draw_char(20, 10, 'A');
oled_draw_char(30, 10, '@');
```

Scaled character:

```cpp
oled_draw_char(10, 10, '*', true, 2);
```

With `scale = 2`, each font pixel becomes a 2x2 block.

### `oled_draw_text`

```cpp
void oled_draw_text(int x, int y, const char *text, bool on = true, int scale = 1);
```

Draws a null-terminated ASCII string.

Examples:

```cpp
oled_draw_text(5, 5, "ASCII OK");
oled_draw_text(5, 20, "SCORE: 10");
oled_draw_text(5, 35, "BIG", true, 2);
```

Newlines are supported:

```cpp
oled_draw_text(0, 0, "LINE 1\nLINE 2");
```

## Framebuffer Model

The OLED memory is page-based. Each byte stores one vertical column of 8 pixels.

Pixel `(x, y)` maps to:

```cpp
index = x + (y / 8) * OLED_SCREEN_WIDTH;
mask  = 1 << (y % 8);
```

This is why the framebuffer size is:

```cpp
128 columns * 8 pages = 1024 bytes
```

## Recommended Frame Pattern

Good:

```cpp
oled_clear();
oled_draw_rect(0, 0, OLED_SCREEN_WIDTH, OLED_SCREEN_HEIGHT);
oled_draw_text(5, 5, "SNAKE");
oled_draw_char(player.x, player.y, '*');
oled_flush();
```

Avoid flushing after every primitive:

```cpp
oled_draw_text(5, 5, "SNAKE");
oled_flush();
oled_draw_char(player.x, player.y, '*');
oled_flush();
```

That works, but it is slower and can flicker more.

## Current Limitations

- Monochrome only.
- Full-screen flush only, no partial update API yet.
- Built-in font is tiny 5x7 ASCII only.
- No clipping for whole primitives, only pixel-level bounds checks.
- No text wrapping.

## Possible Future Additions

- `oled_draw_circle`
- `oled_draw_bitmap`
- partial flush by rectangle/page
- inverse text mode
- sprite helper functions
- simple UI widgets
