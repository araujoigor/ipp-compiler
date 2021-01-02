%{
	#define YYDEBUG 1
	/* 
	 * Realiza-se a inclusão dos arquivos que contém as classes
	 * utilizadas como estrutura de dados e como base para análise
	 * gramatical do código.
	 */
	#include "include/IString.hpp"
	#include "include/IReal.hpp"
	#include "include/IInteger.hpp"
	#include "include/IAssignment.hpp"
	#include "include/IDefinition.hpp"
	#include "include/IFlowControl.hpp"
	#include "include/IMath.hpp"
	#include "include/IWhile.hpp"
	#include "include/IRepeat.hpp"
	#include "include/IPrint.hpp"
	#include "include/IRead.hpp"
	#include "include/IExit.hpp"
	
	/* Inclusão das bibliotecas padrões utilizadas neste arquivo de código */
	#include <typeinfo>
	#include <cstdlib>
	#include <cstdio>
	#include <unistd.h>
	#include <sys/wait.h>
	#include <sysexits.h>
	
	/* Declaração das variáveis e funções externas a esse código
	 * mas que são utilizadas por ele.
	 * 
	 * int yylex() retorna o tipo do ultimo token encontrado.
	 *
	 * int yyparse() realiza a análise do token atual
	 *
	 * FILE *yyin representa o descritor do arquivo de entrada.
	 *
	 * int linenumber representa a variável que armazena a linha de código
	 * atual (definida no arquivo .l).
	 */
	extern "C" int yylex();
	extern "C" FILE *yyin;
	extern "C" int linenumber;
	
	/* 
	 * Definição da função chamada quando de um erro de análise gramátical, no que 
	 * diz respeito ao fato de que o código análisado não respeita nenhuma regra gramátical
	 * definida abaixo. 
	 * A implementação simplesmente indica um erro e a linha em que o erro ocorreu. 
	 */
	void yyerror(const char *s) {
		std::cout << "Erro de sintaxe na linha " << linenumber << std::endl << s << std::endl;
		exit(EX_DATAERR);
	}
	
	/* 
	 * Definição do valores estáticos constantes das classes IString, IReal e IInteger.
	 * Essas variáveis representam o tipo do dado armazenado pela variável.
	 * Mais informações na classe IData. 
	 */
	const std::string IString::myType = "string";
	const std::string IReal::myType = "real";
	const std::string IInteger::myType = "inteiro";
	
	/*
	 * Definição da stream de dados utilizada para gerar o log de compilação.
	 */	
	std::fstream compilationLog("compilationLog", std::ios::out);
	
	/*
	 * Definição dos métodos de criação de objetos que representam variáveis
	 * (generateVariable) a partir de tokens reconhecidos pela gramática de definição 
	 * de variáveis (ver região de definiçao de regras gramáticais).
	 * A idéida desses métodos é gerar o tipo de dados correto para armazenamento
	 * de valores, a partir do token TIPO e ID (e possivelmente VALUE) recebidos
	 * através de uma das regras gramáticais de definição de variável.
	 */
	
	/*
	 * Método generateVariable que recebe como parâmetro uma string representando
	 * o tipo de variável e uma string que representa o nome da variável.
	 * Este método simplesmente compara o tipo recebido com os possíveis tipos
	 * e gera uma classe filha de IData, como o nome passado como parâmetro.
	 * Caso o tipo não seja um tipo esperado (o que é impossível, devido às regras
	 * lexicas criadas no arquivo .l), um erro é gerado.
	 */
	IData* generateVariable(std::string *type, std::string *id){ 
		if(*type == "inteiro") return new IInteger(*id);
		else if(*type == "string") return new IString(*id);
		else if(*type == "real") return new IReal(*id);

		std::cout << "Erro ao definir variavel. Tipo desconhecido. (generateVariable(string*, string*))" << std::endl;
		return NULL;
	}
	
	/*
	 * Método generateVariable que recebe como parâmetro uma string representando
	 * o tipo de variável, uma string que representa o nome da variável e uma string
	 * que representa o valor inicial da variável.
	 * Este método simplesmente compara o tipo recebido com os possíveis tipos
	 * e garante que o tipo recebido é do tipo esperado (através do auxílio do método isNumber
	 * da classe IMath (ver classe IMath)) e  gera uma classe filha de IData, como o nome 
	 * passado como parâmetro e com o valor recebido.
	 * Caso o tipo não seja um tipo esperado (o que é impossível, devido às regras
	 * lexicas criadas no arquivo .l), um erro é gerado.
	 */
	IData* generateVariable(std::string *type, std::string *id, std::string *value){ 
		bool isNumValue = IMath::isNumber(*value);
	
		if(*type == "inteiro") {
			if(isNumValue)
				return new IInteger(*id, atoi(value->c_str()));
			else{
				std::cout << "Impossivel atribuir um valor nao numerico a uma variavel numerica. Abortando..." << std::endl;
				return new IInteger(*id);
			}
		}
		else if(*type == "real"){
			if(isNumValue)
				return new IReal(*id, atof(value->c_str()));
			else{
				std::cout << "Impossivel atribuir um valor nao numerico a uma variavel numerica. Abortando..." << std::endl;
				return new IReal(*id);
			}
				
		}
		else if(*type == "string"){ 
			if(!isNumValue)
				return new IString(*id, value->substr(1,value->size()-2));
			else{
				std::cout << "Impossivel atribuir um valor numerico a uma variavel string. Abortando..." << std::endl;
				return new IReal(*id);
			}
		}

		std::cout << "Erro ao definir variavel. Tipo desconhecido. (generateVariable(string*, string*, string*))" << std::endl;
		std::cout << "Parametros: type = " << *type << ", id = " << *id << ", value = " << *value << std::endl;
		return NULL;
	}
	
	/* 
	 * Definição das variáveis IBlock que representam, respectivamente
	 * o ultimo bloco de código gerado pelas regras gramáticais (ou seja, o bloco
	 * de código que está sendo analisado no momento) e o bloco de código que representa
	 * a raiz da arvore de blocos do programa analisado.
	 */
	IBlock *lastBlock, *programBlock;
%}

// Turns on detailed error reporting
%define parse.error verbose

/* 
 * Diz ao Bison que ele deve incluir os seguintes arquivos 
 * no seu código fonte gerado. Evita referência indefinidas
 * durante a compilação do código do Bison.
 */
%code requires { #include "include/IString.hpp" }
%code requires { #include "include/IReal.hpp" }
%code requires { #include "include/IInteger.hpp" }
%code requires { #include "include/IAssignment.hpp" }
%code requires { #include "include/IDefinition.hpp" }
%code requires { #include "include/IMath.hpp" }
%code requires { #include "include/IFlowControl.hpp" }
%code requires { #include "include/IWhile.hpp" }
%code requires { #include "include/IRepeat.hpp" }
%code requires { #include "include/IPrint.hpp" }
%code requires { #include "include/IRead.hpp" }
%code requires { #include "include/IExit.hpp" }

/* Definição da União utilizada pelo Bison para armazenar dados
 * referentes a tokens e a regras gramáticais.
 * Os campos dessa união são principalmente utilizados
 * no mapeamento dos tipos de dados utilizados nas regras
 * gramáticais definidas (utilização dos operadores $$, $1, 
 * $2, etc...) 
 */
%union {
	/* 
	 * Tipos de dados utilizados SOMENTE
	 * para armazenamento de dados nas
	 * regras gramáticais
	 */
	IBlock *block;
	IAssignment *attr;
	IString *varchar;
	IInteger *varint;
	IReal *varreal;
	IDefinition *def;
	IStatement *stmt;
	IFlowControl *ifstmt;
	IWhile *whilestmt;
	IRepeat *repeat;
	IPrint *print;
	IRead *read;
	IExit *exit;
	
	/* 
	 * Tipos de dados utilizados SOMENTE
	 * para armazenamento de tokens
	 */
	std::string *string;
	int token;
}

/* 
 * Mapeamento dos tokens existentes para cada um dos campos 
 * cuja resposabilidade é armazenar tokens (string ou token).
 */
%token <string> TYPE IFSTMT SENAO ID NUM_INTEGER NUM_REAL CHAR VALUE COMPARISON OR EXIT
%token <string> AND LPAREN RPAREN WHILE REPEAT REPEAT_END PRINT PRINT_P READ READ_P
%token <token> EQUAL TLBRACE TRBRACE COMA PLUS MINUS TIMES DIVIDE

/* 
 * Mapeamento das regras gramáticais criadas para cada um dos
 * campos cuja resposabilidade é armazenar valores referentes
 * a cada uma das regras criadas.
 */
%type <block> block program
%type <stmt> stmt
%type <def> def
%type <attr> attr
%type <ifstmt> ifstmt
%type <whilestmt> whilestmt
%type <string> math
%type <string> exp
%type <repeat> repeatstmt
%type <print> printstmt
%type <string> printexp
%type <read> readstmt
%type <exit> exitstmt 

/* Definição das precedências de operadores */
%left PLUS MINUS
%left TIMES DIVIDE
%left LPAREN RPAREN

/* 
 * Definição da regra gramátical a ser buscada no inicio 
 * do processo de análise.
 */
%start program

%%
			/* Início do trecho de definição de regras gramáticais */
			
			/* 
			 * Regra inicial de gramática. Simplesmente armazena o bloco de código
			 * raíz do código fonte na variável previamente criada para isto.
			 * Esta regra é acessada apenas uma vez, no inicio do processo de 
			 * análise.
			 */
program : 	block { programBlock = $1; };

			/* 
			 * Regra gramátical responsável por criar um novo bloco de código.
			 * Um bloco de código é representado por um ou mais statements.
			 * Quando um bloco é iniciado e o primeiro statement é definido,
			 * um bloco de código é criado. Todos os statements posteriores
			 * são adicionados ao bloco previamente criado.
			 *
			 * Esta regra é utilizada sempre que um novo bloco de código é criado,
			 * por exemplo, dentro de uma regra IFSTMT.
			 */
block 	:	stmt { $$ = new IBlock(); $1->setOwnerBlock($$); $$->addStatement($1); lastBlock = $$; } | 
			block stmt { $2->setOwnerBlock($1); $1->addStatement($2); };
			
			/*
			 * Regra que define todos os tipos de statement passíveis de
			 * reconhecimento. A regra específica do statement em questão
			 * é chamada e definida como raíz da subárvore atual. Além 
			 * disso, utiliza-se esta regra para a geração do arquivo de
			 * log.
			 */	
stmt 	:	def { $$ = $1; compilationLog << "Proximo a linha: " << linenumber << " definicao de variavel" << std::endl;} | 
			attr { $$ = $1; compilationLog << "Proximo a linha: " << linenumber << " atribuicao de valor a uma variavel" << std::endl;} | 
			ifstmt { $$ = $1; compilationLog << "Proximo a linha: " << linenumber << " comando de desvio de fluxo (se)" << std::endl;} | 
			whilestmt{$$=$1; compilationLog << "Proximo a linha: " << linenumber << " comando de repeticao (enquanto)" << std::endl;} | 
			repeatstmt { $$ = $1; compilationLog << "Proximo a linha: " << linenumber << " comando de repeticao (repita)" << std::endl;} | 
			printstmt{ $$ = $1; compilationLog << "Proximo a linha: " << linenumber << " comando de saida (escreva)" << std::endl;} | 
			readstmt { $$ = $1; compilationLog << "Proximo a linha: " << linenumber << " comando de entrada (leia)" << std::endl;} | 
			exitstmt{$$=$1; compilationLog << "Proximo a linha: " << linenumber << " comando de finalizacao (sair)" << std::endl;} ;

			/*
			 * Regra de definição de variáveis. A declaração pode ser
			 * de apenas uma variável, sem valor inicial ou com
			 * valor inicial (que pode ser uma expressão matemática,
			 * um valor ou uma outra variável, previamente definida.
			 * Além disso, a regra pode verificar multiplas definições,
			 * sempre separadas por virgula. Porém as definições devem
			 * ser sempre do mesmo tipo.
			 */
def		: 	TYPE ID { $$ = new IDefinition($1, linenumber);  $$->addVar(generateVariable($1, $2)); } | 
			TYPE ID EQUAL VALUE { $$ = new IDefinition($1, linenumber);  $$->addVar(generateVariable($1, $2, $4)); } | 
			TYPE ID EQUAL ID { $$ = new IDefinition($1, linenumber);  $$->addVar($2, $4); } | 
			TYPE ID EQUAL math { $$ = new IDefinition($1, linenumber); IMath *math = new IMath($4); math->setOutType($$->getType()); $$->addVar($2, math); } | 
			def COMA ID { $1->addVar(generateVariable($1->getType(), $3)); } | 
			def COMA ID EQUAL VALUE { $1->addVar(generateVariable($1->getType(), $3, $5)); } | 
			def COMA ID EQUAL ID { $1->addVar($3, $5); } |
			def COMA ID EQUAL math { IMath *math = new IMath($5); math->setOutType($1->getType()); $1->addVar($3, math); };
			
			/*
			 * Regra de atribuição de valores a uma variável pré-definida.
			 * A atribuição pode acontecer mediante a um valor, o nome de uma
			 * outra variável pré-definida ou a uma expressão matemática.
			 */
attr 	: 	ID EQUAL math { IMath *math = new IMath($3); $$ = new IAssignment($1, math, linenumber); } |
			ID EQUAL ID { $$ = new IAssignment($1, $3, linenumber); } | 
			ID EQUAL VALUE { if(IMath::isInteger(*$3)) $$ = new IAssignment($1, atoi($3->c_str()), linenumber); else if(IMath::isNumber(*$3)) $$ = new IAssignment($1, atof($3->c_str()), linenumber); else $$ = new IAssignment($1, $3->substr(1,$3->size()-2), linenumber); };

			/*
			 * Regra de definição de uma expressão matemática.
			 * Os tokens terminais são sempre um valor ou o nome de uma variável.
			 * A regra utiliza-se dos operadores binários matemáticos para 
			 * concatenação e criação da expressão matemática completa.
			 */
math	:	ID | VALUE |
			LPAREN math RPAREN { $2->insert($2->begin(),'('); $2->push_back(')'); $$ = $2;}|
			math PLUS math { $1->push_back('+'); $1->append(*$3); $$ = $1; } |
			math MINUS math { $1->push_back('-'); $1->append(*$3); $$ = $1;} |
			math TIMES math { $1->push_back('*'); $1->append(*$3); $$ = $1;} |
			math DIVIDE math { $1->push_back('/'); $1->append(*$3); $$ = $1;}; 
			
			/*
			 * Regra de processamento de expressão lógica.
			 * Os tokens terminais, novamente, são valores ou nomes de variáveis.
			 * A regra utiliza-se dos operadores binários logicos para concatenação
			 * e criação da expressão lógica completa.
			 */
exp 	: 	ID | VALUE | 
			LPAREN exp RPAREN { $2->insert($2->begin(), '('); $2->push_back(')'); $$ = $2; } | 
			exp OR exp { $1->append("||"); $1->append(*$3); $$ = $1; } | 
			exp AND exp { $1->append("&&"); $1->append(*$3); $$ = $1; } |
			exp COMPARISON exp { $1->append(*$2); $1->append(*$3); $$ = $1;  }; 
			
			/*
			 * Definição da gramática de análise de comandos 
			 * de controle de fluxo (if).
			 * A definição do comando if sempre possui uma expressão
			 * lógica entre parenteses, e um bloco código a ser executado.
			 * Opcionalmente, pode existir um bloco de código alternativo,
			 * representado pelo token 'senão'.
			 */
ifstmt	:	IFSTMT exp RPAREN TLBRACE block TRBRACE { $$ = new IFlowControl($5, NULL, $2, linenumber); }| 
			IFSTMT exp RPAREN TLBRACE block TRBRACE SENAO TLBRACE block TRBRACE { $$ = new IFlowControl($5, $9, $2, linenumber); }|
			IFSTMT exp RPAREN TLBRACE block TRBRACE SENAO stmt { $$ = new IFlowControl($5, NULL, $2, linenumber); $$->addElseSingleStatement($8); } |
			IFSTMT exp RPAREN stmt { $$ = new IFlowControl(NULL, NULL, $2, linenumber); $$->addIfSingleStatement($4); } |
			IFSTMT exp RPAREN stmt SENAO TLBRACE block TRBRACE { $$ = new IFlowControl(NULL, $7, $2, linenumber); $$->addIfSingleStatement($4); } |
			IFSTMT exp RPAREN stmt SENAO stmt {$$ = new IFlowControl(NULL, NULL, $2, linenumber); $$->addIfSingleStatement($4); $$->addElseSingleStatement($6);};

			/*
			 * Regra de análise do comando while. 
			 * Um comando while deve possuir o token de inicio de
			 * comando while ( WHILE ), uma expressão lógica, entre parênteses
			 * e um bloco de código a ser executado.
			 */
whilestmt:	WHILE exp RPAREN stmt { $$ = new IWhile(NULL, $2, linenumber); $$->addSingleStatement($4); } |
			WHILE exp RPAREN TLBRACE block TRBRACE { $$ = new IWhile($5, $2, linenumber); };
			
			/*
			 * Regra de análise do comando repita (REPEAT).
			 * Um comando REPEAT deve obrigatóriamente conter
			 * um valor ou o nome de uma variável entre os
			 * tokens de inicio (REPEAT) e término (REPEAT_END)
			 * de comando repita e um bloco de código
			 * a ser executado.
			 */
repeatstmt:	REPEAT ID REPEAT_END stmt { $$ = new IRepeat(NULL, $2, linenumber); $$->addSingleStatement($4);}|
			REPEAT ID REPEAT_END TLBRACE block TRBRACE {$$ = new IRepeat($5, $2, linenumber);} | 
			REPEAT VALUE REPEAT_END stmt {$$ = new IRepeat(NULL, $2, linenumber); $$->addSingleStatement($4);} |
			REPEAT VALUE REPEAT_END TLBRACE block TRBRACE {$$ = new IRepeat($5, $2, linenumber);};

			/*
			 * Regra gramátical para concatenação de variáveis e strings 
			 * para uso no comando PRINTSTMT. A concatenação é feita
			 * através do token PLUS (+)
			 */
printexp:	VALUE {$$ = $1;} | ID {$$ = $1;} | printexp PLUS printexp { $$ = $1; $$->push_back('+'); $$->append(*$3);};

			/*
			 * Regra de análise do comando PRINT.
			 * Um comando PRINT possui uma expressão 
			 * definida pela regra 'printexp' entre parenteses 
			 * ou não.
			 */
printstmt:	PRINT_P printexp RPAREN { $$ = new IPrint($2, linenumber); } |
			PRINT printexp { $$ = new IPrint($2, linenumber); };	
			
			/*
			 * Regra de análise do comando READ.
			 * Um comando READ possui uma expressão 
			 * definida pelo nome de uma variável 
			 * precedida pelo token de inicio do comando
			 * READ. O nome da variável pode ou não, estar
			 * entre parênteses.
			 */
readstmt: 	READ_P ID RPAREN { $$ = new IRead($2, linenumber); } |
			READ ID { $$ = new IRead($2, linenumber); };
			
			/*
			 * Regra de análise do comando EXIT.
			 * Um comando EXIT possui apenas o token
			 * do comando EXIT.
			 */			
exitstmt:	EXIT { $$ = new IExit(linenumber); };
%% 
 
/* Método principal do programa */
int main(int argc, char **argv) {
	bool log = true;
	
	/* O compilador obrigatóriamente recebe três parâmetros:
	 * o nome do programa, o arquivo de entrada e o arquivo de
	 * saida 
	 */
	if(argc != 3){
		std::cout << "Usage: i++ [source file] [output file]" << std::endl;
		return EX_USAGE;
	}
	
	/* Cria-se o descritor de arquivo do arquivo de entrada */
	FILE *myfile = fopen(argv[1], "r");
	if (!myfile) {
		std::cout << "Impossível abrir o arquivo de entrada" << std::endl;
		return EX_NOINPUT;
	}

	/* Atribui-se o descritor do arquivo de entrada para
	 * a variável que representa o arquivo a ser analisado
	 * pelo FLEX.
	 */
	yyin = myfile;
	
	if(!compilationLog){
		std::cout << "Erro ao gerar log de compilacao" << std::endl;
		log  = false;
	}
	
	/* Inicia-se o processo de análise */
	do {
		yyparse();
	} while (!feof(yyin));
	
	/* 
	 * Realiza-se a criação do arquivo intermediário
	 * de compilação
	 */
	std::string tempCppCode;
	tempCppCode.append(argv[1]);
	tempCppCode.append(".cpp");
	
	/* 
	 * Realiza-se uma chamada ao método createCode
	 * do bloco raíz do código. Se tudo correu
	 * bem, inicia-se a thread de compilação.
	 */
	bool compilationSuccess = programBlock->createCode(tempCppCode);
	if(compilationSuccess){	
		//Realiza-se um fork no processo atual.
		pid_t pid = fork();
		int retVal;
		
		//A thread filha deve executar o g++, através da chamada de sistema execv.
		if(pid == 0){
			char *args[] = { (char *)"/usr/bin/g++", (char *)tempCppCode.c_str(), (char *)"-o", argv[2], 0 };		
			execv(args[0], args);
		}
		
		//O pai deve esperar pelo término da execução do filho.
		wait(&retVal);
	}
	
	//Fecha-se o arquivo de log.
	if(log){
		compilationLog.close();
	}
	
	//Deleta-se o arquivo intermediário de compilação
	remove(tempCppCode.c_str());

	if(!compilationSuccess) return EX_DATAERR;

	return EX_OK;
}
