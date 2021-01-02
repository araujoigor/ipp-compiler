/*
 * IWhile.hpp
 *
 *  Created on: Jun 4, 2014
 *      Author: igoraraujo
 */

#ifndef IWHILE_HPP_
#define IWHILE_HPP_

#include "IStatement.hpp"
#include <string>
#include <boost/regex.hpp>

using namespace std;

class IWhile: public IStatement {
private:
	//Variável que guarda a expressão
	string *expression;

	//Bloco de códigos do while
	IBlock *innerBlock;

public:
	//Construtor padrão utilizado pelo BISON. Recebemos o bloco de codigo do while,
	//a expressao e a linha onde foi declarado o while
	IWhile(IBlock *block, string *exp, int myline) :
			IStatement(myline) {
		innerBlock = block;
		expression = exp;

		if (innerBlock != NULL)
			innerBlock->setStatementOwner(this);
	}

	virtual ~IWhile() {
		delete expression;
		delete innerBlock;
	}

	//Utilizamos esse método para adição de um statement
	//em um while que nao possui um bloco
	void addSingleStatement(IStatement *stmt) {
		if (innerBlock == NULL && stmt != NULL) {
			innerBlock = new IBlock();
			innerBlock->addStatement(stmt);
			stmt->setOwnerBlock(innerBlock);
			innerBlock->setStatementOwner(this);
		}
	}

	//Método para geração do código
	void generateCode(std::fstream& fileStream) {
		//Variavel para armazenamento dos tokens que iremos gerar através da quebra
		//da expressao do if pelos tokens '&&' e '||'
		vector<string> tokens;

		//Armazenamos a expressao original temporariamente, para processamento
		string tempStr = *expression;

		//eliminamos todos os parenteses para analise
		boost::erase_all(tempStr, "(");
		boost::erase_all(tempStr, ")");

		//Quebramos a expressao pelos operadores || e &&
		boost::algorithm::split_regex(tokens, tempStr,
				boost::regex("&&|\\|\\|"));

		//Iteramos sobre as sub expressões para dividir os elementos da expressão e procurar por comparações
		//incorretas e/ou variaveis não declaradas
		for (auto it = tokens.begin(); it != tokens.end(); it++) {
			//Vetor de strings para armazenamento das subexpressões
			vector<string> varsExps;

			//Quebramos cada subexpressão pelos operadores de comparação
			boost::algorithm::split_regex(varsExps, *it,
					boost::regex("==|!=|<|>|<=|>="));
			string type[2];
			int i = 0;

			//Obtemos o tipo de cada operando das comparações
			for (auto it2 = varsExps.begin(); it2 != varsExps.end(); it2++) {
				//Se for um numeri=o
				if (IMath::isNumber(*it2)) {
					type[i] = IReal::myType;
					continue;
				}

				//Se for uma string
				if (it2->find("\"") != string::npos) {
					type[i] = IString::myType;
					continue;
				}

				IData *data = ownerBlock->getLocalVariable(*it2);

				//Se for uma variavel
				if (data == NULL) {
					//Variavel nao declarada, gera um erro
					throw ISourceErrorException(
							"Variavel " + *it2 + " nao declarada.", myLine);
				} else {
					type[0] = data->getType();
				}

				i++;
			}

			//Caso a comparação seja de tipagem errada, gera-se um erro
			if (type[0] != type[1]
					&& (type[0] == IString::myType || type[1] == IString::myType)) {
				throw ISourceErrorException(
						"Impossivel comparar uma variavel numerica com uma string.",
						myLine);
			}
		}

		fileStream << "while (" << *expression << "){" << endl;
		innerBlock->generateCode(fileStream);
		fileStream << "}" << endl;
	}
};

#endif /* IWHILE_HPP_ */
