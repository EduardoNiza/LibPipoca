# LibPipoca

Biblioteca de simplificação e aglutinação de funcionalidades da Evaluator7T:
Pra você que só quer pipocar seu código lá 🍿

Codifique na Evaluator como se fosse um Arduino.

Funções suportadas:
IniciaGPIO(pino, estado) : 
Pino : apelido do pino
Estado: Entrada (0) ou saída (1)

UsaGPIO(pino , estado)
Pino : apelido do pino
Estado: Ligado (1, HIGH, True) ou Desligado (0, LOW, False)

Delay (tempo)
Tempo em milissegundos (limite de tempo 32bits/50MHz)

Serial0(baud, protocolo)
baud: Baud Rate (default 9600)
protocolo: Configura protocolo (5N1 até 8E2) (default 8N1)

Serial1(baud, protocolo)
baud: Baud Rate (default 9600)
protocolo: Configura protocolo (5N1 até 8E2) (default 8N1)
