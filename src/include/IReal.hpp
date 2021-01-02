#ifndef __IREAL_HPP__
#define __IREAL_HPP__

#include "IData.hpp"

/* Classe que abstrai o armazenamento de um valor real */
class IReal : public IData{
private:
	double value;

public:
	/* Tipo da variável */
	const static std::string myType;

public:
	double getValue(){
	  return value;
	}

	void printValue(std::fstream& fileStream){
		fileStream << value;
	}

	void setValue(double newValue){
		hasValue = true;
		value = newValue;
	}

	std::string getType(){
		return myType;
	}

	IReal(std::string id) : IData(id){
		value = 0;
	}
  
	IReal(std::string id, double myValue) : IData(id){
		value = myValue;
		hasValue = true;
	}
};

#endif
