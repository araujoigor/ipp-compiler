/*
 * IFlowControl.hpp
 *
 *  Created on: Jun 3, 2014
 *      Author: igoraraujo
 */

#ifndef IFLOWCONTROL_HPP_
#define IFLOWCONTROL_HPP_

#include "IStatement.hpp"

#include <string>
#include <boost/algorithm/string/regex.hpp>
#include <boost/algorithm/string.hpp>

using namespace std;

/* Classe utilizada para processamento de códigos IF/ELSE */
class IFlowControl : public IStatement{

private:
	/* Variável representando a expressão logica do if */
	string *expression;

	/* Bloco de códigos do IF */
	IBlock *ifBlock = NULL;

	/* Bloco de códigos do ELSE*/
	IBlock *elseBlock = NULL;


public:

	//Construtor padrão utilizado pelo BISON. Recebemos o bloco de codigo do if,
	// o bloco de codigo do else, a expressao e a linha onde foi declarado o if
	IFlowControl(IBlock* newIfBlock, IBlock* newElseBlock, string* exp, int myline) : IStatement(myline){
		ifBlock = newIfBlock;
		elseBlock = newElseBlock;
		expression = exp;

		if(ifBlock != NULL)
			ifBlock->setStatementOwner(this);

		if(elseBlock != NULL)
			elseBlock->setStatementOwner(this);
	}

	virtual ~IFlowControl(){
		delete expression;
		delete ifBlock;
		delete elseBlock;
	}

	//Utilizamos esse método para adição de um statement
	//em um if que nao possui um bloco
	void addIfSingleStatement(IStatement *stmt){
		if(ifBlock == NULL && stmt != NULL){
			ifBlock = new IBlock();
			ifBlock->addStatement(stmt);
			stmt->setOwnerBlock(ifBlock);
			ifBlock->setStatementOwner(this);
		}
	}

	//Utilizamos esse método para adição de um statement
	//em um if que nao possui um bloco
	void addElseSingleStatement(IStatement *stmt){
		if(elseBlock == NULL && stmt != NULL){
			elseBlock = new IBlock();
			elseBlock->addStatement(stmt);
			stmt->setOwnerBlock(elseBlock);
			elseBlock->setStatementOwner(this);
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
		boost::algorithm::split_regex(tokens, tempStr, boost::regex("&&|\\|\\|"));

		//Iteramos sobre as sub expressões para dividir os elementos da expressão e procurar por comparações
		//incorretas e/ou variaveis não declaradas
		for(auto it = tokens.begin(); it != tokens.end(); it++){
			//Vetor de strings para armazenamento das subexpressões
			vector<string> varsExps;

			//Quebramos cada subexpressão pelos operadores de comparação
			boost::algorithm::split_regex(varsExps, *it, boost::regex("==|!=|<|>|<=|>="));
			string type[2];
			int i = 0;

			//Obtemos o tipo de cada operando das comparações
			for(auto it2 = varsExps.begin(); it2 != varsExps.end(); it2++){
				//Se for um numeri=o
				if(IMath::isNumber(*it2)){
					type[i] = IReal::myType;
					continue;
				}

				//Se for uma string
				if(it2->find("\"") != string::npos){
					type[i] = IString::myType;
					continue;
				}

				IData *data = ownerBlock->getLocalVariable(*it2);

				//Se for uma variavel
				if(data == NULL){
					//Variavel nao declarada, gera um erro
					throw ISourceErrorException("Variavel " + *it2 + " nao declarada.", myLine);
				}else{
					type[i] = data->getType();
				}

				i++;
			}

			//Caso a comparação seja de tipagem errada, gera-se um erro
			if(type[0] != type[1] && (type[0] == IString::myType || type[1] == IString::myType)){
				throw ISourceErrorException("Impossivel comparar uma variavel numerica com uma string.", myLine);
			}
		}

		//Se tudo estiver certo, traduzimos o código
		fileStream << "if (" << *expression << "){" << endl;
		ifBlock->generateCode(fileStream);
		fileStream << "}" << endl;

		if(elseBlock != NULL){
			fileStream << "else{" << endl;
			elseBlock->generateCode(fileStream);
			fileStream << "}" << endl;
		}

	}

};




#endif /* IFLOWCONTROL_HPP_ */
