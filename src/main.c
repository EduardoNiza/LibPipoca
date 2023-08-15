
#include "evlt7t.h"
#include <stdint.h>
#include <stdlib.h>
#include "pipoca.h"
#include "main.h"

//deixe código aqui para rodar uma vez
void setup(void){
    UsaGPIO(LED1, OUTPUT);
    IniciaSerial(9600);

}

//deixe código aqui para rodadr em loop
void loop(void){
    SerialEnvia("Hello people!\r\n");
    EscreveGPIO(LED1, HIGH);
    delayms(500);
    EscreveGPIO(LED1, LOW);
    delayms(500);
}

//Aqui temos as interrupções externas, que vocêpode usar ou não

