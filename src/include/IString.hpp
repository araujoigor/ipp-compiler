#ifndef __ICHAR_HPP__
#define __ICHAR_HPP__

#include "IData.hpp"

#include <iostream>

/* Classe que abstrai o armazenamento de um valor real */
class IString: public IData{
private:
	std::string value;

public:
	/* Tipo da variável */
	const static std::string myType;

public:
	std::string getValue(){
		return value;
	}

	void printValue(std::fstream& fileStream){
		fileStream << "\"" << value << "\"";
	}

	void setValue(char newValue){
		hasValue = true;
		value = newValue;
	}

	std::string getType(){
		return myType;
	}

	IString(std::string id) : IData(id){
		value = "";
	}
  
	IString(std::string id, std::string myValue) : IData(id){
		value = myValue;

		hasValue = true;
	}
};

#endif
