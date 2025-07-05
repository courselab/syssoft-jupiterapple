# P1 - Endereçamento de memória

## Explicação do programa

O programa P1, quando executado irá imprimir o endereço de memória virtual da
função main.

## Explicação dos resultados

Em C, o nome de uma função representa um ponteiro para seu código, e pode ser
impresso como um endereço. Esse endereço será determinado pelo endereço de
início do programa e um offset, tal soma é armazenada e associada ao nome da
função main como seu endereço e representando seu conteúdo.

Quando o programa é executado e se torna um processo, ele irá receber um
endereço de memória virtual do SO, portanto apenas durante o processo de
carregamento do programa será possível determinar o endereço virtual da função.

Esse endereço irá variar entre execuções devido à técnica de segurança
ASLR (Address Space Layout Randomization), que randomiza o local em que cada
parte do programa é carregada na memória virtual.
