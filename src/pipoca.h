#pragma once

#include <stdint.h>
#include <stdlib.h>
#include "evlt7t.h"

#define HIGH    1
#define True    1
#define OUTPUT  1
#define LOW     0
#define False   0
#define INPUT   0
#define INPUT_INTERRUPT 2

#define CHAVE4  0
#define CHAVE3  1
#define CHAVE2  2
#define CHAVE1  3
#define LED1    7
#define LED2    6
#define LED3    5
#define LED4    4
#define BOTAO   8

#define SEGa    10
#define SEGb    11
#define SEGc    12
#define SEGd    13
#define SEGe    14
#define SEGg    15
#define SEGf    16

void trata_irq_timer0(void);
uint32_t get_ticks(void);
void svrsetup(void);
void delayms(uint32_t v);
void inicia_timer0(void);
void interr_ext0(void);
void interr_ext1(void);
void interr_ext2(void);
void interr_ext3(void);
void SerialEnvia(char *str);
void IniciaSerial(uint16_t baud);

void UsaGPIO(uint8_t pino, uint8_t modo);
void EscreveGPIO(uint8_t pino, uint8_t valor);
int LeGPIO(uint8_t pino);
