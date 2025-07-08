# P3 - Function calls

### a) Locate the line where 'foo' calls 'bar' and explain how the caller passes the parameter to the callee. In your answer, clarify the concepts of calling convention and application binary interface.

```

mov    0x8(%ebp),%eax      ; lê o argumento n
add    $0x1,%eax           ; faz n + 1
push   %eax                ; empilha o argumento m = n+1
call   804919f <bar>       ; chama bar, sendo 904919f o endereço de memória onde a função bar começa

```

O parâmetro é passado pela pilha. A Calling Convention consiste no conjunto de regras sobre como passar argumentos e retornar valores, já a ABI (Application Binary Interface) define as calling conventions, alinhamento de memória, registradores usados etc., garantindo que diferentes partes de um programa (ou programas diferentes) compiladas separadamente possam se comunicar e funcionar juntas.

### b) How 'bar' returns its result to 'foo'? How does the answer relate to what you discussed in item (a)? Is it a linux-only thing?

```

mov    0x8(%ebp),%eax      ; pega m
add    $0x1,%eax           ; b = m + 1 ; retorno implícito: o valor de retorno fica em %eax

```

Nesse caso a função foo() precisa apenas ler %eax depois do call. Esse esquema é o padrão da ABI em arquiteturas x86, tanto em Linux quanto em Windows.

### c) Explain what is the purpose of the first two instruction and the second-to-last instruction of the function 'foo'. Give a didactic overview of what they do and what would happen if they were suppressed? Are they always necessary, or can they be omitted in some circumstances?

```

push %ebp
mov %esp, %ebp

...

leave

```

As duas primeiras linhas determinam o chamado "Prólogo da Função", que é responsável por configurar o stack frame da atual função, permitindo acessar os parâmetros/variáveis locais por %ebp + offset. Já a penúltima intrução determina o "Epílogo da Função", que finaliza o stack frame da função. Essas instruções são cruciais para a correta gestão da pilha e o controle de fluxo de um programa. Elas garantem que:

- Cada função tenha seu próprio espaço isolado na pilha para suas variáveis locais e parâmetros.

- As funções possam chamar umas às outras sem corromper seus respectivos ambientes de execução.

- O controle possa ser retornado corretamente para a função chamadora após a conclusão de uma função.

Caso essas linhas fossem suprimidas, as variáveis locais ficariam inacessíveis ou quebrariam o programa:

- Supressão do Prólogo: A função sobrescreveria dados na pilha que pertencem à função chamadora, levando a corrupção de dados e falhas. O retorno para a função chamadora seria incorreto, pois o %ebp não seria restaurado adequadamente.

- Supressão do Epílogo: O espaço da pilha alocado para as variáveis locais da função não seria liberado. Isso levaria a um uso ineficiente da memória da pilha e, além disso, o %ebp e %esp não seriam restaurados, fazendo com que o ret retornasse para um endereço incorreto.

Tais instruções são apenas suprimidas em contexto de otimização do frame pointer ou quando uma função é simples (não aloca variáveis locais complexas, não chama outras funções), tirando esses casos, elas são de suma importância e não devem ser suprimidas.

### d) What is the purpose of the third line in both functions 'foo' and 'bar' and what does it have to do with the x86 ABI? Does it serve the same purpose than the third line of 'main'? Explain didactically how those pieces of code work.

```

foo: sub $0x18,%esp
bar: sub $0x10,%esp

```

O propósito dessas instruções é alocar espaço na pilha para as variáveis locais da respectiva função.

Em foo, sub $0x18,%esp subtrai 0x18 (24 em decimal) do registrador %esp. Isso move o Stack Pointer para "baixo" na pilha (já que a pilha cresce para endereços de memória menores), reservando 24 bytes de espaço. Esse espaço é usado para a variável local a e para alinhar a pilha conforme a ABI.

Já em bar, sub $0x10,%esp subtrai 0x10 (16 em decimal) do %esp, reservando 16 bytes para a variável local b e alinhamento.

A terceira linha da main alinha a pilha em 16 bytes, exigência para chamadas de funções seguras com instruções SIMD (conforme a ABI do GCC). Em vez de subtrair um valor fixo, and $0xfffffff0,%esp alinha o Stack Pointer %esp para baixo até o próximo múltiplo de 16 bytes.

0xfffffff0 é uma máscara de bits que, quando aplicada com uma operação AND, zera os últimos 4 bits de um número, arredondando ele para baixo até o múltiplo de 16 mais próximo.

### e) In the source code, change the return type of 'bar' to 'char'. Rebuild p3 and examine the difference in the disassembly of 'foo'. Explain what the compiler did and, based on your answer, discuss why the declaration of 'foo' and 'bar' before 'main' are useful. 

A diferença nos assemblys do p3 antes e depois da alteração no tipo de retorno da função bar é a adição desta linha logo depois do retorno dessa função, na função foo:

``` movsbl %al,%eax ```

O que o compilador faz aqui é pegar os 8 bits menos significativos do registrador eax que são manipulados pela função bar (o registrador al guarda esse valor) e extende esse valor para 32 bits, e o coloca novamente no eax. Esse tratamento adapta o retorno da função, que tem 8 bits (char) para a variável que o recebe, que tem 32 bits (int)

Uma utilidade que declarar as funções antes da main traz é que é fácil visualizar os tipos das funções que o arquivo contém. Ao mudar o tipo da função bar, recebe-se um aviso de que sua declaração não condiz com o que foi definido anteriormente. Ou seja, é um obstáculo de atenção para mudanças de tipo que podem ser comprometedoras.