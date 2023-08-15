#pragma once

#include <stdint.h>
#include <stdlib.h>
#include "evlt7t.h"

void trata_irq_timer0(void);
uint32_t get_ticks(void);
void delay_tempo(uint32_t v);
void inicia_timer0(void);
void trata_irq_ext0(void);
void inicia_botao(void);
