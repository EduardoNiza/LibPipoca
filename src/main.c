
#include "evlt7t.h"
#include <stdint.h>
#include <stdlib.h>
#include "pipoca.h"

//deixe código aqui para rodar uma vez
void setup(){
    UsaGPIO(4, OUTPUT);
    IniciaSerial(9600);

}

//deixe código aqui para rodadr em loop
void loop(){
    SerialEnvia("Hello people!\r\n");
    EscreveGPIO(4, HIGH);
    delayms(500);
    EscreveGPIO(4, LOW);
    delayms(500);
}

//Aqui temos as interrupções externas, que vocêpode usar ou não
void interr_ext0(){}
void interr_ext1(){}
void interr_ext2(){}
void interr_ext3(){}
