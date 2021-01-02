inteiro secret, guess, attempts, option, counter = 0

escreva "=== Welcome to the Guess The Number game!\n\n"
escreva "1) Play\n2) Instructions\n3) Exit\nType your choice: "

leia option

se (option > 3 ou option < 0){
	escreva "\nYou typed an invalid option. Leaving...!\n\n"
	sair
} senão se (option == 3){
	escreva "\n#ValeuFlws\n\n"
	sair
} senão se (option == 2){
	escreva "\nIn this game, the user defines a number to be guessed by a second player and also the number of attempts this player has.\n"
} senão{
	escreva "\nType the maximum number of attempts to guess the secret number: "
	leia attempts

	escreva "\nType the secret number: "
	leia secret

	escreva "\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n"
	escreva "\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n"
	escreva "READY? GO!\n\n"
	escreva "Good luck, friend! You have " + attempts + " attempts. (Type -1 to leave the game)\n"

	repita attempts vezes{
		counter = counter + 1 
		escreva "Type the attempt number " + counter + ": "
		leia guess
	
		se(guess == -1){
			escreva "\n#vlwflws\n\n"
			sair
		}

		se(secret < guess){
			escreva "\nWrong. The secret is smaller than " + guess + ".  Try again.\n\n"
		} senão se (secret > guess) {
			escreva "\nWrong. The secret is bigger than " + guess + ".  Try again.\n\n"
		} senão{
			escreva "\n\nCongrats! You won!\n\n"
			sair		
		}
	}

	escreva "Time is up, friend. You have lost. The secret was: " + secret + "\n\n"
	escreva "#vlwflws\n\n"
	
}
