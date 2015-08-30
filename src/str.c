#include "str.h"

/**************************************************************
* strmvback: string move back.                                *
* El caracter n de la cadena sobreescribe al 0, el n+1 al 1,  *
* y asi hasta reducir la cadena en n caracteres.              *
* Ejemplo: strmvback( "0123456789", 2 ) --> "23456789"        *
* Ejemplo: strmvback( "abcdefghij", 1 ) --> "bcdefghij"       *
**************************************************************/
void strmvback( char *cadena, int n )
{
    strcpy( cadena, &cadena[n] );
} /* strmvback */


/**************************************************************
* strmchr: string remove character.                           *
* Borra de cadena cada ocurrencia del caracter chr.           *
* Ejemplo: strmchr( "v=\"ls -l -a\"", \" ) --> "v=ls -l -a"   *
**************************************************************/
void strmchr( char chr, char *cadena )
{
    char *ptr;
    while( (ptr = strchr( cadena, chr )) != NULL ) {
        strcpy( &ptr[0], &ptr[1] );
    }
} /* strmchr */

