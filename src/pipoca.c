#include <stdint.h>
#include <stdlib.h>
#include "evlt7t.h"
#include "pipoca.h"

/**
 * Tratamento da interrupção externa 0 (sinal P8, conectado ao botão).
 * Pisca o segundo led.
 */
void trata_irq_ext0(void) {
   bit_inv(IOPDATA, 5);
}

/**
 * Função de inicialização do tratamento da interrupção do botão.
 */
void 
inicia_botao(void) {
   /*
    * Configura a saída do led.
    */
   bit_set(IOPMOD, 5);     // configura o pino 5 como saída (segundo led).
   bit_clr(IOPDATA, 5);    // inicia o led apagado.

   /*
    * Configura P8 como entrada com interrupção.
    */
   bit_clr(IOPMOD, 8);
   IOPCON = (IOPCON & ~(0b11111)) | 0b11001;   // borda de subida
   bit_clr(INTMSK, 0);      // habilita interrupção externa 0
}

/**
 * Base de tempo do sistema, incrementado a cada 1 milissegundo.
 */
volatile static uint32_t ticks;

/**
 * Tratamento da interrupção do timer 0.
 * Chamado por trata_irq().
 */
void trata_irq_timer0(void) {
   ticks++;
}

/**
 * Lê o valor atual dos ticks com segurança.
 * @return Valor atual do contador de ticks.
 */
uint32_t get_ticks(void) {
   uint32_t res;
   bit_set(INTMSK, 10);
   res = ticks;
   bit_clr(INTMSK, 10);
   return res;
}

/**
 * Espera ocioso com base no contador ticks.
 * @param v Tempo a esperar em unidades de 1ms.
 */
void delay_tempo(uint32_t v) {
   uint32_t inicio = get_ticks();
   while((get_ticks() - inicio) < v) ;
}

/**
 * Configura o timer0 para gerar interrupção a cada 1ms.
 */
void inicia_timer0(void) {
   TDATA0 = 49999;        // valor para 0.001s com clock de 50 MHz
   TCNT0 = TDATA0;
   TMOD = (TMOD & (~0b111)) | 0b011;  // ativa o timer 0
   bit_clr(INTMSK, 10);     // habilita interrupção do timer 0
   bit_clr(INTMSK, 21);     // habilita interrupções globais 
}


/*
 * buffer de transmissão
 */
static volatile uint32_t ntx = 0;        ///< Quantidade de bytes a transmitir
static volatile uint8_t *ptx = NULL;     ///< Ponteiro para o próximo byte a transmitir

/**
 * Tratamento da interrupção de transmissão da UART 0.
 * Função chamada por trata_irq().
 */
void 
trata_irq_uart0_tx(void) {
   if(ntx == 0) return;
   ntx--;
   if(ntx == 0) return;
   ptx++;
   UTXBUF0 = *ptx;
}

/**
 * libc não está presente.
 */
int 
strlen(char *s) {
   int n = 0;
   while(*s) {
      n++;
      s++;
   }
   return n;
}

/**
 * Envia um string através da serial, usando interrupções.
 * @param str String a enviar.
 */
void 
serie(char *str) {
   while(ntx) ;             // verifica transmissão pendente
   ntx = strlen(str);
   ptx = str;
   UTXBUF0 = str[0];        // envia o primeiro caractere.
}

/**
 * Configura a UART 0 (serial do usuário).
 * (somente transmissão)
 */
void 
inicia_serial(int baud) {
   ULCON0 = 0b111;           // 8N2, sem paridade, clock interno
   UCON0 = 0b1001;           // TX e RX habilitado
   if (baud == 115200){
   	UBRDIV0 = (13 << 4); // 13, valor para 115200 em 50 MHz
   } else if (baud == 57600){
   	UBRDIV0 = (26 << 4); // 26, valor para 57600 em 50 MHz
   } else if (baud == 38400){
   	UBRDIV0 = (40 << 4); // 40, valor para 38400 em 50 MHz
   } else if (baud == 19200){
   	UBRDIV0 = (80 << 4); // 80, valor para 19200 em 50 MHz
   } else if (baud == 4800){
   	UBRDIV0 = (324 << 4); // 324, valor para 4800 em 50 MHz
   } else if (baud == 2400){
   	UBRDIV0 = (650 << 4); // 650, valor para 2400 em 50 MHz
   } else if (baud == 1200){
   	UBRDIV0 = (1301 << 4); // 1301, valor para 1200 em 50 MHz
   } else {  	//valor de exceção para preenchimentos errados
	UBRDIV0 = (162 << 4); // 162, valor para 9600 em 50 MHz
   }
   
   bit_clr(INTMSK, 4);    // habilita interrupção da transmissão.
}

