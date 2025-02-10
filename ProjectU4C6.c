#include <stdlib.h>
#include "pico/stdlib.h"
#include "hardware/i2c.h"
#include "inc/ssd1306.h"
#include "inc/font.h"
#include "ws2812.h"

// Pino GPIO para os LEDs RGB
#define LED_G 11 
#define LED_B 12

// Pinos GPIO para os botões A e B
#define Button_A 5
#define Button_B 6

// Configuração do I2C para o display SSD1306
#define I2C_PORT i2c1
#define I2C_SDA 14
#define I2C_SCL 15
#define endereco 0x3C

// Definição da UART
#define UART_ID uart0 // Seleciona a UART0
#define BAUD_RATE 115200 // Define a taxa de transmissão
#define UART_TX_PIN 0 // Pino GPIO usado para TX
#define UART_RX_PIN 1 // Pino GPIO usado para RX

// Variáveis para controle dos LEDs RGB
volatile bool status_led_g = false; 
volatile bool status_led_b = false;

// Variável global para o display SSD1306
ssd1306_t ssd;

// Função de interrupção para os botões A e B
void gpio_irq(uint gpio, uint32_t events) {
    static uint32_t last_time = 0;
    uint32_t current_time = to_ms_since_boot(get_absolute_time());

    // Debounce: ignora chamadas muito próximas no tempo
    if (current_time - last_time < 300) {
        return;
    }
    last_time = current_time;

    if (gpio == Button_A) {
        status_led_g = !status_led_g;
        gpio_put(LED_G, status_led_g);
        printf("Botão A pressionado. LED Verde: %s\n", status_led_g ? "Ligado" : "Desligado");

        // Exibe o estado do LED Verde no display
        ssd1306_fill(&ssd, false); // Limpa o display
        char estado_led[20];
        ssd1306_draw_string(&ssd, "Verde:", 0, 0);  
        ssd1306_draw_string(&ssd, status_led_g ? "ON" : "OFF", 0, 10); // Exibe o estado do LED Verde
        ssd1306_send_data(&ssd);

    } else if (gpio == Button_B) {
        status_led_b = !status_led_b;
        gpio_put(LED_B, status_led_b);
        printf("Botão B pressionado. LED Azul: %s\n", status_led_b ? "Ligado" : "Desligado");

        // Exibe o estado do LED Azul no display
        ssd1306_fill(&ssd, false); // Limpa o display
        char estado_led[20];
        snprintf(estado_led, sizeof(estado_led), "Azul %s", status_led_b ? "ON" : "OFF");
        ssd1306_draw_string(&ssd, estado_led, 0, 0); // Exibe o estado do LED Azul
        ssd1306_send_data(&ssd);
    }
}

void setup_uart() {
    // Inicializa a UART com a taxa de transmissão definida
    uart_init(UART_ID, BAUD_RATE);
    // Configura os pinos GPIO para TX e RX
    gpio_set_function(UART_TX_PIN, GPIO_FUNC_UART);
    gpio_set_function(UART_RX_PIN, GPIO_FUNC_UART);
}

void setup() {
    // Configuração dos pinos RGB
    gpio_init(LED_G);
    gpio_set_dir(LED_G, GPIO_OUT);
    gpio_init(LED_B);
    gpio_set_dir(LED_B, GPIO_OUT);

    // Configuração dos botões A e B
    gpio_init(Button_A);
    gpio_set_dir(Button_A, GPIO_IN);
    gpio_pull_up(Button_A);
    gpio_init(Button_B);
    gpio_set_dir(Button_B, GPIO_IN);
    gpio_pull_up(Button_B);
}

int main() {
    stdio_init_all();
    setup();
    setup_uart();
    inicializar_ws2812();
    exibir_numero(0);

    // Configuração da interrupção para os botões A e B
    gpio_set_irq_enabled_with_callback(Button_A, GPIO_IRQ_EDGE_RISE, true, &gpio_irq);
    gpio_set_irq_enabled_with_callback(Button_B, GPIO_IRQ_EDGE_RISE, true, &gpio_irq);

    // Inicialização do I2C para o display SSD1306
    i2c_init(I2C_PORT, 400 * 1000);
    gpio_set_function(I2C_SDA, GPIO_FUNC_I2C);
    gpio_set_function(I2C_SCL, GPIO_FUNC_I2C);
    gpio_pull_up(I2C_SDA);
    gpio_pull_up(I2C_SCL);

    // Inicialização do display SSD1306
    ssd1306_init(&ssd, 128, 32, false, endereco, I2C_PORT);
    ssd1306_fill(&ssd, false); // Limpa o display
    ssd1306_send_data(&ssd);

    while (true) {
        if (stdio_usb_connected()) { // Confirma que o USB está conectado
            char c;
            if (scanf("%c", &c) == 1) { // Lê caractere da entrada padrão
                printf("Recebido: '%c'\n", c);

                // Exibe o caractere no display
                ssd1306_fill(&ssd, false); // Limpa o display
                char str[2] = {c, '\0'}; // Converte o caractere para uma string
                ssd1306_draw_string(&ssd, str, 0, 0); // Exibe o caractere no display
                ssd1306_send_data(&ssd);

                if (c >= '0' && c <= '9') {
                  int numero = c - '0'; // Converte char para número inteiro
                  if (numero <= 9 && numero >= 0) {
                      exibir_numero(numero); // Exibe o número na matriz de LEDs
                  }
              } else if ((c >= 'A' && c <= 'Z') || (c >= 'a' && c <= 'z')) {
                  // Letras são exibidas no display, mas não há ação na matriz de LEDs
              } else {
                ssd1306_draw_string(&ssd, "ERRO", 8, 10); // Exibe "ERRO" no display 
                ssd1306_send_data(&ssd);
                printf("Entrada inválida!\n"); // Exibe mensagem de erro no serial monitor
              }
              
            }
        }
        sleep_ms(40);
    }
}