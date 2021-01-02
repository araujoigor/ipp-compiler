#ifndef __IDATA_HPP__
#define __IDATA_HPP__

#include <string>
#include <fstream>

/* Classe abstrada utilizada no armazenamento de
 * dados de variáveis. Ela define os métodos básicos de todos
 * os seus filhos, IString, IInteger e IReal.
 */
class IData{
private:
	/* Toda variável possui um nome */
	std::string id;

protected:
	/* A variavel pode ou não ter sido inicializada. */
	bool hasValue;

public:
	std::string getId(){
		return id;
	}

	IData(std::string myId){
		id = myId;
		hasValue = false;
	}

	bool getHasValue(){
		return hasValue;
	}

	virtual ~IData(){}

	/* Métodos virtuais para retorno do tipo da variável e do valor armazenado por ela.*/
	virtual std::string getType() = 0;
	virtual void printValue(std::fstream& fileStream) = 0;

};

#endif
