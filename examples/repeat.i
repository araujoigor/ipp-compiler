real increment, repetitions, value = 0
inteiro finalValue

escreva "In this program you will be able to learn how to use the repetition instruction `repita` and how the compiler warns you about potential issues with your code\n"

escreva "The final value can be wrong because the `repita` instruction rounds the number of executions to the upper integer when dealing with real (non-integer) values\n\n"

escreva "To which number you want to count? "
leia finalValue

escreva "\n\nWith which increment? "
leia increment

repetitions = (finalValue / increment) + 1

repita repetitions vezes{
	escreva value + " "
	value = value + increment
}
