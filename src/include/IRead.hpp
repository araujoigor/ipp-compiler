/*
 * IRead.hpp
 *
 *  Created on: Jun 4, 2014
 *      Author: igoraraujo
 */

#ifndef IREAD_HPP_
#define IREAD_HPP_

#include "IStatement.hpp"
#include <iostream>

using namespace std;

/* Classe utilizada para a analise de comandos leia*/
class IRead : public IStatement{

public:
	/* A variável leia deve OBRIGATORIAMENTE receber o nome de uma variável definida
	 * que guardamos aqui.
	 */
	string *varId;

	/* Construtor que recebe o nome da variável a receber o valor */
	IRead(string *var, int myline) : IStatement(myline){
		varId = var;
	}

	/* Destrutor */
	virtual ~IRead(){
		delete varId;
	}

	/* Método generateCode, para analise e criação do código intermediário. */
	void generateCode(std::fstream& fileStream) {

		/* Um leia é representado por um cin */
		fileStream << "cin >> ";

		/* Obtemos a variável especificada, a partir dos blocos "pai" */
		IData *data = ownerBlock->getLocalVariable(varId);

		/* Se a variável não existir, geramos um erro. */
		 if(data == NULL){
			 std::string errorMsg = "Variavel \'" + *varId + "\' nao declarada.";
			 throw ISourceErrorException(errorMsg, myLine);
		 }

		 fileStream << *varId << ";" << endl;

	}

};

#endif /* IREAD_HPP_ */
