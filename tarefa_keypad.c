#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/timer.h"
#include "hardware/pwm.h"
#include "hardware/clocks.h"


#define ROWS 4
#define COLS 4
#define NOTE_DO1  261
#define NOTE_RE  294
#define NOTE_MI  329
#define NOTE_FA  349
#define NOTE_SOL  392
#define NOTE_LA  440
#define NOTE_SI  493
#define NOTE_DO2  523


const uint col_pins[4] = {4, 3, 2, 1}; 
const uint row_pins[4] = {8, 7, 6, 5};
const uint rgb_1[3] = {28, 27, 26};
const uint rgb_2[3] = {22, 21, 20};
const uint rgb_3[3] = {19, 18, 17};
const uint buzzer_pin = 14; // GPIO do buzzer



// Matrize de mapeamento de teclas
const char keys[ROWS][COLS] = {
    {'1', '2', '3', 'A'},
    {'4', '5', '6', 'B'},
    {'7', '8', '9', 'C'},
    {'*', '0', '#', 'D'}
};

//função para inicializar o buzzer
void pico_buzzer_init(uint gpio) {
    //gpio_init(gpio);
    //gpio_set_dir(gpio, GPIO_OUT);
    gpio_set_function(gpio, GPIO_FUNC_PWM);
    uint slice_num = pwm_gpio_to_slice_num(gpio);
    pwm_set_enabled(slice_num, true);
}

//função para tocar uma nota no buzzer
void pico_buzzer_play(uint gpio, uint frequency) {
    uint slice_num = pwm_gpio_to_slice_num(gpio);
    //uint32_t clock = clock_get_hz(clk_sys);
    //uint32_t wrap = clock / frequency - 1;
    uint32_t clock = 125000000; 
    uint32_t divider = clock / (frequency * 4096); 
    uint32_t wrap = (clock / divider) / frequency - 1;
    uint32_t level = wrap / 2; 
    //pwm_set_wrap(slice_num, clock_get_hz(clk_sys) / frequency - 1);
    //pwm_set_chan_level(slice_num, PWM_CHAN_A, (clock_get_hz(clk_sys) / frequency) / 2);
    pwm_set_clkdiv(slice_num, divider);
    pwm_set_wrap(slice_num, wrap);
    pwm_set_chan_level(slice_num, PWM_CHAN_A, level);
    pwm_set_enabled(slice_num, true);
    /*uint32_t period_us = 1000000 / frequency; // Período em microssegundos
    uint32_t half_period_us = period_us / 2; // Meio período para alternar o estado do pino
    uint32_t end_time = to_ms_since_boot(get_absolute_time()) + 250;

    while (to_ms_since_boot(get_absolute_time()) < end_time) {
        gpio_put(gpio, 1);
        sleep_us(half_period_us);
        gpio_put(gpio, 0);
        sleep_us(half_period_us);
    }*/
}

//função para parar o buzzer
void pico_buzzer_stop(uint gpio) {
    uint slice_num = pwm_gpio_to_slice_num(gpio);
    pwm_set_chan_level(slice_num, PWM_CHAN_A, 0);
    pwm_set_enabled(slice_num, false);
    //gpio_put(gpio, 0);
}

//função para tocar a melodia "Marcha Soldado"
void play_marcha_soldado(uint gpio) {
    int melody[] = {
        NOTE_DO1, NOTE_RE, NOTE_MI, NOTE_DO1, NOTE_DO1, NOTE_RE, NOTE_MI, NOTE_DO1, NOTE_MI, NOTE_FA, NOTE_SOL,
        NOTE_MI, NOTE_FA, NOTE_SOL, NOTE_MI, NOTE_SOL, NOTE_LA, NOTE_SOL, NOTE_LA, NOTE_SI, NOTE_DO2,
        NOTE_SOL, NOTE_LA, NOTE_SI, NOTE_SOL, NOTE_SI, NOTE_DO2, NOTE_SI, NOTE_DO2, NOTE_RE, NOTE_MI,
        NOTE_DO1, NOTE_DO1, NOTE_RE, NOTE_MI, NOTE_DO1, NOTE_DO1, NOTE_RE, NOTE_MI, NOTE_DO1
    };
    int noteDurations[] = {
        4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4,
        4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4,
        4, 4, 4, 4, 4, 4, 4, 4, 4, 4,
        4, 4, 4, 4, 4, 4, 4, 4, 4, 4
    };
    int length = sizeof(melody) /sizeof(melody)[0];
    for (int i = 0; i < length; i++) {
        int noteDuration = 1000 / noteDurations[i];
        pico_buzzer_play(gpio, melody[i]);
        sleep_ms(noteDuration);
        pico_buzzer_stop(gpio);
        sleep_ms(noteDuration * 0.3);
    }
}

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


void pico_keypad_control_led(char key) {
    switch (key) {
        case '1':
            gpio_put(rgb_1[0], !gpio_get(rgb_1[0])); // Alterna o estado do canal vermelho do LED 1
            printf("Canal vermelho do led 1 alternado.\n");
            break;
        case '2':
            gpio_put(rgb_1[1], !gpio_get(rgb_1[1])); // Alterna o estado do canal verde do LED 1
            printf("Canal verde do led 1 alternado.\n");
            break;
        case '3':
            gpio_put(rgb_1[2], !gpio_get(rgb_1[2])); // Alterna o estado do canal azul do LED 1
            printf("Canal azul do led 1 alternado.\n");
            break;
        case 'A':
            pico_rgb_turn_off(rgb_1); // Desliga o LED 1
            printf("Led 1 desligado\n");
            break;
        case '4':
            gpio_put(rgb_2[0], !gpio_get(rgb_2[0])); // Alterna o estado do canal vermelho do LED 2
            printf("Canal vermelho do led 2 alternado.\n");
            break;
        case '5':
            gpio_put(rgb_2[1], !gpio_get(rgb_2[1])); // Alterna o estado do canal verde do LED 2
            printf("Canal verde do led 2 alternado.\n");
            break;
        case '6':
            gpio_put(rgb_2[2], !gpio_get(rgb_2[2])); // Alterna o estado do canal azul do LED 2
            printf("Canal azul do led 2 alternado.\n");
            break;
        case 'B':
            pico_rgb_turn_off(rgb_2); // Desliga o LED 2
            printf("Led 2 desligado\n");
            break;
        case '7':
            gpio_put(rgb_3[0], !gpio_get(rgb_3[0])); // Alterna o estado do canal vermelho do LED 3
            printf("Canal vermelho do led 3 alternado.\n");
            break;
        case '8':
            gpio_put(rgb_3[1], !gpio_get(rgb_3[1])); // Alterna o estado do canal verde do LED 3
            printf("Canal verde do led 3 alternado.\n");
            break;
        case '9':
            gpio_put(rgb_3[2], !gpio_get(rgb_3[2])); // Alterna o estado do canal azul do LED 3
            printf("Canal azul do led 3 alternado.\n");
            break;
        case 'C':
            pico_rgb_turn_off(rgb_3); // Desliga o LED 3
            printf("Led 3 desligado\n");
            break;
        case '0': // Desliga todos os LEDS de uma vez
            pico_rgb_turn_off(rgb_1);
            pico_rgb_turn_off(rgb_2);
            pico_rgb_turn_off(rgb_3);
            printf("Todos os LEDS desligados");
            break;
        default:
            printf("Tecla '%c' não mapeada.\n", key);
            break;
    }
}

//função para controlar o teclado no modo de reprodução de música
void pico_keypad_control_music(char key) {
    switch (key) {
        case '1': 
            pico_buzzer_play(buzzer_pin, NOTE_DO1);
            sleep_ms(200);
            pico_buzzer_stop(buzzer_pin);
            break;
        case '2': 
            pico_buzzer_play(buzzer_pin, NOTE_RE); 
            sleep_ms(200);
            pico_buzzer_stop(buzzer_pin);
            break;
        case '3': 
            pico_buzzer_play(buzzer_pin, NOTE_MI); 
            sleep_ms(200);
            pico_buzzer_stop(buzzer_pin);
            break;
        case '4': 
            pico_buzzer_play(buzzer_pin, NOTE_FA); 
            sleep_ms(200);
            pico_buzzer_stop(buzzer_pin);
            break;
        case '5': 
            pico_buzzer_play(buzzer_pin, NOTE_SOL); 
            sleep_ms(200);
            pico_buzzer_stop(buzzer_pin);
            break;
        case '6': 
            pico_buzzer_play(buzzer_pin, NOTE_LA); 
            sleep_ms(200);
            pico_buzzer_stop(buzzer_pin);
            break;
        case '7': 
            pico_buzzer_play(buzzer_pin, NOTE_SI); 
            sleep_ms(200);
            pico_buzzer_stop(buzzer_pin);
            break;
        case '8': 
            pico_buzzer_play(buzzer_pin, NOTE_DO2); 
            sleep_ms(200);
            pico_buzzer_stop(buzzer_pin);
            break;
        case 'A': play_marcha_soldado(buzzer_pin); break; // Toca a melodia "Marcha Soldado" quando a tecla 'A' é pressionada
        case '0': pico_buzzer_stop(buzzer_pin); break; // Para de tocar a nota no buzzer
        default: pico_buzzer_stop(buzzer_pin); break;
    }
}

//função principal
int main() {
    char key;
    bool music_mode = false; // Estado inicial: modo padrão (LEDs)
    stdio_init_all();
    pico_setup_keypad();
    pico_rgb_init(rgb_1);
    pico_rgb_init(rgb_2);
    pico_rgb_init(rgb_3);
    pico_buzzer_init(buzzer_pin);
    
    while (true) {
        key = pico_scan_keypad(); // Verifica se há alguma tecla pressionada
        if (key != '\0') {
            if (key == 'D') {
                music_mode = true; // Muda para o modo de reprodução de música
                pico_buzzer_play(buzzer_pin, NOTE_RE); // Toca uma nota para indicar a mudança de modo
                pico_rgb_turn_off(rgb_1); // Desliga todos os LEDS ao mudar para o modo de música
                pico_rgb_turn_off(rgb_2);
                pico_rgb_turn_off(rgb_3);
            } else if (key == '0') {
                music_mode = false; // Volta para o modo padrão (LEDs)
            }

            if (music_mode) {
                pico_keypad_control_music(key); // Executa a ação correspondente no modo de música
            } else {
                pico_keypad_control_led(key); // Executa a ação correspondente no modo padrão (LEDs)
            }
        }
        sleep_ms(100); // Pausa para evitar leitura repetida contínua
        
    }
    return 0;
    
}