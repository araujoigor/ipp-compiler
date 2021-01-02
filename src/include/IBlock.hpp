#ifndef __IBLOCK_HPP__
#define __IBLOCK_HPP__

#include "CodeGenerator.hpp"
#include "IStatement.hpp"

#include <algorithm>

/* Classe responsável por gerenciar blocos de código*/
class IBlock : CodeGenerator{

private:
	/* Todo bloco de código deve possuir uma lista de variáveis locais,
	 * uma lista de statements que o compõem e um statement pai (com exceção
	 * do bloco raiz)*/
	std::vector<IData*> blockVariables;
	std::vector<IStatement*> statementsList;

	IStatement *stmtOwner = NULL;

public:

	/* Contrutor para blocos que possuem somente um statement. */
	IBlock(IStatement *stmt){
		stmtOwner = stmt;
	}

	/* Contrutor padrão vazio */
	IBlock(){}

	/* Método para definição do statement pai*/
	void setStatementOwner(IStatement *stmt){
		stmtOwner = stmt;
	}

	/* Método utilizado para o registro de variáveis locais neste bloco.
	 * Ele verifica se uma variável com aquele nome já existe. Se não
	 * existir a registra. Do contrário gera um erro.
	 */
	void registerLocalVariable(IData* var, int myLine) {
		if(getLocalVariable(var->getId()) != NULL){
			std::string errorMsg = "Variavel referenciada ja existente.";
			throw ISourceErrorException(errorMsg, myLine);
		}
		blockVariables.push_back(var);
	}

	/* Método que realiza a buscar de uma variável local
	 * pré-definida. Caso a variável com o nome especificado exista, sua referência
	 * é retornada. Do contrario, retorna-se NULL. */
	IData* getLocalVariable(std::string variableId){
		for(ulong i = 0; i < blockVariables.size(); i++){

			if(blockVariables[i]->getId() == variableId)
				return blockVariables[i];
		}

		/* Caso o bloco local não possua a variavel, perguntamos aos blocos pai */
		if(stmtOwner != NULL){
			return stmtOwner->getOwnerBlock()->getLocalVariable(variableId);
		}

		return NULL;
	}

	/* Mestmo método acima, porém para um ponteiro de string) */
	IData* getLocalVariable(std::string *variableId){
		if(variableId == NULL) return NULL;

		return getLocalVariable(*variableId);
	}

	/* Método utilizado para adição de statements no bloco */
	void addStatement(IStatement *stmt){
		statementsList.push_back(stmt);
	}

	/* Método para criação do código do bloco. O bloco por si só não possui
	 * código. Então este metodo apenas chama o método generateCode
	 * de cada statement que compõe esse bloco. Assim, a tarefa
	 * de criação de código parcial fica delegado a cada classe.
	 */
	void generateCode(std::fstream& fileStream) {
		for(ulong i = 0; i < statementsList.size(); i++){
			statementsList[i]->generateCode(fileStream);
		}
	}

	/* Método utilizado para criação do código parcial.
	 * Esse método é utilizado apenas pelo bloco raiz.
	 * Ele gera o template do código parcial e inclui
	 * o código analisado através de uma chamada ao
	 * método generateCode() do bloco raiz.
	 *
	 * Caso alguma exceção seja lançada, o processo de
	 * criação é interrompido e o codigo de erro é mostrado.
	 */
	bool createCode(std::string tempFile){
		std::fstream output(tempFile, std::ios::out);

		try{
			if(output){
				output << "#include <iostream>\n";
				output << "using namespace std;\n";
				output << "int main(){\n";
				generateCode(output);
				output << "return 0; }";
				output.close();

				return true;
			}else{
				std::cout << "Erro ao abrir arquivo fonte" << tempFile << std::endl;
				return false;
			}
		}catch(ISourceErrorException& exception){
			std::cout << exception.getErrorMessage() << std::endl;
			output.close();

			return false;
		}
	}

	/* Método estático utilizado para criação de nomes randomicos
	 * de variáveis. Esse método é utilizado principalmente no processamento
	 * do comando REPITA, que se baseia em um laço FOR e portanto,
	 * a variável utilizada na contagem deve ser única,
	 * para evitar problemas com comandos repitas aninhados. */
	static std::string generateRandomVariableName( size_t length )
	{
	    auto randchar = []() -> char
	    {
	        const char charset[] =
	        "0123456789"
	        "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
	        "abcdefghijklmnopqrstuvwxyz";
	        const size_t max_index = (sizeof(charset) - 1);
	        return charset[ rand() % max_index ];
	    };
	    std::string str(length,0);
	    std::generate_n( str.begin(), length, randchar );
	    return str;
	}
};

#endif
