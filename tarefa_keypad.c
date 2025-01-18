#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/timer.h"

#define ROWS 4
#define COLS 4

//define os pinos do teclado e dos leds
const uint col_pins[4] = {4, 3, 2, 1}; 
const uint row_pins[4] = {8, 7, 6, 5};
const uint rgb_1[3] = {28, 27, 26};
const uint rgb_2[3] = {22, 21, 20};
const uint rgb_3[3] = {19, 18, 17};


// Matrize de mapeamento de teclas
const char keys[ROWS][COLS] = {
    {'1', '2', '3', 'A'},
    {'4', '5', '6', 'B'},
    {'7', '8', '9', 'C'},
    {'*', '0', '#', 'D'}
};

//função que inicializa os pinos de um led RGB
void pico_rgb_init(const uint *rgb_pin)
{   
    
    for(int i = 0; i < 3; i++)
    {
        //_rgb_1[i] = rgb_pin[i];
        gpio_init(rgb_pin[i]);      //incializa os pinos
        gpio_set_dir(rgb_pin[i], GPIO_OUT);     //define cada um deles como saida
        gpio_put(rgb_pin[i], false);        //garante que todos serão desligados por padrão
    }
}

void pico_rgb_turn_off(const uint *rgb_pin)
{
    for(int i = 0; i < 3; i++)
    {
        gpio_put(rgb_pin[i], false);        //garante que todos serão desligados por padrão
    }
}

void pico_rgb_turn_on(const uint *rgb_pin)
{
    for(int i = 0; i < 3; i++)
    {
        gpio_put(rgb_pin[i], true);        //garante que todos serão desligados por padrão
    }
}

void pico_setup_keypad() {
    // Configura os pinos das linhas como saída e os pinos das colunas como entrada
    for (int i = 0; i < ROWS; i++) {
        gpio_init(row_pins[i]);
        gpio_set_dir(row_pins[i], GPIO_OUT);
        gpio_put(row_pins[i], 1); // Inicializa as linhas com nível alto
    }

    for (int i = 0; i < COLS; i++) {
        gpio_init(col_pins[i]);
        gpio_set_dir(col_pins[i], GPIO_IN);
        gpio_pull_up(col_pins[i]); // Ativa o pull-up nas colunas
    }
}

char pico_scan_keypad() {
    for (int r = 0; r < ROWS; r++) {
        gpio_put(row_pins[r], 0); // Ativa a linha (coloca em nível baixo)
        for (int c = 0; c < COLS; c++) {
            if (gpio_get(col_pins[c]) == 0) { // Verifica se o botão foi pressionado
                while (gpio_get(col_pins[c]) == 0) {
                    // Espera até que o botão seja liberado
                }
                return keys[r][c]; // Retorna a tecla pressionada
            }
        }
        gpio_put(row_pins[r], 1); // Desativa a linha (coloca em nível alto)
    }
    return '\0'; // Retorna null se nenhum botão for pressionado
}

void pico_rgb_control(char key) {
    switch (key) {
        case '1':
            gpio_put(rgb_1[0], !gpio_get(rgb_1[0]));    //se a tecla 1 for pressionada, alterna o estado do canal vermelho do led 1
            printf("Canal vermelho do led 1 alternado.\n");
            break;
        case '2':
            gpio_put(rgb_1[1], !gpio_get(rgb_1[1]));
            printf("Canal verde do led 1 alternado.\n"); //se a tecla 2 for pressionada, alterna o estado do canal verde do led 1
            break;
        case '3':
            gpio_put(rgb_1[2], !gpio_get(rgb_1[2]));    //se a tecla 3 for pressionada, alterna o estado do canal azul do led 1
            printf("Canal azul do led 1 alternado.\n");
            break;
        case 'A':
            pico_rgb_turn_off(rgb_1); //se a tecla pressionada for o A, desliga todo o led rgb
            printf("Led 1 desligado\n");
            break;
        default:
            break;
    }
}

//função principal
int main() {
    char key;
    stdio_init_all();
    pico_setup_keypad();
    pico_rgb_init(rgb_1);
    pico_rgb_init(rgb_2);
   // pico_rgb_init(rgb_3);

    
    while (true) {
        key = pico_scan_keypad(); // Verifica se há alguma tecla pressionada
        if (key != '\0') {
            pico_rgb_control(key); // Executa a ação correspondente
        }
        sleep_ms(100); // Pausa para evitar leitura repetida contínua
    }

    return 0;

}