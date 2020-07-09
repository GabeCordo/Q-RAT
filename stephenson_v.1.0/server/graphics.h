#ifndef HEADER_GRAPHICS
#define HEADER_GRAPHCIS

	/*
	 * Colour Macros for the UNIX Terminal
	 */
	
	#define Black "\u001b[30m"
	#define Red "\u001b[31m"
	#define Green "\u001b[32m"
	#define Yellow "\u001b[33m"
	#define Blue "\u001b[34m"
	#define Magenta "\u001b[35m"
	#define Cyan "\u001b[36m"
	#define White "\u001b[37m"
	#define Reset "\u001b[0m"
	
	/*
	 * Macro For Printing Graphics To Console
	 */
	
	#define PRINT_BANNER														\
		printf("\nStephenson Remote Access Tool\n");							\
		printf("[%s+%s] https://github.com/GabeCordo/Stephenson", Red, White);	\
	
#endif // HEADER GRAPHICS