#include <stdint.h>
#include <stdlib.h>
#include "evlt7t.h"
#include "pipoca.h"


void trata_irq_uart0_tx(void);
void SerialEnvia(char *str);
/**
 * Tratamento da interrupção externa 0 (sinal P8, conectado ao botão).
 * Pisca o segundo led.
 */

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
 * Tempo a esperar em unidades de 1ms.
 */
void delayms(uint32_t v) {
   uint32_t inicio = get_ticks();
   while((get_ticks() - inicio) < v) ;
}

/**
 * Tratamento das interrupções do ARM.
 */
void __attribute__((interrupt("IRQ")))
trata_irq(void) {
   /*
    * Verifica causa da interrupção.
    */
   uint32_t pend = INTPND;
   if(bit_is_set(pend, 10)) trata_irq_timer0();
   if(bit_is_set(pend, 4)) trata_irq_uart0_tx();
   if(bit_is_set(pend, 0)) interr_ext0();
   if(bit_is_set(pend, 1)) interr_ext1();
   if(bit_is_set(pend, 2)) interr_ext2();
   if(bit_is_set(pend, 3)) interr_ext3();

   /*
    * Reconhece todas as interrupções.
    */
   INTPND = pend;
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

int LeGPIO(uint8_t pino){ //Leitura digital de Pino
    return bit_is_set(IOPDATA, pino);
}

void EscreveGPIO(uint8_t pino, uint8_t valor){ //escrita digital de pino
    if(bit_is_set(IOPMOD, pino)){
        if(valor)
            bit_set(IOPDATA, pino);
        else
            bit_clr(IOPDATA, pino);
    }
}

void UsaGPIO(uint8_t pino, uint8_t modo){ //declaração do pino

    if (modo == OUTPUT){
        bit_set(IOPMOD, pino);
    } else if(pino <= 11 && pino >=8 && modo >= INPUT_INTERRUPT){
        bit_clr(IOPMOD, pino);
        IOPCON = IOPCON | (0b11001 << (pino-8)); // interrupccao com borda de subida
        bit_clr(INTMSK, (pino-8)); // habilita interrupção do pino
        bit_clr(INTMSK, 21);     // habilita interrupções globais
    }
    else
        bit_clr(IOPMOD, pino); // INPUT se houver exceção

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
void trata_irq_uart0_tx(void) {
   if(ntx == 0) return;
   ntx--;
   if(ntx == 0) return;
   ptx++;
   UTXBUF0 = *ptx;
}

int strtam(char *s) {
   int n = 0;
   while(*s) {
      n++;
      s++;
   }
   return n;
}

/**
 * Envia um string através da serial, usando interrupções.
 */
void SerialEnvia(char *str) {
   while(ntx) ;             // verifica transmissão pendente
   ntx = strtam(str);
   ptx = str;
   UTXBUF0 = str[0];        // envia o primeiro caractere
}

/**
 * Configura a UART 0 (serial do usuário).
 * (somente transmissão)
 */
void IniciaSerial(uint16_t baud) {
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
   bit_clr(INTMSK, 5);    // habilita interrupção da recepção.
}

void svrsetup(void){
    inicia_timer0();
}

