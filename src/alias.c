#include "alias.h"

/****************************************************************
* Inicializo la estructura de los alias al iniciar el shell.    *
****************************************************************/
void alias_init()
{
    ALIAS.next_empty=0;
} /* alias_init */


/****************************************************************
* Dice si la estructura de alias esta vacia.                    *
****************************************************************/
int alias_empty()
{
    return( ALIAS.next_empty==0 );
} /* alias_empty */


/****************************************************************
* Dice si la estructura de alias esta llena.                    *
****************************************************************/
int alias_full()
{
    return( ALIAS.next_empty==NR_ALIAS );
} /* alias_full */


/****************************************************************
* Devuelve el apodo de la cadena pasada como argumento si existe*
* como alias o NULL en caso contrario.                          *
****************************************************************/
char *alias_get( char *cadena )
{
    int i=0, encontrado=0;

    while( (i<ALIAS.next_empty) && (!encontrado) )
    {
        if( !strcmp( ALIAS.l_alias[i].nombre, cadena ) )
            encontrado=1;	/* Nombre encontrado */
        else
            i++;
    }
    if( !encontrado ){
        return( NULL );
    }
    return ( &ALIAS.l_alias[i].apodo[0] );
} /* alias_get */


/****************************************************************
* Inserta alias añadiendolos al final de la estructura.         *
****************************************************************/
void alias_put( char *cadena )
{
    int i=0, encontrado=0;
    char *nombre, *apodo;

    nombre = strtok( &cadena[0], "=" );

    /*
    * Busco si aux ya esta en ALIAS. De ser asi lo reemplazo.
    */
    if( !alias_full() )
    {
        while( (i<ALIAS.next_empty) && (!encontrado) )
        {
            if( !strcmp( ALIAS.l_alias[i].nombre, nombre ) )
                encontrado=1;	/* Nombre encontrado */
            else
                i++;
        }

        if( !encontrado ) { /* Hay peligro de que alias a=b y alias b=a */
            apodo = strtok( (void*)0, "=" );
            if( (alias_get(apodo)) && (!strcmp(alias_get(apodo), nombre)) )
                printf("alias: cyclic redundance not permitted\n");
            else {
                ALIAS.l_alias[ALIAS.next_empty].nombre = strdup( nombre );
                ALIAS.l_alias[ALIAS.next_empty].apodo = strdup( apodo );
                ALIAS.next_empty++;
            }
        }
        else { /* Si lo encuentra tansolo sustituimos el apodo */
            apodo = strtok( (void *)0, "=" );
            ALIAS.l_alias[i].apodo = strdup( apodo );
        }
    }
    else
        printf("alias: there is not space for more alias. Increase NR_ALIAS\n");

} /* alias_put */


/****************************************************************
* Funcion necesaria para implementar el comando interno unalias *
****************************************************************/
void alias_delete( char *cadena  )
{
    int i=0, encontrado=0, j;

    if( !alias_empty() )
    {
        while( (i<ALIAS.next_empty) && (!encontrado) )
        {
            if( !strcmp( ALIAS.l_alias[i].nombre, cadena ) )
                encontrado=1;	/* Nombre encontrado */
            else
                i++;
        }

        if( !encontrado )
            printf("unalias: %s not found\n", cadena);
        else
        {
            /* Comienza proceso de borrado. i contiene indice a borrar. */
            for( j = i+1; j<ALIAS.next_empty; j++ ) {
                ALIAS.l_alias[j-1].nombre = ALIAS.l_alias[j].nombre;
                ALIAS.l_alias[j-1].apodo = ALIAS.l_alias[j].apodo;
            }
            ALIAS.next_empty--;
        }
    }

} /* alias_delete */


/****************************************************************
* Inserta alias por defecto, añadiendolos al final de la        *
* estructura ALIAS. Esta funcion se llama en LoadShell.         *
****************************************************************/
void alias_loadefault()
{
    char *aux;

    aux = strdup("ll=ls -l");
    alias_put( aux );

    aux = strdup("la=ll -a");
    alias_put( aux );

    aux = strdup("v=ls -l");
    alias_put( aux );

    aux = strdup("w=who");
    alias_put( aux );

} /* alias_loadefault */


/****************************************************************
* Lista por pantalla todos los alias de l_alias.                *
****************************************************************/
void alias_display()
{
    int i;

    if (!alias_empty()) {
        for( i=0; i<ALIAS.next_empty; i++ )
        printf("alias %s='%s'\n", ALIAS.l_alias[i].nombre, ALIAS.l_alias[i].apodo);
    }

} /* alias_display */


/****************************************************************
* Esta funcion me imprime una alias en pantalla.               *
****************************************************************/
void alias_display_single( char *cadena )
{
    int i=0, encontrado=0;

    if (alias_empty())
        return;

    while( (i<ALIAS.next_empty) && (!encontrado) )
    {
        if( !strcmp( ALIAS.l_alias[i].nombre, cadena ) ){
            encontrado = 1; /* Nombre encontrado */
            printf("alias %s='%s'\n", ALIAS.l_alias[i].nombre, ALIAS.l_alias[i].apodo);
        }
        else
            i++;
    }

    if( !encontrado )
        printf("alias: %s not found\n", cadena);

} /* alias_display_single */


/****************************************************************
* Esta funcion se encarga de tratar toda la cadena que acompaña *
* al comando alias. Asi por ejemplo:                            *
*                                                               *
*	VCommand={"r", "ll=ls", "v='ls -l'", "j='ls"}               *
*                                                               *
* Siguiendo con detenimiento el algoritmo de alias_handle se    *
* observa como para dicho VCommand, esta funcion imprime el     *
* alias de r, asigna ll=ls, asigna v=ls -l y por ultimo da un   *
* error indicando que falta el parametro ' en la cadena alias.  *
* El VCommand que realmente se recibe no es &EXE[i].VCommand[0] *
* sino &EXE[i].VCommand[1], puesto que el parametro [0]="alias" *
* no aporta ninguna informacion, por lo que prescindo de el     *
****************************************************************/
void alias_handle( char **VCommand )
{
    int i, delimited, error;
    char aux[MAXLENCAD], c_delim, *ptr_delim1, *ptr_delim2;

    if( VCommand[0]==NULL )	{ /* Si se ejecuta alias */
        alias_display();
        return;
    }

    for( i = 0; VCommand[i]!=NULL; i++ )
    {
        if( (strstr(VCommand[i], "='")) || (strstr(VCommand[i], "=\""))  )
        {
            /*
            * Construyo cadena y caracter delimitador por propia comodidad.
            */
            if( strstr(VCommand[i], "='"))
                c_delim='\'';
            else
            c_delim='\"';

            ptr_delim1=strchr( VCommand[i], c_delim );
            ptr_delim2=strchr( ptr_delim1+1, c_delim );

            /* Comienzo concatenacion de parametros */
            if( ptr_delim2==NULL ){	/* a='pepe o a=' */
                if( ptr_delim1[1]=='\0' )	/* a=' */
                    printf("alias: expected token %c\n", c_delim);
                else{ /* Pillar comando completo */
                    aux[0]='\0';
                    strmchr( c_delim, VCommand[i] );
                    strcpy( aux, VCommand[i] );
                    delimited = error = 0;

                    while( (!delimited)&&(!error) ){
                        strcat( aux, " ");
                        error=(VCommand[i+1]==NULL);
                        if( !error ){
                            i++;
                            strcat( aux, VCommand[i]);
                            delimited = aux[ strlen(aux)-1 ]==c_delim;
                        }
                    }
                    if( !delimited )
                        printf("alias: expected token %c\n", c_delim);
                    else{
                        strmchr( c_delim, aux );
                        alias_put(aux);
                    }
                }
            }
            else { /* a='' o a='pepe' */
                if( ptr_delim2!=ptr_delim1+1 ){/* Caso que a='' paso de el */
                    strmchr( c_delim, VCommand[i] );
                    alias_put(VCommand[i]);
                }
            } /* else */
        }
        else{
            if( strstr(VCommand[i], "=") )
                alias_put( VCommand[i] );
            else
                alias_display_single( VCommand[i] );
        } /* else */
    } /* for */

} /* alias_handle */


/****************************************************************
* Esta funcion modifica la estructura global EXE con los        *
* valores almacenamos en la tabla de los alias.                 *
****************************************************************/
void alias_to_EXE( int nprocess )
{
    int i,j;
    char *apodo, *aux;

    for( i=0; i<nprocess; i++ )
    {
        while( (apodo = alias_get(EXE[i].VCommand[0]))!=NULL )
        {
            /* Mientras el comando ejecutable tenga entrada en la lista de alias */
            apodo = strdup( apodo );
            /* Duplico apodo porque si trabajo directamente con ALIAS me cargo la tabla. */
            EXE[i].VCommand[0] = strdup( strtok(&apodo[0], " ") );
            for( j=0; EXE[i].VCommand[j]!=NULL; j++ );
            /* Ahora j es el primer puntero vacio de VCommand */
            aux = strtok((void *)0, " ");
            while( aux!=NULL ){
                EXE[i].VCommand[j]=strdup( aux );
                j++;
                aux = strtok((void *)0, " ");
            }

            /*
            * Hay que usar aux, puesto que si hago:
            * EXE[i].VCommand[j]=strdup(strtok((void *)0, " ")), cuando strtok me
            * devuelve NULL, strdup( NULL )="\0", y me desmonta la estructura EXE.
            */
        }
    }

} /* alias_to_EXE */

