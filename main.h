/*
 * main.h
 *
 *  Created on: 02 марта 2016 г.
 *      Author: strelok
 */

#ifndef MAIN_H_
#define MAIN_H_
#include "stm8s.h"

#define _BV(a) (1 << a)
#define BUTTON_EXTI_FALL_BIT 7

#define LAMP_ON GPIOA -> ODR |= _BV(3)
#define LAMP_OFF GPIOA -> ODR &= ~_BV(3)

#define TICK_ON GPIOB -> ODR &= ~_BV(5)
#define TICK_OFF GPIOB -> ODR |= _BV(5)
#define TICK_TOGGLE GPIOB -> ODR ^= _BV(5)

#define BUTTON_1_PRESSED !(GPIOD -> IDR & _BV(4))
#define BUTTON_2_PRESSED !(GPIOD -> IDR & _BV(5))
#define BUTTON_3_PRESSED !(GPIOD -> IDR & _BV(6))


#endif /* MAIN_H_ */
