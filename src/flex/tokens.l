%{
#include <string>
#include "parser.hpp"
#include "include/IString.hpp"
#include "include/IReal.hpp"
#include "include/IInteger.hpp"
#include "include/IAssignment.hpp"
#include "include/IDefinition.hpp"
#include "include/IMath.hpp"

/* 
 * Aqui, definde-se uma macro para realizar a passagem do ultimo TOKEN encontrado
 * para a estrutura de União definida no arquivo .y (Bison). Essa macro é responsável
 * por salvar os tokens do tipo string no campo string da União citada. 
 */
#define SAVE_TOKEN yylval.string = new std::string(yytext, yyleng)

/* 
 * Aqui, definde-se uma macro para realizar a passagem do ultimo TOKEN encontrado
 * para a estrutura de União definida no arquivo .y (Bison). Essa macro é responsável
 * por salvar os tokens do tipo int no campo int da União citada. 
 */
#define TOKEN(t) (yylval.token = t)

// We define yywrap to always return 1 since we never compile multiple files (to prevent linking agains fl)
extern "C" int yywrap() { return 1; };
extern "C" int yylex();

/*
 * Define-se um contador para verificação de qual linha
 * de código estamos verificando no momento.
 * esse valor é utilizado para impressão das mensagens
 * de erro
 */
int linenumber = 1;
%}

/* Regra de definição de um valor numérico inteiro. */
INT			("+"|"-")*[0-9]+

%%

									/* 
									 * Tiram-se todas as tabulações e espaços em branco
									 * do código fonte, sem realizar nenhuma ação. 
									 */
[ \t] 								;

									/*
									 * Retiram-se as quebras de linha do código fonte,
									 * incrementando o número de linhas de código para
									 * cada \n encontrado.
									 */
[\n]								{ ++linenumber; }

									/* Definem-se os tokens representados por TYPE, 
									 * que representam os possíveis tipos de váriaveis a 
									 * serem declaradas. Mediante um encontro, salva-se o 
									 * token encontrado, e retorna-se o tipo do token (TYPE).
									 */
"inteiro"|"string"|"real"			{ SAVE_TOKEN; return TYPE;}

									/* Define-se o token representado por AND, 
									 * que representa o operador lógico 'e' (&&). 
									 * Mediante um encontro, salva-se o token encontrado, e 
									 * retorna-se o tipo do token (AND).
									 */
" e "								{ SAVE_TOKEN; return AND; }

									/* Define-se o token representado por OR, 
									 * que representa o operador lógico 'ou' (||). 
									 * Mediante um encontro, salva-se o token encontrado, e 
									 * retorna-se o tipo do token (OR).
									 */
" ou "								{ SAVE_TOKEN; return OR; }

									/* Define-se o token representado por REPEAT, 
									 * que representa o início da declaração da instrução 'repita'. 
									 * Mediante um encontro, salva-se o token encontrado, e 
									 * retorna-se o tipo do token (REPEAT).
									 */
"repita"							{ SAVE_TOKEN; return REPEAT; }

									/* Define-se o token representado por REPEAT_END, 
									 * que representa o término da declaração da instrução 'repita'. 
									 * Mediante um encontro, salva-se o token encontrado, e 
									 * retorna-se o tipo do token (REPEAT_END).
									 */
"vezes"								{ SAVE_TOKEN; return REPEAT_END; }

									/* Definem-se os tokens representados por IFSTMT, 
									 * que representam o início da declaração da instrução 'se' (if). 
									 * Mediante um encontro, salva-se o token encontrado, e 
									 * retorna-se o tipo do token (IFSTMT).
									 */
"se ("|"se("						{ SAVE_TOKEN; return IFSTMT; }

									/* Define-se o token representado por SENAO, 
									 * que representa o início da declaração da instrução 'senão' (else). 
									 * Mediante um encontro, salva-se o token encontrado, e 
									 * retorna-se o tipo do token (SENAO).
									 */
"senão"								{ SAVE_TOKEN; return SENAO; }

									/* Definem-se os tokens representados por WHILE, 
									 * que representam o início da declaração da instrução 'enquanto' (while). 
									 * Mediante um encontro, salva-se o token encontrado, e 
									 * retorna-se o tipo do token (WHILE).
									 */
"enquanto("|"enquanto ("			{ SAVE_TOKEN; return WHILE; }

									/* Define-se o token representado por PRINT, 
									 * que representa o início da declaração da instrução 'escreva' 
									 * (printf ou cout). 
									 * Mediante um encontro, salva-se o token encontrado, e 
									 * retorna-se o tipo do token (PRINT).
									 */
"escreva"							{ SAVE_TOKEN; return PRINT; }

									/* Define-se o token representado por PRINT_P, 
									 * que é idêntico ao explicado acima, porém com conteúdo 
									 * definido entre parênteses. 
									 * Mediante um encontro, salva-se o token encontrado, e 
									 * retorna-se o tipo do token (PRINT_P).
									 */
"escreva("|"escreva ("				{ SAVE_TOKEN; return PRINT_P; }

									/* Define-se o token representado por READ, 
									 * que representa o início da declaração da instrução 'leia' 
									 * (scanf ou cin). 
									 * Mediante um encontro, salva-se o token encontrado, e 
									 * retorna-se o tipo do token (READ).
									 */
"leia"								{ SAVE_TOKEN; return READ; }

									/* Define-se o token representado por READ_P, 
									 * que é idêntico ao explicado acima, porém com conteúdo 
									 * definido entre parênteses. 
									 * Mediante um encontro, salva-se o token encontrado, e 
									 * retorna-se o tipo do token (READ_P).
									 */
"leia("|"leia ("					{ SAVE_TOKEN; return READ_P;}

									/* Define-se o token representado por EXIT, 
									 * que representa o início da declaração da instrução 'sair' 
									 * (exit()). 
									 * Mediante um encontro, salva-se o token encontrado, e 
									 * retorna-se o tipo do token (EXIT).
									 */
"sair"								{ SAVE_TOKEN; return EXIT; }


									/* 
									 * Definem-se as regras para declaração de valores. 
									 * São definidos valores inteiros ( {INT} ), valores de ponto fixo
									 * ( {INT}"."[0-9] ) e valores string ( \"([^"'])*\"|\'([^"'])*\' ).
									 * 
									 * Os valores String, que possuem regra mais complicada, são 
									 * valores existentes entre aspas duplas ou simples, que possuam qualquer
									 * caracter com exceção das aspas duplas e simples.
									 * 
									 * Valores inteiros são representados pela regra INT, definida anteriormente.
									 *
									 * Valores de ponto fixo são uma concatenação de um valor inteiro, um ponto 
									 * (.) e uma repetição de zero ou mais valores numéricos.
									 *
									 * Mediante um encontro de tal padrões, salva-se o token encontrado, e 
									 * retorna-se o tipo do token (VALUE).
									 */
{INT}|{INT}"."[0-9]*|\"([^"'])*\"|\'([^"'])*\'	{ SAVE_TOKEN; return VALUE; }

									/* 
									 * Definem-se os tokens representados por COMPARISON, 
									 * que representam operadores de comparação lógica de valores. 
									 * Mediante um encontro, salva-se o token encontrado, e 
									 * retorna-se o tipo do token (COMPARISON).
									 */
==|!=|<|>|<=|>=						{ SAVE_TOKEN; return COMPARISON; }

									/* 
									 * Define-se a regra para declaração de nomes de variáveis. 
									 * Elas devem iniciar com uma letra qualquer e contér, após isso,
									 * uma ou mais repetições de caracteres alfanuméricos. 
									 * Mediante um encontro de tal padrão, salva-se o token encontrado, e 
									 * retorna-se o tipo do token (ID).
									 */
[a-zA-Z_][a-zA-Z0-9_]*	 			{ SAVE_TOKEN; return ID; }

				/* 
				 * Todas as declarações abaixo, definem tokens de apenas um caracter
				 * e que portanto são salvos no campo token (int) da União definida
				 * no arquivo .y (Bison). Elas representam operadores matemáticos,
				 * (+,-,*,/), atribuição (=), de precedência de bloco de código 
				 * ( '(', ')' ), de inicio e término de bloco e código ( {, } )
				 * e de auxílio de multiplas definições (,).
				 */
"+"				{ return TOKEN(PLUS); }
"-"				{ return TOKEN(MINUS); }
"*"				{ return TOKEN(TIMES); }
"/"				{ return TOKEN(DIVIDE); }
"="				{ return TOKEN(EQUAL); }
"("             { SAVE_TOKEN; return LPAREN; }
")"             { SAVE_TOKEN; return RPAREN; }
"{"				{ return TOKEN(TLBRACE); }
"}"				{ return TOKEN(TRBRACE); }
","				{ return TOKEN(COMA); }
