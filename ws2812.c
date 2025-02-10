#include <stdio.h>
#include <stdlib.h>
#include "pico/stdlib.h" 
#include "hardware/pio.h"
#include "hardware/clocks.h"
#include "build/ws2812.pio.h"
#include "inc/ssd1306.h"


#define IS_RGBW false
#define NUM_PIXELS 25
#define WS2812_PIN 7

#define LED_R 13
#define LED_G 11
#define LED_B 12

// Número atual exibido na matriz de LEDs
volatile int num_atual; 
volatile bool atualizar_display = false; // Flag para atualizar display

// Números de 0 a 9 representados na matriz 5x5
bool numeros[10][NUM_PIXELS] = {
       { 
        0, 1, 1, 1, 0, // 0
        0, 1, 0, 1, 0,
        0, 1, 0, 1, 0,
        0, 1, 0, 1, 0,
        0, 1, 1, 1, 0
    },
    { 
        0, 1, 1, 1, 0, //1
        0, 0, 1, 0, 0,
        0, 0, 1, 0, 0,
        0, 1, 1, 0, 0,
        0, 0, 1, 0, 0
    },
    { 
        0, 1, 1, 1, 0, //2
        0, 1, 0, 0, 0,
        0, 1, 1, 1, 0,
        0, 0, 0, 1, 0,
        0, 1, 1, 1, 0
    },
    { 
        0, 1, 1, 1, 0, //3
        0, 0, 0, 1, 0,
        0, 1, 1, 1, 0,
        0, 0, 0, 1, 0,
        0, 1, 1, 1, 0
    },
    { 
        0, 1, 0, 0, 0, //4
        0, 0, 0, 1, 0,
        0, 1, 1, 1, 0,
        0, 1, 0, 1, 0,
        0, 1, 0, 1, 0
    },
    { 
        0, 1, 1, 1, 0, //5
        0, 0, 0, 1, 0,
        0, 1, 1, 1, 0,
        0, 1, 0, 0, 0,
        0, 1, 1, 1, 0
    },
    {
        0, 1, 1, 1, 0, //6
        0, 1, 0, 1, 0,
        0, 1, 1, 1, 0,
        0, 1, 0, 0, 0,
        0, 1, 1, 1, 0
    },
    { 
        0, 1, 0, 0, 0, //7
        0, 0, 0, 1, 0,
        0, 1, 0, 0, 0,
        0, 0, 0, 1, 0,
        0, 1, 1, 1, 0
    },
    { 
        0, 1, 1, 1, 0, //8
        0, 1, 0, 1, 0,
        0, 1, 1, 1, 0,
        0, 1, 0, 1, 0,
        0, 1, 1, 1, 0
    },
    {
        0, 1, 1, 1, 0, //9
        0, 0, 0, 1, 0,
        0, 1, 1, 1, 0,
        0, 1, 0, 1, 0,
        0, 1, 1, 1, 0
    }

};

// Acende o LED na cor especificada
static inline void put_pixel(uint32_t pixel_grb) {
    pio_sm_put_blocking(pio0, 0, pixel_grb << 8u);
}

// Converte valores RGB para um número de 32 bits
static inline uint32_t urgb_u32(uint8_t r, uint8_t g, uint8_t b) {
    return ((uint32_t)(r) << 8) | ((uint32_t)(g) << 16) | (uint32_t)(b);
}

// Exibe o número na matriz de LEDs
void exibir_numero(int numero) {
    if (numero < 0 || numero > 9) {
        printf("Número inválido! Deve ser entre 0 e 9.\n");
        return;
    }

    printf("Exibindo número: %d\n", numero);  // DEBUG

    for (int i = 0; i < NUM_PIXELS; i++) {
        if (numeros[numero][i]) {
            put_pixel(urgb_u32(25, 0, 25)); // Roxo
        } else {
            put_pixel(urgb_u32(0, 0, 0)); // Apagado
        }
    }
    sleep_ms(10); // Delay para garantir que os dados sejam enviados
}

// Inicializa o PIO para WS2812
static PIO pio = pio0;
static int sm = 0;
static uint offset;

void inicializar_ws2812() {
    offset = pio_add_program(pio, &ws2812_program);
    ws2812_program_init(pio, sm, offset, WS2812_PIN, 800000, IS_RGBW);
}