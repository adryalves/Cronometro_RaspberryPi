#include <stdio.h>
#include "pico/stdlib.h"

#include "hardware/i2c.h"
#include "hardware/gpio.h"
#include "inc/ssd1306.h"

#define I2C_PORT i2c1
#define I2C_SDA 14
#define I2C_SCL 15
#define endereco 0x3C

// Botões
const uint button_A = 5;
const uint button_B = 6;

ssd1306_t ssd; // Instância do display

volatile int counter = 0;               // Contador de minutos (0 a 4)
volatile bool running = false;          // Indica se a contagem regressiva está ativa
static volatile uint32_t last_time = 0; // Tempo do último evento (usado para debounce)

// led
const uint led_A = 13;

// Função para atualizar o display com o número atual
void atualizar_display(int numero)
{
    ssd1306_fill(&ssd, false); // Limpa o display
    char mensagem[16];
    sprintf(mensagem, "Valor: %d", numero);
    ssd1306_draw_string(&ssd, mensagem, 8, 10); // Exibe a mensagem
    ssd1306_send_data(&ssd);                    // Atualiza o display
}

// Manipulador de interrupção dos botões
static void gpio_irq_handler(uint gpio, uint32_t events)
{
    uint32_t current_time = to_us_since_boot(get_absolute_time());

    if (current_time - last_time > 200000)
    { // Debounce: 200 ms
        last_time = current_time;

        if (gpio == button_A && !running)
        {                                // Botão A: Incrementa o contador
            counter = (counter + 1) % 5; // Ciclo entre 0 e 4
            atualizar_display(counter);
            gpio_put(led_A, 0);
        }
        else if (gpio == button_B && counter > 0)
        { // Botão B: Inicia contagem regressiva
            running = true;
        }
    }
}

int main()
{
    stdio_init_all();

    // Inicializa botões
    gpio_init(button_A);
    gpio_set_dir(button_A, GPIO_IN);
    gpio_pull_up(button_A);
    gpio_init(button_B);
    gpio_set_dir(button_B, GPIO_IN);
    gpio_pull_up(button_B);

    // inicializar o LED verde - GPIO13
    gpio_init(led_A);              // Inicializa o pino do LED
    gpio_set_dir(led_A, GPIO_OUT); // Configura o pino como saída

    gpio_set_irq_enabled_with_callback(button_A, GPIO_IRQ_EDGE_FALL, true, &gpio_irq_handler);
    gpio_set_irq_enabled_with_callback(button_B, GPIO_IRQ_EDGE_FALL, true, &gpio_irq_handler);

    // Inicializa I2C e display
    i2c_init(I2C_PORT, 400 * 1000);
    gpio_set_function(I2C_SDA, GPIO_FUNC_I2C);
    gpio_set_function(I2C_SCL, GPIO_FUNC_I2C);
    gpio_pull_up(I2C_SDA);
    gpio_pull_up(I2C_SCL);

    ssd1306_init(&ssd, 128, 64, false, endereco, I2C_PORT);
    ssd1306_config(&ssd);
    ssd1306_fill(&ssd, false);
    ssd1306_send_data(&ssd);

    atualizar_display(counter); // Exibe o valor inicial

    int segundos_restantes = 0;
    absolute_time_t ultimo_update = get_absolute_time();

    while (true)
    {
        if (running)
        {
            segundos_restantes = counter * 60; // Converte minutos para segundos

            while (segundos_restantes >= 0 && running)
            {
                if (absolute_time_diff_us(ultimo_update, get_absolute_time()) >= 1000000)
                {
                    atualizar_display(segundos_restantes);
                    segundos_restantes--;
                    ultimo_update = get_absolute_time(); // Atualiza o tempo do último segundo

                    if (segundos_restantes < 0)
                    {
                        running = false; // Finaliza a contagem ao chegar em zero
                        counter = 0;
                        gpio_put(led_A, 1);
                    }
                }
                tight_loop_contents(); // Mantém o watchdog e interrupções ativas
            }
        }

        tight_loop_contents(); // Evita que o processador fique ocioso
    }
}
