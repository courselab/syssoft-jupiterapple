# P2 -Design Vulnerability

## Explicando o Problema

Quando digitamos “foo”, a função *strcmp* faz a comparação adequada, atribui o valor de 1 para a variável *verified*, e o acesso ao restante do código é concedido. Quando digita-se algo diferente de “foo”, como “bar”, por exemplo, o *strcmp* vai apontar a diferenciação do valor esperado, e a variável *verified* irá continuar com o valor que foi atribuído no início do código, ou seja, zero. Dessa forma, ela permite entrar na validação de que o usuário não inseriu a senha correta e seu acesso deve ser negado, finalizando a execução do programa naquele ponto.

A vulnerabilidade do código, porém, acontece quando a senha inserida possui um tamanho maior do que o que foi alocado para o buffer, causando o que é chamado de **Buffer Overflow.**

No começo do código, duas variáveis são declaradas:

```
  int verified = 0;
   char user_key[10];
```

Durante o processo de montagem do código, dois registradores são alocados para apontar para a seção de memória reservada para essas variáveis: o **Base Pointer** e o **Stack Pointer**, de forma que esses espaços são alocados **consecutivamente**.

A cada caractere inserido na *user_key*, o Stack Pointer salva esse valor na posição de memória correspondente e “incrementa” sua posição em 1 para receber o valor seguinte.

Mais uma vez no código, percebe-se que a variável user_key é preenchida nesse trecho:

```
scanf ("%s", user_key);
```

Nesse ponto, observa-se um **risco de vulnerabilidade** muito alto, pois o template de string não limita a quantidade de caracteres que será recebida na variável. Aqui, independentemente do tamanho, o *strcmp* irá fazer a comparação corretamente, não atribuindo o valor 1 (true) para *verified*.

No entanto, ao observar o comportamento dos ponteiros da pilha na atribuição dos caracteres para a variável *user_key*, é possível evidenciar que, ao receber o valor da última posição possível (nesse caso, o referente à posição 9 no array e a 10 posições acima da posição inicial na memória), o ponteiro incrementa normalmente, mas, dessa vez, ele está **alterando valores alocados para outras finalidades**, e, nesse caso, ele está alterando o valor da variável *verified*. Dessa forma, quando o código tenta fazer a verificação da variável, procurando pelo 0 ou pelo 1 (`if (!verified)`), ele encontra o valor que foi sobrescrito pelo excedente da string, fazendo com que ele pule a etapa de bloqueio do acesso e permita que o usuário que inseriu a senha com tamanho superior ao destinado para a função entre na parte restrita do código.

Em resumo, foi notável que a causa da **falha no design** foi o uso do *scanf*, que permitiu a cópia de um valor ilimitado para um buffer com tamanho restrito pré-definido, sobrescrevendo seções alheias de memória.

## Possível Solução

Dado o problema, uma solução para que o **Buffer Overflow** não ocorra é **restringir a quantidade de caracteres que serão recebidos do usuário**. Para isso, é possível utilizar a função **fgets**, que permite limitar os bytes recebidos no *stdin*, ignorando qualquer excedente que o usuário venha a inserir.
