# P4 - Como bibliotecas funcionam

## a

O p4-v1 e p4-v2 executam normalmente, gerando a saída esperada "Foo". Ao rodar o p4-v3 da mesma forma, recebe-se o erro "./p4-v3: error while loading shared libraries: libp4.so: cannot open shared object file: No such file or directory", pois a biblioteca dinâmica criada não está no diretório padrão do sistema para bibliotecas desse tipo. O sistema não sabe encontrar a libp4.so. Para conseguir rodar esse executável pode-se colocar o .so no diretório adequado (/usr/lib geralmente) ou acrescentar a flag LD_LIBRARY_PATH=. no comando, para dizer para o compilador que deve buscar bibliotecas dinâmeicas no diretório atual.

É possível rodar os executáveis p4-v1 e p4-v2 normalmente em qualquer diretório, já que todas as suas dependências estão resolvidas em seus próprios binários. Isso não ocorre com o p4-v3, como foi explicado acima. Para conseguir rodar o p4-v3 em qualquer diretório, podemos alterar o Makefile da seguinte maneira:

```
p4-v3 : p4.o libp4.so
	gcc -m32 $< -L. -lp4 -Wl,-rpath='$(CURDIR)' -o $@
```

Assim, definimos o caminho de busca por bibliotecas dinâmicas do ELF para o diretório onde nosso Makefile está, que é tembém onde a bilioteca libp4.so está localizada. Dessa forma o próprio executável informa ao linker dinâmico onde procurar sua dependência.

## b

Analisando o tamanho exato dos arquivos, temos que o p4-v1 possui o maior tamanho. Isso se dá pelo fato de esse executável ter recebido todo o conteúdo do arquivo p4a.c e p4b.o, incluido a função Bar que não é utilizada, por eles terem sido linkados diretamente ao p4.o. Na compilação do p4-v2, quando geramos uma biblioteca estática com p4a.o e p4b.o e a linkamos no p4.o, só é acoplado ao p4.o o código da função Foo, que é utilizada de fato. Por esse motivo o executável p4-v2 é um pouco menor. O executável p4-v3 apresenta o menor tamanho dos três por usar a biblioteca dinâmica libp4.so, gerada a partir dos mesmos p4a.o e p4b.o. A linkagem dinâmica não exige que nenhum código da biblioteca seja copiado no executável objetivo, nele só existe o nome da biblioteca dinâmica. A linkagem só vai ocorrer de fato no runtime, quando o código desejado que está na biblioteca será trazido para a memória.

## c

O comando nm nos mostra a tabela de símbolos do executável, ou seja, mostra objetos e como localizar seus valores/definições.

Listing da função foo:
Nos executáveis p4-v1 e p4-v2 está com a letra 'T', o que indica que esta função está definida no binário (veio da linkagem direta e da biblioteca estática, respectivamente). Já no p4-v3, ela está com a letra 'U' (de undefined), por ter de ser resolvida em tempo de execução (vem da biblioteca dinâmica).

Listing da função bar:
No executável p4-v1 está com a letra 'T', já que está definida no binário mesmo sem uso (veio da linkagem direta). Nos executáveis p4-v2 e p4-v3 ela não aparece, já que sua referência foi descartada nesses binários pelo fato de ela não estar sendo usada de fato e nem estar definida no binário como no p4-v1. Não teria porquê o linker colocá-la na tabela de símbolos nesses casos.

## d

O p4-v3, além da dependência da bilioteca dinâmica padrão do C (para a função printf) que p4-v1 e p4-v2 possuem, tem a dependência Shared library: [libp4.so], a biblioteca dinâmica que criamos.


O Kernel, quando executa um programa com dependências dinâmicas, invoca o linker dinâmico. O linker, quando vê essas dependências (diretiva Shared library), procura a localização dos arquivos .so dessas bibliotecas. Primeiro ele verifica se algum caminho foi especificado no parâmetro rpath do ELF. Depois, ele verifica se algum caminho foi passado por parâmetro no comando de execução do binário por meio da variável LD_LIBRARY_PATH. Por fim, ele procura nas bibliotecas padrão de localização das bibliotecas dinâmicas (/usr/lib, por exemplo). Se as dependências foram encontradas, seus códigos serão colocados em memória e linkados ao ELF objetivo. Se não, ocorre um erro.

## e

Uma biblioteca estática é um arquivo objeto que contém uma coleção de funções, variáveis e objetos compilados que são acoplados ao executável que tem esses itens como dependência **durante sua compilação**. O código da biblioteca passa a fazer parte do binário gerado.
Uma biblioteca dinâmica também é um arquivo objeto que contém uma coleção de funções, variáveis e objetos compilados, mas será carregada na memória durante a execução de um programa que a tenha como dependência para ser linkada em runtime. O executável que depende de uma biblioteca dinâmica não receberá nenhuma parte de seu código, apenas terá uma referência à essa biblioteca.
Uma biblioteca dinâmica pode ser de dois tipos: de código realocável (dependente de posições de memória) e PIC (Position Independent Code), ou seja, um tipo é o oposto do outro. No caso de uma biblioteca que depende de posições, em seu executável as referências para seus objetos constituintes está definido estaticamente, com os endereços de memória "preferidos" para aquela biblioteca. Quando não é possivel carregar a biblioteca nesses endereços específicos, o linker dinâmico modifica esses endereços usando uma tabela de realocação. A vantagem de usar bibliotecas assim é bem reduzida, especialmente em sistemas modernos: não é necessário calcular o offset das posições, é possível acessá-las diretamente. Já uma biblioteca independente de posições de memória pode ser executada independente de onde seu código foi carregado, pois seu endereçamento é relativo: o acesso às posições é relativo ao Program Counter. A principal vantagem de se utilizar bibliotecas com esse mecanismo é que vários processos podem compartilhar a mesma biblioteca na memória física, que pode estar mapeada em endereços virtuais diferentes. Além disso, é uma técnica de segurança poder aleatorizar o endereço de funções (ASLR).

Como um executável que só utiliza bibliotecas estáticas possuirá todo o código de suas dependências nele, ele pode rodar em qualquer ambiente. Diferentemente de um executável que depende de bibliotecas dinâmicas: será necessário que todos os arquivos dessas bibliotecas estejam no ambiente em que se quer rodá-lo, com os caminhos configurados corretamente.

Quando uma biblioteca estática é atualizada, será necessário recompilar todos os programas que a utilizam linkando a nova versão. No caso de uma biblioteca dinâmica, basta atualizar os .so que estão sendo utilizados para os programas passarem a usar a nova versão.

No caso em que vários programas que dependem de uma mesma biblioteca serão executados em uma mesma máquina, o uso de bibliotecas dinâmicas representará uma enorme economia de espaço, pois só existirá uma cópia do código de cada biblioteca. Os programas poderão compartilhar os .so que estarão em memória. Se fosse uma mesma biblioteca estática, cada programa teria uma cópia sua embutida em seu binário.

