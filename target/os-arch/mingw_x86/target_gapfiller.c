/*
 * Lots of bits and bob not implemented by mingw though the headers don't tell you!
 */

#include <stdlib.h>
#include <windows.h>


char setenv (char *Name, char * Value){
	SetEnvironmentVariable(Name,Value);
}
