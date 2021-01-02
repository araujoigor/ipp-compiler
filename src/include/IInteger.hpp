#ifndef __IINTEGER_HPP__
#define __IINTEGER_HPP__

#include "IData.hpp"

/* Classe que abstrai o armazenamento de um valor inteiro*/
class IInteger : public IData{
private:
	/* Valor que a variável possui */
	int value;

public:
	/* Tipo da variável */
	const static std::string myType;

public:
	int getValue(){
		return value;
	}

	void printValue(std::fstream& fileStream){
		fileStream << value;
	}

	void setValue(int newValue){
		hasValue = true;
		value = newValue;
	}

	std::string getType(){
		return myType;
	}

	IInteger(std::string id) : IData(id){
		value = 0;
	}

	IInteger(std::string id, int myValue) : IData(id){
		value = myValue;
		hasValue = true;
	}
};

#endif
