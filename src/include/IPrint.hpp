/*
 * IPrint.hpp
 *
 *  Created on: Jun 4, 2014
 *      Author: igoraraujo
 */

#ifndef IPRINT_HPP_
#define IPRINT_HPP_

#include "IStatement.hpp"

#include <boost/tokenizer.hpp>
#include <string>

using namespace std;

/* Typedef para simplificação da definição do tipo tokenizer da biblioteca boost */
typedef boost::tokenizer<boost::char_separator<char>> Tokenizer;

/* Classe utilizada para a analise de comandos leia*/
class IPrint : public IStatement{

private:
	/* Variavel que contem a expressão a ser escrita */
	string *expression;

public:

	/* Construtor, que recebe a expressão */
	IPrint(string *exp, int myline) : IStatement(myline){
		expression = exp;
	}

	virtual ~IPrint(){
		delete expression;
	}

	/* Método generateCode, para analise e criação do código intermediário. */
	void generateCode(std::fstream& fileStream) {

		/* O comando equivalente a escreva é o cout */
		fileStream << "cout";

		/* Quebramos a expressão usando o operador de concatenação '+' */
		boost::char_separator<char> separator("+");
		Tokenizer tokens(*expression, separator);

		/* Para cada token, procuramos por erros*/
		for(auto tokenIterator = tokens.begin(); tokenIterator != tokens.end(); tokenIterator++){

			/* Se o token não for um número nem uma string, é uma variável */
			if(!IMath::isNumber(*tokenIterator) && tokenIterator->find("\"") == string::npos){
				IData *var = ownerBlock->getLocalVariable(*tokenIterator);

				/* Se a variável não tiver sido definida, geramos um erro */
				if(var == NULL){
					std::string errorMsg = "Variavel " + *tokenIterator + " nao declarada.";
					throw ISourceErrorException(errorMsg, myLine);
				}

				/* Adicionamos o operador << para cada token, para concatenação dos valores. */
				fileStream << " << " << *tokenIterator;
			}else
				/* Adicionamos o operador << para cada token, para concatenação dos valores. */
				fileStream << " << " << *tokenIterator;
		}

		fileStream << ";" << endl;
	}
};

#endif /* IPRINT_HPP_ */
