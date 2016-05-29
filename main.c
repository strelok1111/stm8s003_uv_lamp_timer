#include "stm8s.h"
#include "main.h"
#include <math.h>

volatile uint16_t timer_count_sec = 0;
volatile uint8_t button_pressed_delay = 0;
volatile uint16_t max_count = 0;

//void _delay_ms(uint32_t ms){
//	uint32_t tiks = ms * 354;
//	while(tiks-- > 0);
//}
//void uart_write_c(const char c) {
//	while(!(UART1 -> SR & UART1_SR_TXE));
//	UART1 -> DR = c;
//}
//
//int uart_write(const char *str) {
//	char i;
//	for(i = 0; i < strlen(str); i++) {
//		uart_write_c(str[i]);
//	}
//	return(i);
//}
//void UART_init(void){
//	UART1 -> CR2 = UART1_CR2_TEN | UART1_CR2_REN | UART1_CR2_RIEN; // Allow TX & RX
//	UART1 -> CR3 &= ~(UART1_CR3_STOP); // 1 stop bit
//	UART1 -> BRR2 = 0x03;
//	UART1 -> BRR1 = 0x68; // 9600 baud
//	GPIOD -> DDR |= _BV(5); // Put TX line on
//	GPIOD -> CR1 |= _BV(5);
//}
void CLK_init(void){
	CLK->ECKR|=CLK_ECKR_HSEEN;
    CLK->SWCR|=CLK_SWCR_SWEN;
    CLK->SWR=0xB4;
    CLK->CKDIVR=0;
}
void enable_lights(uint8_t count){
	if(count == 0){
		GPIOB -> ODR |= _BV(4) ;
		GPIOC -> ODR &= ~_BV(3) & ~_BV(4) & ~_BV(5) & ~_BV(6) & ~_BV(7);
		GPIOD -> ODR &= ~_BV(2) & ~_BV(3);
	}else if(count == 1){
		GPIOB -> ODR |= _BV(4);
		GPIOC -> ODR &= ~_BV(3) & ~_BV(4) & ~_BV(5) & ~_BV(6) & ~_BV(7);
		GPIOD -> ODR &= ~_BV(2);
		GPIOD -> ODR |= _BV(3);
	}else if(count == 2){
		GPIOB -> ODR |= _BV(4);
		GPIOC -> ODR &= ~_BV(3) & ~_BV(4) & ~_BV(5) & ~_BV(6) & ~_BV(7);
		GPIOD -> ODR |= _BV(2) | _BV(3);
	}else if(count == 3){
		GPIOB -> ODR |= _BV(4);
		GPIOC -> ODR &= ~_BV(3) & ~_BV(4) & ~_BV(5) & ~_BV(6);
		GPIOC -> ODR |= _BV(7);
		GPIOD -> ODR |= _BV(2) | _BV(3);
	}else if(count == 4){
		GPIOB -> ODR |= _BV(4);
		GPIOC -> ODR &= ~_BV(3) & ~_BV(4) & ~_BV(5);
		GPIOC -> ODR |= _BV(7)  | _BV(6);
		GPIOD -> ODR |= _BV(2) | _BV(3);
	}else if(count == 5){
		GPIOB -> ODR |= _BV(4);
		GPIOC -> ODR &= ~_BV(3) & ~_BV(4) ;
		GPIOC -> ODR |= _BV(7)  | _BV(6) | _BV(5);
		GPIOD -> ODR |= _BV(2) | _BV(3);
	}else if(count == 6){
		GPIOB -> ODR |= _BV(4);
		GPIOC -> ODR &= ~_BV(3);
		GPIOC -> ODR |= _BV(7)  | _BV(6) | _BV(5) | _BV(4);
		GPIOD -> ODR |= _BV(2) | _BV(3);
	}else if(count == 7){
		GPIOB -> ODR |= _BV(4);
		GPIOC -> ODR |= _BV(7)  | _BV(6) | _BV(5) | _BV(4) | _BV(3);
		GPIOD -> ODR |= _BV(2) | _BV(3);
	}else if(count == 8){
		GPIOB -> ODR &= ~_BV(4);
		GPIOC -> ODR |= _BV(7) | _BV(6) | _BV(5) | _BV(4) | _BV(3);
		GPIOD -> ODR |= _BV(2) | _BV(3);
	}
}
void timer_stop(void){
	timer_count_sec = 0;
	max_count = 0;
	TICK_OFF;
	LAMP_OFF;
	enable_lights(0);
}
void timer_start(uint16_t sec){
	timer_count_sec = sec - 1;
	max_count = sec;
	LAMP_ON;
	TICK_ON;
	enable_lights(8);
	TIM1 -> CNTRH = 0;
	TIM1 -> CNTRL = 0;
}
uint8_t timer_started(void){
	return timer_count_sec > 0;
}
void press_button_1_call(void){
	if(timer_started()  && !button_pressed_delay){
		timer_stop();
	}else if(!timer_started() && !button_pressed_delay){
		timer_start(45);
	}
}
void press_button_2_call(void){
	if(timer_started() && !button_pressed_delay){
		timer_stop();
	}else if(!timer_started() && !button_pressed_delay){
		timer_start(180);
	}
}
void press_button_3_call(void){

}
void EXTI_D_Interrupt(void) __interrupt(6) {
	if(BUTTON_1_PRESSED){
		press_button_1_call();
	}
	if(BUTTON_2_PRESSED){
		press_button_2_call();
	}
	if(BUTTON_3_PRESSED){
		press_button_3_call();
	}
	button_pressed_delay = 1;
}
void GPIO_init(void){
	GPIOB -> DDR |= _BV(4) | _BV(5);
	GPIOB -> CR1 |= _BV(4) | _BV(5);
	GPIOB -> ODR |= _BV(4) | _BV(5);

	GPIOA -> DDR |= _BV(3);
	GPIOA -> CR1 |= _BV(3);

	GPIOC -> DDR |= _BV(3) | _BV(4) | _BV(5) | _BV(6) | _BV(7);
	GPIOC -> CR1 |= _BV(3) | _BV(4) | _BV(5) | _BV(6) | _BV(7);

	GPIOD -> DDR = _BV(2) | _BV(3);
	GPIOD -> DDR &= ~_BV(4) & ~_BV(5) & ~_BV(6);
	GPIOD -> CR1 = _BV(2) | _BV(3) | _BV(4) | _BV(5) | _BV(6);
	GPIOD -> CR2 = _BV(4) | _BV(5) | _BV(6);
	EXTI -> CR1 = _BV(BUTTON_EXTI_FALL_BIT);
}

void start_check(void){
	uint32_t delay;
	uint8_t count = 0;
	for(count = 0 ;count <= 8;count ++){
		enable_lights(count);
		delay = 10000;
		while(delay--);
	}
	for(count = 9 ;count > 0;count --){
		enable_lights(count-1);
		delay = 10000;
		while(delay--);
	}
}

void Timer_init(void){
	TIM1 -> CR1 |= TIM1_CR1_CEN;
	TIM1 -> IER |= TIM1_IER_TIE | TIM1_IER_CC3IE | TIM1_IER_CC4IE;
	TIM1 -> PSCRH = 8001 >> 8;
	TIM1 -> PSCRL = 8001 & 0xff;
	TIM1 -> CCR3H = 1000 >> 8;
	TIM1 -> CCR3L = 1000 & 0xff;
	TIM1 -> CCR4H = 500 >> 8;
	TIM1 -> CCR4L = 500 & 0xff;
	TIM1 -> EGR |= TIM1_EGR_UG;
}
void TIMER1_Interrupt(void) __interrupt(12) {
	if( TIM1 -> SR1 & TIM1_SR1_CC3IF){
		TIM1 -> SR1 &= ~TIM1_SR1_CC3IF;
		TIM1 -> CNTRH = 0;
		TIM1 -> CNTRL = 0;
		if(timer_started()){
			enable_lights((uint8_t)ceilf(((float)timer_count_sec / max_count) * 8.0));
			timer_count_sec--;
			TICK_TOGGLE;
		}else{
			timer_stop();
		}

	}
	if( TIM1 -> SR1 & TIM1_SR1_CC4IF){
		TIM1 -> SR1 &= ~TIM1_SR1_CC4IF;
		button_pressed_delay = 0;
		if(timer_started()){
			TICK_TOGGLE;
		}
	}
}
int main() {
	CLK_init();
	GPIO_init();
	Timer_init();
	enableInterrupts();
	start_check();
	while(1);
}
