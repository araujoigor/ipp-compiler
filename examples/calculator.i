inteiro option = 0
real num1, num2, result

enquanto (option != 5){

	escreva "== Operations:\n\n"
	escreva "1) Sum\n2) Subtraction\n3) Multiplication\n4) Division\n5) Exit\n"
	escreva "\nChoose one: "
	
	leia option
	
	se(option == 5){
		escreva "\n#ValeuFlws\n\n"
	}senão se(option > 5 ou option < 1){
		escreva "\nYou entered an invalid option, champ. Leaving...\n\n"
		sair
	}senão{
		escreva "\nEnter the first number: "
		leia num1
	
		escreva "\nEnter the second number: "
		leia num2
	
		se (option == 1){
			result = num1 + num2
			escreva "\nResult: " + num1 + " + " + num2 + " = " + result
		}senão se(option == 2){
			result = num1 - num2
			escreva "\nResult: " + num1 + " - " + num2 + " = " + result
		}senão se(option == 3){
			result = num1 * num2
			escreva "\nResult: " + num1 + " * " + num2 + " = " + result
		}senão se(option == 4){
			result = num1 / num2
			escreva "\nResult: " + num1 + " / " + num2 + " = " + result
		} 
		
		escreva "\n\n"
	}
}
