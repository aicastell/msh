#include "errores.h"

/**************************************************************
* A esta funcion la llamo cuando se produce algun error al    *
* ejecutar una llamada al sistema. Lo que hace es imprimir en *
* pantalla el mensaje de error.                               *
**************************************************************/
void TratarError( char *syscall )
{
    if (errno > 0)
        printf("%s: %s: %s\n", shNAME, syscall, strerror(errno));

}/*TrararError*/


/**************************************************************
* Esta funcion devuelve un valor negativo caso que encuentre  *
* alguna redireccion extraña en la cadena.                    *
**************************************************************/
int IncorrecToken( char *Line )
{
    if( strstr(Line, ">>>") ) return(-100);
    if( strstr(Line, "<<") ) return(-101);
    /* Otros ... */
    return( 0 );
}/* IncorrecToken*/


/**************************************************************
* Esta funcion devuelve un valor negativo caso que encuentre  *
* algun comando en modo background incorrecto.                *
* Ejemplo del error que detecta:                              *
* 	ls -l & | more                                        *
**************************************************************/
int IncorrectBackG ( int Nprocesos )
{
    int i=0;

    while( (i<Nprocesos)&&(EXE[i].BackGround!=1) ) i++;

    if( i<Nprocesos-1 )
        return(-50);

    return (0);

} /*IncorrectBackG*/


/**************************************************************
* Esta funcion muestra por pantalla todos los mensajes de     *
* error que no se pueden tratar con errno.                    *
**************************************************************/
void msg_error( int coderr )
{
    printf("%s: ", shNAME);
    switch( coderr ){
    case -50: printf("unexpected token '&'\n"); break;
    case -100: printf("unexpected token '>>>'\n"); break;
    case -101: printf("unexpected token '<<'\n"); break;
    case -1: printf("syntax error near unexpected token '<'\n"); break;
    case -2: printf("syntax error near unexpected token '>'\n"); break;
    case -3: printf("syntax error near unexpected token '>>'\n"); break;
    case -4: printf("syntax error near unexpected token '2>'\n"); break;
    case -5: printf("syntax error near unexpected token '2>>'\n"); break;
    default: printf("error undefined\n");
    }

} /* msg_error */

