#ifndef __IDEFINITION_HPP__
#define __IDEFINITION_HPP__

#include "IStatement.hpp"
#include "IMath.hpp"
#include <vector>
#include <iostream>
#include <map>

class IData;

/* Define o método externo ao código, generateVariable, que é utilizado localmente */
extern IData* generateVariable(std::string *type, std::string *id);

/* Classe IDefinition, responsável por validade e gerar códigos referentes a
 * definição de variáveis.
 */
class IDefinition : public IStatement{
private:
	/* Uma definição ou multipla definição de variáveis possui
	 * sempre um tipo
	 */
	std::string type;

	/* Vetor de variáveis a serem definidas*/
	std::vector<IData*> vars;

	/* Mapeamento de definições que recebem como valor outras variáveis.
	 * A string a esquerda representa o nome da variável que será criada e a
	 * string a esquerda, o nome da variável que terá seu valor atribuido.
	 */
	std::map<std::string, std::string> varEqualsVarMapping;

	/* Mapeamento de definições que recebem como valor expressões matemáticas.
	 * A string a esquerda representa o nome da variável que será criada.
	 */
	std::map<std::string, IMath *> varEqualsMathMapping;


	/* Variável booleana que representa se, mediante a uma
	 * definição de bloco de código pai, deve-se registrar
	 * as variáveis criadas por essa atribuição.
	 * Essa variável é necessária, pois, como a árvore de análise
	 * é ascendente, quando a definição acontece,
	 * o bloco pai ainda não foi criado.
	 */
	bool scheduledVariableRegister = false;

	/* Método de inicialização, onde define-se o tipo da atribuição. */
	void init(std::string myType){
		type = myType;
	}

	/* Método privado utilizado para averiguação do tipo
	 * de dados em c++ referente aos tipos definidos
	 * na linguagem criada. */
	std::string getCppType() {
		if(type == "inteiro")
			return "int";
		else if(type == "real")
			return "double";
		else if(type == "string")
			return "std::string";
		else{
			std::string errorMsg = "Tipo de variavel mal definido.";
			throw ISourceErrorException(errorMsg, myLine);
		}
	}

public:

	/* Construtor. Recebe o tipo de definição e a linha da definição. */
	IDefinition(std::string* myType, int myline) : IStatement(myline){
		init(*myType);
	}

	/* Implementação do método da classe mãe getType */
	std::string* getType(){ return &type; }

	/* Método para a adição de variáveis na lista de variáveis a serem registradas e criadas
	 * Se existir um bloco pai, já realiza-se o registro da mesma no bloco pai. Do contrário
	 * agenda-se um registro assim que o bloco pai for definido.
	 * Esse método é utilizado para definições que tem como valor inicial um valor numérico ou de string.
	 */
	void addVar(IData *var){
		vars.push_back(var);

		if(ownerBlock == NULL)
			scheduledVariableRegister = true;
		else
			ownerBlock->registerLocalVariable(var, myLine);
	}

	/* Mesmo método acima, porém utilizado para definições que recebem como valor
	 * outra variável.
	 */
	void addVar(std::string *leftVar, std::string *rightVar){
		varEqualsVarMapping[*leftVar] = *rightVar;

		if(ownerBlock == NULL)
			scheduledVariableRegister = true;
		else
			ownerBlock->registerLocalVariable(generateVariable(&type, leftVar), myLine);
	}

	/* Mesmo método acima, porém utilizado para definições que recebem como valor
	 * uma expressão matemática.
	 */
	void addVar(std::string *leftValue, IMath *rightVar){
		varEqualsMathMapping[*leftValue] = rightVar;

		if(ownerBlock == NULL)
			scheduledVariableRegister = true;
		else
			ownerBlock->registerLocalVariable(generateVariable(&type, leftValue), myLine);
	}

	/* Definição do bloco pai. Caso existam variáveis a serem registradas, isso é
	 * realizado.
	 */
	void setOwnerBlock(IBlock *newOwnerBlock){
		IStatement::setOwnerBlock(newOwnerBlock);
		if(scheduledVariableRegister){
			for(ulong i = 0; i < vars.size(); i++){
				newOwnerBlock->registerLocalVariable(vars[i], myLine);
			}

			for(auto it = varEqualsVarMapping.begin(); it != varEqualsVarMapping.end(); ++it){
				newOwnerBlock->registerLocalVariable(generateVariable(&type, ((std::string *)&it->first)), myLine);
			}

			for(auto it = varEqualsMathMapping.begin(); it != varEqualsMathMapping.end(); ++it){
				newOwnerBlock->registerLocalVariable(generateVariable(&type, ((std::string *)&it->first)), myLine);
			}
		}
	}

	/* Método generateCode, para analise e criação do código intermediário. */
	void generateCode(std::fstream& fileStream) {
		int i;

		/* Primeiramente escreve-se o tipo da definição em questão */
		fileStream << getCppType() + " ";

		/*
		 * Primeiramente, processa-se as definições que recebem como valor um numero ou uma string.
		 */
		for(i = 0; i < (int)vars.size() - 1; i++){
			IData* actual = vars[i];

			/* Escreve-se o nome da variável */
			fileStream << actual->getId();

			/* E se ela possuir um valor, escreve-se esse valor. */
			if(actual->getHasValue()){
				fileStream << " = ";
				actual->printValue(fileStream);
			}

			/* Adiciona-se uma virgula, já que se trata de uma multipla definição */
			fileStream << ", ";
		}

		if(vars.size() > 0){
			IData* actual = vars[i];
			fileStream << vars[i]->getId();
			if(actual->getHasValue()){
				fileStream << " = ";
				actual->printValue(fileStream);
			}
			i++;
		}

		/*
		 * Checa-se então as variáveis que tem como valor outras variáveis.
		 */

		for(auto it = varEqualsVarMapping.begin(); it != varEqualsVarMapping.end(); ++it){
			/* Obtemos as variáveis a esquerda e a direita (que recebe e cede o valor, respectivamente) */
			IData *rightVar = ownerBlock->getLocalVariable(it->second);
			IData *leftVar = ownerBlock->getLocalVariable(it->first);

			/* Caso a variavél da direita não exista, gera-se um erro. */
			if(rightVar == NULL){
				std::string errorMsg = "Variavel " + it->second + " nao declarada.";
				throw ISourceErrorException(errorMsg, myLine);
			}

			/* Caso não haja compatibilidade de tipos, gera-se um erro */
			if(rightVar->getType() != type && !(type == "real" && rightVar->getType() == "inteiro") ){
				std::string errorMsg = "Impossivel atribuir '" + it->second + "' (" + rightVar->getType() + ") a '" + it->first + "' (" + type + "). Tipos incompativeis.";
				throw ISourceErrorException(errorMsg, myLine);
			}

			if(type == "inteiro" && rightVar->getType() == "inteiro"){
				((IInteger *)leftVar)->setValue(((IInteger *)rightVar)->getValue());
			}else if(type == "real" && rightVar->getType() == "inteiro"){
				((IReal *)leftVar)->setValue(((IInteger *)rightVar)->getValue());
			}else if(type == "real" && rightVar->getType() == "real"){
				((IReal *)leftVar)->setValue(((IReal *)rightVar)->getValue());
			}else{
				std::string errorMsg = "Impossivel atribuir '"+ it->second + "' (" + rightVar->getType() + ") a '" + it->first + "' (" + type + "). Tipos incompativeis.";
				throw ISourceErrorException(errorMsg, myLine);
			}

			/* Se houver mais de uma definição, adiciona-se uma virgula. */
			if(i > 0)
				fileStream << ", ";

			/* E escreve-se o nome das variaveis a esquerda e a direita */
			fileStream << it->first << " = " << it->second;
			i++;

		}

		/*
		 * Finalmente, checa-se então as variáveis que tem como valor uma expressãom matemática
		 */
		for(auto it = varEqualsMathMapping.begin(); it != varEqualsMathMapping.end(); ++it){
			int mathType;

			/* Se houver mais de uma definição, adicionamos uma virgula. */
			if(i > 0)
				fileStream << ", ";

			/* E escreve-se o nome da variável a direita e a expressão a esquerda. */
			fileStream << it->first << " = " << it->second->checkAndGetExpression(ownerBlock, myLine);

			/* Realiza-se a analise de compatibilidade de tipos */
			mathType = it->second->getMathType();

			if(mathType == IMath::TYPE_REAL && type == IInteger::myType){
				std::cout << "Atencao: usar uma variavel real em uma operacao para uma variavel inteira causa perda de dados. " << std::endl;
			}else if(type == "string"){
				std::string errorMsg = "Impossivel atribuir uma expressao matematica a variavel string " + it->first;
				throw ISourceErrorException(errorMsg, myLine);
			}
		}

		/* No final da atribuição, adicionamos um ; */
		fileStream << ";" << std::endl;
	}
};

#endif
