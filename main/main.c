#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/gpio.h"

#define LED_PIN GPIO_NUM_2  // Pino do LED embarcado
#define SHORT_DELAY_MS 200  // Sinal curto (200ms)
#define LONG_DELAY_MS 600   // Sinal longo (600ms)
#define PAUSE_BETWEEN_BLINKS 200
#define PAUSE_BETWEEN_LETTERS 600

void blink_led(int duration) {
    gpio_set_level(LED_PIN, 1);  // Liga o LED
    vTaskDelay(pdMS_TO_TICKS(duration));
    gpio_set_level(LED_PIN, 0);  // Desliga o LED
    vTaskDelay(pdMS_TO_TICKS(PAUSE_BETWEEN_BLINKS));
}

void send_sos_task(void *pvParameter) {
    // Configura o pino do LED como saída
    gpio_reset_pin(LED_PIN);
    gpio_set_direction(LED_PIN, GPIO_MODE_OUTPUT);

    while (1) {
        // S: três sinais curtos
        for (int i = 0; i < 3; i++) blink_led(SHORT_DELAY_MS);

        vTaskDelay(pdMS_TO_TICKS(PAUSE_BETWEEN_LETTERS));

        // O: três sinais longos
        for (int i = 0; i < 3; i++) blink_led(LONG_DELAY_MS);

        vTaskDelay(pdMS_TO_TICKS(PAUSE_BETWEEN_LETTERS));

        // S: três sinais curtos
        for (int i = 0; i < 3; i++) blink_led(SHORT_DELAY_MS);

        vTaskDelay(pdMS_TO_TICKS(3000));  // Pausa final entre os sinais SOS
    }

    vTaskDelete(NULL);  // Nunca será chamado devido ao loop infinito
}

void app_main() {
    xTaskCreate(&send_sos_task, "send_sos_task", 2048, NULL, 5, NULL);
}