/*
 * IRepeat.hpp
 *
 *  Created on: Jun 4, 2014
 *      Author: igoraraujo
 */

#ifndef IREPEAT_HPP_
#define IREPEAT_HPP_

#include "IStatement.hpp"
#include "IMath.hpp"
#include <string>
#include <iostream>

using namespace std;

/* Classe utilizada para processamento do código REPITA */
class IRepeat : public IStatement{
private:
	/* Nome da variável utilizada como contador do bloco, se utilizado */
	string *varname = NULL;
	/* Valor utilizado no contador do bloco */
	int value = 0;

	/* Bloco interno do REPITA */
	IBlock *innerBlock;

	/* Variáveis que dizem que o valor utilizado no contador
	 * é um valor ou uma variável.
	 */
	bool isValue = false;
	bool isStrValue = false;

public:

	/* Construtor que cria um IRepeat, rcebendo
	 * o bloco interno e a variavel de contagem
	 */
	IRepeat(IBlock *block, string *var, int myline) : IStatement(myline){
		if(var == NULL)
			cout << "Erro: valor vazio para repeticao." << endl;

		innerBlock = block;

		varname = var;

		/* Se o valor for um numero, o repita se baseia em contadores numericos */
		if(IMath::isNumber(*varname)){
			if(IMath::isInteger(*varname))
				value = atoi(varname->c_str());
			else{
				/* Se for um valor real, mostramos o warning de arrendondamento */
				value = (int)atof(varname->c_str());
				//warning
				cout << "Atencao: a utilizacao de um valor real em uma expressao \'repita\' causa arredondamento para baixo." << endl;
			}

			isValue = true;
		}
		/* Se o valor for uma string, setamos os boleanos correspondentes */
		else if(varname->find("\"") != std::string::npos)
			isValue = isStrValue = true;

		if(innerBlock != NULL)
			innerBlock->setStatementOwner(this);
	}

	/* Método utilizado para adição de um unico statement para o bloco REPITA.
	 * Utilizado quando o bloco é de apenas um statement */
	void addSingleStatement(IStatement *stmt){
		if(innerBlock == NULL && stmt != NULL){
			innerBlock = new IBlock();
			innerBlock->addStatement(stmt);
			stmt->setOwnerBlock(innerBlock);
			innerBlock->setStatementOwner(this);
		}
	}

	void generateCode(std::fstream& fileStream) {
		/* Como o método repita se baseia no laço for, geramos um nome de variável
		 * aleatório para utilizar como contador
		 */
		string randomVarName = IBlock::generateRandomVariableName(15);
		randomVarName.insert(randomVarName.begin(), '_');

		/* Criamos o corpo do for */
		fileStream << "for(int " << randomVarName << " = 0; " << randomVarName << " < ";

		string comparer = std::to_string(value);

		/* Se o contador for uma variável, verificamos se a mesma existe e se ela é numérica */
		if(!isValue){
			IData *data = ownerBlock->getLocalVariable(varname);

			/* Se ela não existir, geramos um erro */
			if(data == NULL){
				std::string errorMsg = "Variavel " + *varname + " nao declarada.";
				throw ISourceErrorException(errorMsg, myLine);
			}

			string type = data->getType();

			/* Se o tipo for string, geramos um erro */
			if(type == IString::myType){
				std::string errorMsg = "Variavel string utilizada em laco \'repita\'.";
				throw ISourceErrorException(errorMsg, myLine);
			}else if(type == IReal::myType)
				/* Se for um valor real, mostramos o warning de arrendondamento */
				//warning
				cout << "Atencao: a utilizacao de um valor real em uma expressao \'repita\' causa arredondamento para cima (por exemplo, `repita 3.3 vezes`,  repetirá 4 vezes)." << endl;

			comparer = data->getId();
		}else if(isStrValue){
			/* Se for uma variável string, geramos um erro*/
			std::string errorMsg = "Variavel string utilizada em laco \'repita\'.";
			throw ISourceErrorException(errorMsg, myLine);
		}

		/* Terminamos de gerar o bloco */
		fileStream << comparer << "; " << randomVarName << "++){";
		innerBlock->generateCode(fileStream);
		fileStream << "}" << endl;
	}

};



#endif /* IREPEAT_HPP_ */
