# Compilador I++

A linguagem I++ foi desenvolvida pelo aluno de graduação em Engenharia da Computação, Igor Carvalho de Araújo, matriculado na Universidade Federal de Itajubá, durante a realização da máteria Compiladores, ministrada pela Prof. Dra. Thatyana de Faria Piola Seraphim.

## Conteúdo do diretório fonte

O diretório fonte do projeto conta com, além do arquivo makefile e deste README, com o diagrama de classes do projeto (feito no diagramador Dia) e três diretórios: 

bin: contém o executável gerado a partir do arquivo makefile
Programas: contém códigos de exemplo da linguagem I++
src: Contém o código fonte do projeto. O diretório bison contém o arquivo .y usado na criação da gramática. O diretório flex possui o arquivo .l que contém os tokens da linguagem. Já o arquivo include possui todos os arquivos headers das classes utilizadas como estrutura de dados no projeto. Todas elas foram programadas de modo inline.

## Instruções de Compilação

Para realizar-se a compilação do compilador, basta entrar no diretório raiz do projeto, que contém os códigos-fonte e o arquivo makefile e executar um make. O binário do computador irá ser criado no diretório bin.

Já para realizar a compilação de um código fonte com o compilador I++, deve-se seguir a seguinte sintaxe:

`i++ [codigo fonte] [arquivo de saida]`

sem a utilização dos caracteres '[' e ']'. Caso o código fonte não possua erros, o arquivo de saída será gerado bem como um log de compilacao na pasta onde o programa foi compilado. Do contrario uma mensagem de erro será mostrada.

## A linguagem

A linguagem I++ é uma linguagem simplista e minimalista baseada na lingua portuguesa. Tem por objetivo incentivar o aprendizado das técnicas básicas de programação por crianças.

Suas palavras reservadas são:
inteiro, string, real, e, ou, repita, vezes, se, senão, enquanto, escreva, leia e sair.

A linguagem possui apenas três tipos: numeros inteiros, numeros reais e valores de string

A linguagem possui as seguintes instruções:

	= repita n vezes{ bloco de código } 
	Instrução de repetição que repete o bloco de códigos durante um numero inteiro de iterações. n pode ser um valor numérico ou uma variável.

	= se (expressao) { bloco de código } senão { bloco de código }
	Instrução de desvio de fluxo. A presença do bloco senão, não é obrigatória.

	= enquanto(expressao) {}
	Instrução de repetição que repete o bloco de códigos até que a expressão dada se torne falsa.

	= escreva "" ou escreva ("")
	Escreve a mensagem passada como parametro no terminal. Pode-se realizar contatenação de variaveis e valores através do operador +

	= leia variavel
	Lê um dado do usuário e armazena o valor na variavel dada.

	= sair
	Termina a execução do programa.

Operadores matemáticos:
+, -, *, /

Operadores lógicos:
e, ou

Operadores relacionais:
==, !=, <, >, <=, >=

Mais informações sobre a linguagem pode ser obtida através dos exemplos do diretório Programas.





