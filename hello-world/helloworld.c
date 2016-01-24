/*
 * Title: helloworld.c
 * Author: 93aef0ce4dd141ece6f5
 * Description: using inline assembly to print
 *              "hello world" 
 *
 */

#include <stdio.h>

/*
 * testing inline assembly
 */
void print (char *s) {
	__asm__ (".intel_syntax noprefix;"
			"mov	eax, %0;"
			"push 	eax;"
			"call 	printf;"
			"add 	esp, 4;"
			".att_syntax noprefix;"
			:
			: "r" (s)
			);
}

int main (int argc, char *argv[]) {
	print ("Hello World!\n");

	return 0;
}
