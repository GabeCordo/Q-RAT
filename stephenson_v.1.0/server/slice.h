#ifndef HEADER_SLICE
#define HEADER_SLICE

	/*	This is a very small public function, hence I have included it in the
	 *. header file to avoid excess files within the ../server folder
	 */
	#include <stdio.h>
	#include <string.h>
	#include <stdlib.h>

	/*
	 * Function:  slice 
	 * ----------------
	 * implements string-slicing to c:
	 *	  char testing[5] = 'testing'
	 *    slice(testing, 0, 4);
	 *
	 *  string_: number of terms in the series to sum
	 *	lowerbound_: inclusive, first char offset of the string
	 *	upperbound_: non-inclusive, last char offset of the string slice
	 *
	 *  returns: the memory address of a new sliced string, maintaining
	 *           the values of the pointer-argument
	 *           returns NULL on error (if bounds are not correct)
	 */
	char *slice(char string_[], int lowerbound_, int uppperbound_) {
		
		//check to see that we have not received an empty string
		if (strcmp(string_, "\0") == 0)
			return NULL;
		
		char *bufferP;
		size_t str_len, buffer_len;
		str_len = sizeof(string_);
		
		// check to make sure the bounds are not > the length of the string
		if (abs(uppperbound_) > str_len - 1 ||  abs(lowerbound_) > str_len - 1)
			return NULL;
		
		// if we receive a negative index, we need to transform that into 
		if (lowerbound_ < 0 || uppperbound_ < 0) {
			
			uppperbound_ += str_len;
			lowerbound_ += str_len - 1; 
			
		}
		
		//find the length of the new string acording to the bound arguments
		buffer_len = uppperbound_ - lowerbound_;
		string_ += lowerbound_; //bit shift to the lower-bound of string slice
		
		bufferP = calloc(buffer_len, sizeof(char)); //allocate 
		strncpy(bufferP, string_, buffer_len); //copy the slice to the new array bufferP
		
		return bufferP; //return the slice (will be on the heap)
	}
	
#endif // HEADER_SLICE