/*
 * IMath.hpp
 *
 *  Created on: May 18, 2014
 *      Author: igoraraujo
 */

#ifndef IMATH_HPP_
#define IMATH_HPP_

#include "IBlock.hpp"
#include "ISourceErrorException.hpp"
#include <vector>
#include <cstdlib>
#include <boost/lexical_cast.hpp>
#include <boost/tokenizer.hpp>

/* Typedef para simplificação da definição do tipo tokenizer da biblioteca boost */
typedef boost::tokenizer<boost::char_separator<char>> Tokenizer;

/* Classe IMath, que analisa e verifica expressões matemáticas */
class IMath{
private:
	/* String que representa a expressão como um todo */
	std::string expression;

	/* Vetor de variáveis existentes na expressão */
	std::vector<std::string> varList;

	/* Variáveis de controle do tipo de saida do calculo */
	int mathType = TYPE_INTEGER;
	int outType = TYPE_REAL;

public:
	/* Possíveis tipos */
	const static int TYPE_INTEGER = 0;
	const static int TYPE_REAL = 1;
	const static int TYPE_STRING = 2;


public:

	/* Construtor do objeto IMath, a partir de uma expressão completa */
	IMath(std::string *newExpression){
		expression.clear();
		expression.push_back('(');
		expression.append(*newExpression);
		expression.push_back(')');
	}

	int getMathType(){
		return mathType;
	}

	void setOutType(std::string *type){
		if(*type == "inteiro"){
			outType = TYPE_INTEGER;
		}
	}

	/* Método estatico, de apoio que verifica se uma determinada string
	 * é um número ou não. Utiliza-se o método da biblioteca padrão
	 * stof(string, size_t)
	 */
	static bool isNumber(std::string numberStr){
		try{
			std::string::size_type sz;
			std::stof(numberStr, &sz);
			return true;
		}catch(std::invalid_argument& e){
			return false;
		}catch(std::out_of_range& ex){
			std::cout << "Erro ao avaliar string " << numberStr << std::endl;
			return false;
		}
	}

	/* Método estatico, de apoio que verifica se uma determinada string
	 * é um número INTEIRO ou não.
	 */
	static bool isInteger(std::string numberStr){
		if(isNumber(numberStr)){
			if(numberStr.find(".") != std::string::npos || numberStr.find("e") != std::string::npos ||
					numberStr.find("E") != std::string::npos)
				return false;

			return true;
		}

		return false;
	}

	/* Método que realiza a análise da expressão matemática em questão e retorna a expressão completa */
	std::string checkAndGetExpression(IBlock *ownerBlock, int myLine) {

		/* Utiliza-se um tokenizer que quebra a expressão matemática nos seus operadores */
		boost::char_separator<char> separator("+-*/() ");
		Tokenizer tokens(expression, separator);

		/* Para cada token, verificamos se exitem erros, como variáveis não declaradas
		 * ou valores String
		 */
		for(auto tokenIterator = tokens.begin(); tokenIterator != tokens.end(); tokenIterator++){
			/* Se o token atual NÃO for um NUMERO, deve ser uma variável */
			if(!isNumber(*tokenIterator)){
				/* Portanto obtemos a variavel */
				IData *data = ownerBlock->getLocalVariable(*tokenIterator);
				std::string errorMsg;

				/* Se não for uma variável, então é uma string ou a variável não foi declarada
				 * Então geramos o erro aproriado
				 */
				if(data == NULL){
					if(tokenIterator->find("\"") != std::string::npos)
						errorMsg = "Impossivel utilizar uma string em uma expressao matematica (valor " + *tokenIterator + ")";
					else
						errorMsg = "Variavel " + *tokenIterator + " nao declarada.";

					throw ISourceErrorException(errorMsg, myLine);
				}
				/* Se for uma variavel string, também geramos um erro.*/
				else if(data->getType() == IString::myType){
					errorMsg = "Impossivel usar uma string em uma expressao matematica (variavel " + *tokenIterator + ")" ;
					throw ISourceErrorException(errorMsg, myLine);
				}
				else if(data->getType() == IReal::myType)
					mathType = TYPE_REAL;
			}else if(!IMath::isInteger(*tokenIterator))
				mathType = TYPE_REAL;
		}

		if(outType == TYPE_INTEGER){
			expression.insert(0, "(int)");
		}

		return expression;
	}

};


#endif /* IMATH_HPP_ */
