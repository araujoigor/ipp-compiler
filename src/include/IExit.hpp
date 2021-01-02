/*
 * IExit.hpp
 *
 *  Created on: Jun 10, 2014
 *      Author: igoraraujo
 */

#ifndef IEXIT_HPP_
#define IEXIT_HPP_

#include "IStatement.hpp"
#include <iostream>

/* Classe que processa comandos sair */
class IExit : public IStatement{
	public:

	IExit(int linenumber) : IStatement(linenumber){}

	/* A classe simplesmente adiciona um valor de retorno ao local em que aparece. */
	void generateCode(std::fstream& fileStream) {

		fileStream << "return 0;" << std::endl;

	}

};



#endif /* IEXIT_HPP_ */
