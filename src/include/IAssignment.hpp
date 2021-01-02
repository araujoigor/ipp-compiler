#ifndef __IASSIGNMENT_HPP__
#define __IASSIGNMENT_HPP__

#include "IMath.hpp"
#include "IStatement.hpp"
#include "IBlock.hpp"

class IData;

/*
 * Classe responsável por analisar e gerar trechos de código
 * referentes a comandos de atribuição.
 */
class IAssignment : public IStatement{
private:
  /* Representa a variável que receberá o valor de atribuição */
  IData* leftVariable = NULL;

  /* Representa, possívelmente a variável que realizará a atribuição do valor, se utilizada */
  IData* rightVariable = NULL;

  /* Representa o valor de ponto fixo que será atribuido, se utilizado */
  double rightValue = 0;

  /* Representa o valor inteiro que será atribuido, se utilizado */
  int rightValueInt = 0;

  /* Representa o valor de string que será atribuido, se utilizado */
  std::string *rightStrValue = NULL;

  /* Representa a expressão matemática que será atribuida, se utilizada */
  IMath *rightVariableMath = NULL;

  /* Representa o nome da variável que receberá o valor */
  std::string* leftVariableId = NULL;

  /* Representa o nome da variável que cederá o valor, se utilizado */
  std::string* rightVariableId = NULL;

  /* Variáveis booleanas que definem que tipo de dado será utilizada para ceder o valor */
  bool rightIsInteger = false;
  bool rightIsValue = false;
  bool rightIsStrValue = false;
  bool rightIsMath = false;

public:
  
  /* Construtor utilizado para uma atribuição a partir de um valor inteiro numérico */
  IAssignment(std::string* leftVariableId, int value, int myline) : IStatement(myline){
	  this->leftVariableId = new std::string(*leftVariableId);
	  rightValueInt = value;
	  rightIsValue = true;
	  rightIsInteger = true;
  }

  /* Construtor utilizado para uma atribuição a partir de um valor real numérico */
  IAssignment(std::string* leftVariableId, double value, int myline) : IStatement(myline){
	  this->leftVariableId = new std::string(*leftVariableId);
	  rightValue = value;
	  rightIsValue = true;
  }

  /* Construtor utilizado para uma atribuição a partir de uma variável */
  IAssignment(std::string* leftVariableId, std::string* rightVariableId, int myline) : IStatement(myline){
	  this->leftVariableId = new std::string(*leftVariableId);
	  this->rightVariableId = new std::string(*rightVariableId);
  }

  /* Construtor utilizado para uma atribuição a partir de um valor de string */
  IAssignment(std::string* leftVariableId, std::string rightVariableValue, int myline) : IStatement(myline){
  	  this->leftVariableId = new std::string(*leftVariableId);
  	  this->rightStrValue = new std::string(rightVariableValue);
  	rightIsStrValue = true;
  }

  /* Construtor utilizado para uma atribuição a partir de uma expressão matemática */
  IAssignment(std::string* leftVariableId, IMath *rightVariableMath, int myline) : IStatement(myline){
	  this->leftVariableId = leftVariableId;
	  this->rightVariableMath = rightVariableMath;
	  rightIsMath = true;
  }

  //Destrutor
  virtual ~IAssignment(){
	  delete leftVariable;
	  delete rightVariable;
	  delete rightStrValue;
	  delete rightVariableMath;
	  delete leftVariableId;
	  delete rightVariableId;
  }

  /* Método generateCode. Realiza-se aqui a análise e criação do código de atribuição */
  void generateCode(std::fstream& fileStream) {

	  /* Se um bloco pai não foi atribuido, algo saiu errado */
	  if(ownerBlock == NULL){
		  std::string errorMsg = "Atribuicao sem bloco de codigo pai.";
		  throw ISourceErrorException(errorMsg, myLine);
	  }

	  /* Realiza-se a obtenção das variáveis utilizadas na atribuição.
	   * Ambas devem ter sido previamente definidas, uma vez que se trata
	   * de uma atribuição e não definição.
	   */
	  leftVariable = ownerBlock->getLocalVariable(leftVariableId);
	  rightVariable = ownerBlock->getLocalVariable(rightVariableId);

	  /* Como dito, a variável que receberá o valor deve ter sido previamente
	   * definida.
	   */
	  if(leftVariable == NULL){
		  std::string errorMsg = "Variavel " + *leftVariableId + "nao definida.";
		  throw ISourceErrorException(errorMsg, myLine);
	  }

	  /* Escreve-se o nome da variável e o simbolo de atribuição no codigo parcial. */
	  fileStream << leftVariable->getId() + " = ";

	  /* Caso o valor de atribuição seja numérico ou string
	   * verifica-se se existe compatibilidade de tipos
	   */
	  if(rightIsValue || rightIsStrValue){
		  bool attrError = false, attrWarning = false;

		  /* Não pode-se atribuir valores numericos a valores String */
		  if( leftVariable->getType() == IString::myType && rightIsValue)
			  attrError = true;
		  /* Não pode-se atribuir valores string a valores numéricos */
		  else if( (leftVariable->getType() == IInteger::myType || leftVariable->getType() == IReal::myType ) &&
				  rightIsStrValue)
			  attrError = true;
		  /* Atribuições reais a variaveis inteiras causam perdas de dados */
		  else  if( leftVariable->getType() == IInteger::myType && rightIsValue && !rightIsInteger)
			  attrWarning = true;

		  if(attrError){
			  //Erro
			  std::string rightType = (rightIsValue) ? "numerica" : "string";
			  std::string errorMsg = "Impossível atribuir um tipo " + rightType + " a uma variavel do tipo " + leftVariable->getType();
			  throw ISourceErrorException(errorMsg, myLine);
		  }else if(attrWarning){
			  //Warning.
			  std::cout << "Atencao: usar uma variavel real em uma operacao para uma variavel inteira causa perda de dados. " << std::endl;
			  fileStream << "(int)";
		  }

		  std::string outValue;

		  /* Escreve-se o valor no código parcial */
		  if(rightIsStrValue)
			  outValue = ("\"" + (*rightStrValue) + "\"");
		  else
			  outValue = (rightIsInteger) ? std::to_string(rightValueInt) : std::to_string(rightValue);

		  fileStream << outValue << ";\n";
	  }
	  /* Do contrário, se o valor for uma expressão matemática,
	   * verifica-se a compatibilidade de tipos
	   */
	  else if(rightIsMath){
		  /* Escreve-se a expressão no código parcial */
		  fileStream << rightVariableMath->checkAndGetExpression(ownerBlock, myLine) << ";\n";

		  /* Analisa-se o tipo de retorno da expressão matemática e o tipo da
		   * variável que receberá o valor. Se o valor o tipo da variável for String,
		   * então a atribuição não é permitida.
		   * Se a variável for inteira e o valor de retorno da expressão for real,
		   * existe perda de dados, e gera-se um warning.
		   */
		  std::string type = leftVariable->getType();
		  if(rightVariableMath->getMathType() == IMath::TYPE_REAL && type == IInteger::myType){
			  //warning
			  std::cout << "Atencao: usar uma variavel real em uma operacao para uma variavel inteira causa perda de dados. " << std::endl;
		  }else if(type == IString::myType){
			  std::string errorMsg = "Impossivel atribuir uma expressao matematica a variavel string " + leftVariable->getId();
			  throw ISourceErrorException(errorMsg, myLine);
		  }
	  }
	  /*
	   * Finalmente, se o valor de atribuição for outra variável, garante-se a compatibilidade de tipo
	   * e escreve-se seu nome do código parcial.
	   */
	  else{
		  if(rightVariable != NULL ){

			  if(rightVariable->getType() != leftVariable->getType() && !(leftVariable->getType() == "real" && rightVariable->getType() == "inteiro") ){
				  std::string errorMsg = "Impossivel atribuir '" + rightVariable->getId() + "' (" + rightVariable->getType() + ") a '" + leftVariable->getId() + "' (" + leftVariable->getType() + "). Tipos incompativeis.";
				  throw ISourceErrorException(errorMsg, myLine);
			  }

			  fileStream  << rightVariable->getId() << ";\n";
		  }
		  else{
			  std::string errorMsg = "Variavel " + *rightVariableId + " nao declarada.";
			  throw ISourceErrorException(errorMsg, myLine);
		  }

	  }

  }
};

#endif
