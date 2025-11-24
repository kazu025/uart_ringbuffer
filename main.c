#include<stdio.h>
#include"pico/stdlib.h"
#include"hardware/uart.h"
#include"hardware/irq.h"
#include"ring_bffer.h"
#define UART_ID uart0
#define BAUD_RATE 115200
#define DATA_BITS 8
#define STOP_BITS 1
#define PARITY    UART_PARITY_NONE

#define UART_TX_PIN 0
#define UART_RX_PIN 1

static RingBuffer rx_rb;

// UART 受信割り込みハンドラ
void on_uart_tx(){
	while(uart_is_readable(UART_ID)){
		uint8_t ch = uart_getc(UART_ID);
		rb_push(&rx_rb, ch);
	}
}
void init_uart(void){
	rb_init(&rx_rb);
	// UART初期化
	uart_init(UART_ID, BAUD_RATE);
	gpio_set_function(UART_TX_PIN, UART_FUNCSEL_NUM(UART_ID, UART_TX_PIN));
	gpio_set_function(UART_RX_PIN, UART_FUNCSEL_NUM(UART_ID, UART_RX_PIN));
	// H/Wフローコントロール無効
	uart_set_hw_flow(UART_ID, false, false);
	// フレームフォーマット設定
	uart_set_format(UART_ID, DATA_BITS, STOP_BITS, PARITY);
	
	//割り込み設定
	int UART_IRQ = UART_ID == uart0 ? UART0_IRQ : UART1_IRQ;
	irq_set_exclusive_handler(UART_IRQ, on_uart_tx);
	irq_set_enabled(UART_IRQ, true);
	// 受信割り込み：有効　送信割り込み：無効
	uart_set_irq_enables(UART_ID, true, false);
}

void led_init(void){
	gpio_init(PICO_DEFAULT_LED_PIN);
	gpio_set_dir(PICO_DEFAULT_LED_PIN, GPIO_OUT);
}
void led_sw(void){
	static bool flag = true;
	gpio_put(PICO_DEFAULT_LED_PIN, flag);
	flag = !flag;
}

int main(void){
	stdio_init_all();
	led_init();
	init_uart();
	sleep_ms(3000);
	printf("UART ring buffer test.\n");
	while(true){

		uint8_t ch;
		if(rb_pop(&rx_rb, &ch)){
			uart_putc(UART_ID, ch);
			printf("%c", ch);
			led_sw();
		}
	}
}