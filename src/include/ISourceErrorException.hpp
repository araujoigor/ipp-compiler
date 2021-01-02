/*
 * ISourceErrorException.hpp
 *
 *  Created on: Jun 6, 2014
 *      Author: igoraraujo
 */

#ifndef ISOURCEERROREXCEPTION_HPP_
#define ISOURCEERROREXCEPTION_HPP_

#include <string>

/* Classe de exceção criada para tratar erros de código*/
class ISourceErrorException{

private:
	/* Possui uma mensagem de erro */
	std::string errorMessage;

	/* E a linha do erro */
	int line;

public:

	/* Cosntrutor que recebe seus atributos */
	ISourceErrorException(std::string err, int lineError){
		errorMessage = err;
		line = lineError;
	}

	/* Método que retorna o erro */
	std::string getErrorMessage(){
		return "Erro proximo a linha " + std::to_string(line) + ": " + errorMessage;
	}

};

#endif /* ISOURCEERROREXCEPTION_HPP_ */
