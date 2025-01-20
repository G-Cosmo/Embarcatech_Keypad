# Embarcatech_Keypad
Repositório criado para a tarefa 1 de Microcontroladores - GPIO

Grupo 2 - Subgrupo 6

Membros:

*Alisson Silva de Pinho*

*Eder Renato da Silva Cardoso Casar*

*Gabriel Cosmo Oliveira*

*Guilherme Fontana Cardoso*

*Helena Filemon Almeida Silva dos Santos*

*Melk Silva Braga*

*Sarah Modesto Sanches*

*Werliarlinson de Lima Sá Teles*

# Instruções de compilação

Para compilar o código, são necessárias as seguintes extensões: 

*Wokwi Simulator*

*Raspberry Pi Pico*

*Cmake*

Após instalá-las basta buildar o projeto pelo CMake. A partir daí, abra o arquivo 
diagram.json e clique no botão verde para iniciar a simulação.

Enquanto na simulação, o usuário pode clicar nos botões do teclado da simulação
a fim de acender as luzes conectadas à placa ou iniciar o buzzer.

## No modo led

As teclas *1*, *2* e *3* alternam entre ligada e desligada as luzes vermelha, verde
e azul respectivamente no primeiro led.

As teclas *4*, *5* e *6* alternam entre ligada e desligada as luzes vermelha, verde
e azul respectivamente no segundo led.

As teclas *7*, *8* e *9* alternam entre ligada e desligada as luzes vermelha, verde
e azul respectivamente no terceiro led.

As teclas *A*, *B* e *C* desligam o primeiro, segundo e terceiro leds respectivamente.

A tecla * liga todos os leds de uma vez.

A tecla *0* desliga todos os leds de uma vez.

## No modo música

A tecla *D* inicia o modo música.

As teclas *1*, *2*, *3* e *4* tocam as notas *Do*, *Re*, *Mi* e *Fa* respectivamente.

As teclas *5*, *6*, *7* e *8* tocam as notas *Sol*, *La*, *Si* e *Do* respectivamente.

A tecla *A* toca a música "Marcha Soldado".

A tecla *0* sai do modo música e volta para o modo led.

## Vídeo de demonstração

https://youtu.be/A282mXdGhRc
