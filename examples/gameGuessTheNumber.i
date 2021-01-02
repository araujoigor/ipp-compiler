inteiro secret, attempt, maxAttempts, option, counter = 0


escreva "===Welcome to the Guess The Number game!\n\n"
escreva "1) Play\n2) Instructions\n3) Exit\nType your choice: "

leia option

se (option > 3 ou option < 0){
	escreva "\nYou entered an invalid option. Aborting...!\n\n"
	sair
} senão se (option == 3){
	escreva "\n#ValeuFlws\n\n"
	sair
} senão se (option == 2){
	escreva "\nIn this game, the user defines a secret number and the number of choices the other player will have to guess it."
} senão{
	escreva "\nEnter the maximum number of attempts: "
	leia maxAttempts

	escreva "\nEnter the secret number: "
	leia secret

	escreva "\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n"
	escreva "\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n"
	escreva "Good luck, friend! You have " + maxAttempts + " attempts to guess the secret number. (or type -1 to exit)\n"

	repita maxAttempts vezes{
		counter = counter + 1 
		escreva "Enter your attempt number " + counter + ": "
		leia attempt
	
		se(attempt == -1){
			escreva "\n#vlwflws\n\n"
			sair
		}

		se(attempt > secret){
			escreva "\nWrong. The secret number is smaller than " + attempt + ". Try again.\n\n"
		} senão se(attempt < secret) {
			escreva "\nWrong. The secret number is bigger than " + attempt + ". Try again.\n\n"
		}senão{
			escreva "\n\nCongrats, champ. You won!\n\n"
			sair
		}
	}

	escreva "Time is up! You lose. The secret number was: " + secret + "\n\n"
	escreva "#vlwflws\n\n"
}
