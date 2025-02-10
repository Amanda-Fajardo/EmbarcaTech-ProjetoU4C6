#ifndef WS2812_H
#define WS2812_H

#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/pio.h"
#include "hardware/clocks.h"
#include "ws2812.pio.h"

#define IS_RGBW false
#define NUM_PIXELS 25
#define WS2812_PIN 7

void inicializar_ws2812();
void exibir_numero(int numero);

#endif
