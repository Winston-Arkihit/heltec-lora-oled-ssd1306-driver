#pragma once

#include <Arduino.h>

static constexpr int OLED_SCREEN_WIDTH = 128;
static constexpr int OLED_SCREEN_HEIGHT = 64;

bool oled_init(int sda_pin, int scl_pin, int rst_pin);
void oled_clear();
void oled_flush();

void oled_set_pixel(int x, int y, bool on = true);
void oled_draw_line(int x0, int y0, int x1, int y1, bool on = true);
void oled_draw_rect(int x, int y, int width, int height, bool on = true);
void oled_fill_rect(int x, int y, int width, int height, bool on = true);
void oled_draw_char(int x, int y, char c, bool on = true, int scale = 1);
void oled_draw_text(int x, int y, const char *text, bool on = true, int scale = 1);
