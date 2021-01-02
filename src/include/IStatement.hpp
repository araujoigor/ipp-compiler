#ifndef __ISTATEMENT_HPP__
#define __ISTATEMENT_HPP__

#include "CodeGenerator.hpp"

class IBlock;

/* Classe abstrata responsável por definir métodos gerais
 * para todos os statements possiveis
 */
class IStatement : CodeGenerator{

protected:
	/* Todo statement possui a linha de código em que foi definida. */
	int myLine;

	/* E um bloco pai */
	IBlock* ownerBlock;
public:
	/* A classe IStatement é filha da classe CodeGenerator e portanto possui o método (aqui abstrato)
	 * generateCode
	 */
	virtual void generateCode(std::fstream& fileStream)  = 0;

	/* Construtor padrão com a linha de definição */
	IStatement(int lineNumber){
		ownerBlock = NULL;
		myLine = lineNumber;
	}

	IBlock* getOwnerBlock(){
		return ownerBlock;
	}

	/* Setter virtual do bloco pai, pertimitindo sobrescrições por classes filhas */
	virtual void setOwnerBlock(IBlock *newOwnerBlock){
		ownerBlock = newOwnerBlock;
	}
};

#endif
