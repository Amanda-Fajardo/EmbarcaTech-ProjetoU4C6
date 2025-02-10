# EmbarcaTech-ProjetoU4C6

Este projeto demonstra como controlar LEDs RGB, exibir informações em um display SSD1306 e manipular uma matriz de LEDs WS2812 utilizando um Raspberry Pi Pico. O projeto inclui a configuração de botões para controlar os LEDs, a recepção de caracteres via UART e a exibição de símbolos na matriz de LEDs.

## Descrição do Projeto

O código principal (`ProjectU4C6`) realiza as seguintes tarefas:

1. **Controle de LEDs RGB**: 
   - Dois LEDs (Verde e Azul) são controlados por botões (A e B). Quando um botão é pressionado, o estado do LED correspondente é alternado e o novo estado é exibido no display SSD1306.

2. **Display SSD1306**:
   - O display é utilizado para mostrar o estado dos LEDs (Ligado/Desligado), caracteres recebidos via UART e mensagens de erro caso uma entrada inválida seja detectada.

3. **Comunicação UART**:
   - O projeto inclui a configuração de uma UART para receber caracteres via comunicação serial. Os caracteres recebidos são exibidos no display SSD1306.

4. **Matriz de LEDs WS2812:**
    - O projeto também inclui a exibição de números na matriz de LEDs WS2812 quando um número é recebido via UART.

5. **Interrupções de GPIO**:
   - As interrupções são utilizadas para detectar o pressionamento dos botões e alternar o estado dos LEDs.

## Como usar

1. **Clone o repositório:**
```
git clone <https://github.com/Amanda-Fajardo/EmbarcaTech-ProjetoU4C6>
```

2. Tenha o VSCode instalado e configurado.
   
3. Tenha a extensão do Raspberry Pi Pico instalada e configurada.
   
4. Compile o programa utilizando o CMake:

```
mkdir build
cd build
cmake ..
make
```
5. Carregue o firmware no seu microcontrolador Raspberry Pi Pico.

6. **Utilização:**
    - Pressione os botões A e B para alternar o estado dos LEDs Verde e Azul, respectivamente.
    - Envie caracteres via UART para exibi-los no display SSD1306. Números serão exibidos tanto na matriz de LEDs WS2812 quanto no display SSD1306.
    - Se um caractere inválido for recebido, uma mensagem de erro será exibida no display SSD1306.

## Vídeo de demonstração
``https://drive.google.com/drive/folders/1YytSCn2ZlWfwDoi0pivXSpDU8uNLYbwV?usp=sharing``
