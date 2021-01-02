real increment, repetitions, value = 0
inteiro targetNumber

escreva "This program illustrates the use of the repetition command `repita` and how the compiler warns you about potential issues with the code\n"
escreva "\nIn this case, the `repetitions` variable used in the loop instruction is a real number which might make the program behave in an unexpected way since the `repita` instruction rounds non-integer values to the upper integer\n"

escreva "\n\nUntil which number you want to count? "
leia targetNumber

escreva "\n\nWhat is the increment value? "
leia increment

repetitions = (targetNumber / increment) + 1

repita repetitions vezes{
	escreva value + " "
	value = value + increment
}
