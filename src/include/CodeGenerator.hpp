#ifndef __CODEGENERATOR_HPP__
#define __CODEGENERATOR_HPP__

#include <fstream>
#include <iostream>
#include <vector>
#include <string>

#include "ISourceErrorException.hpp"

/*
 * Definição da classe (interface) abstrata CodeGenerator
 * esta classe é definida por todas as classes do projeto,
 * uma vez que o método generateCode delega a cada classe
 * a responsabilidade de gerar seu próprio código final,
 * após a análise de erros.
 */
class CodeGenerator{

public:
	/*
	 * Método virtual abstrato generateCode. É o método mais importante do projeto
	 * uma vez que abstrai todo o processo de análise e criação de código.
	 * Este método deve gerar uma exceção do tipo ISourceErrorException,
	 * sempre que um erro de código for encontrado
	 */
	virtual void generateCode(std::fstream& fileStream) = 0;

	/*
	 * Destrutor virtual da classe. Possibilita que mediante a chamadas
	 * ao destrutor, o destrutor das filhas seja executado, ao invés
	 * deste próprio destrutor.
	 */
	virtual ~CodeGenerator(){}
};

#endif
