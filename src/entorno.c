#include "entorno.h"

/****************************************************************
* Inserta una nueva variable de entorno de la forma name=value  *
* en la estructura ENTORNO, y a la vez, le actualiza export de  *
* la manera adecuada. En caso que la variable exista y sea una  *
* variable export, se llama a entorno_to_environ para que la    *
* variable este inmediatamente disponible en environ.           *
****************************************************************/
void entorno_put( char *cadena, int exported )
{
    int i=0, encontrado=0;
    char *nombre, *valor;

    nombre = strtok( &cadena[0], "=" );

    if( entorno_full() ) {
        printf("environ: there is not space for more environ. Increase NR_ENTORNO\n");
        return;
    }

    /* Busco si nombre ya esta en ENTORNO. De ser asi lo reemplazo. */
    while( (i<ENTORNO.next_empty) && (!encontrado) ){
        if( !strcmp( ENTORNO.l_entorno[i].nombre, nombre ) )
        encontrado=1;
        else
        i++;
    }

    if( !encontrado ) {
        ENTORNO.l_entorno[ENTORNO.next_empty].nombre = strdup( nombre );
        valor = strchr( cadena, '\0' );	/* Sobre el \0 que marca strtok */
        ENTORNO.l_entorno[ENTORNO.next_empty].valor = strdup( &valor[1] );
        ENTORNO.l_entorno[ENTORNO.next_empty].exported = exported;
        ENTORNO.next_empty++;
    }
    else {
        valor=strchr( cadena, '\0' ); /* Sobre el \0 que marca strtok */
        ENTORNO.l_entorno[i].valor=strdup( &valor[1] );
        if( ENTORNO.l_entorno[i].exported == 0 )
            ENTORNO.l_entorno[i].exported = exported;
        if( ENTORNO.l_entorno[i].exported == 1 )
            entorno_to_environ();
    }

} /* entorno_put */


/****************************************************************
* Obtiene el valor de una variable de entorno name pasada como  *
* argumento, o NULL si esa variable no existe. Dicho valor se   *
* extrade de la tabla interna ENTORNO                           *
****************************************************************/
char *entorno_get( char *cadena )
{
    int i=0, encontrado = 0;

    while( (i<ENTORNO.next_empty) && (!encontrado) ) {
        if( !strcmp( ENTORNO.l_entorno[i].nombre, cadena ) )
            encontrado=1;
        else
        i++;
    }

    if( !encontrado )
        return( NULL );

    return( &ENTORNO.l_entorno[i].valor[0] );

} /* entorno_get */


/****************************************************************
* Inicializa la tabla de variables de entorno interna con los   *
* valores de la variable environ.                               *
****************************************************************/
void entorno_init()
{
    int i;

    ENTORNO.next_empty=0;

    for( i=0; environ[i]!=NULL; i++ )
        entorno_put( environ[i], 1 ); /* Las variables heredadas son todas export */

    entorno_to_environ();

} /* entorno_init */


/****************************************************************
* Dice si la tabla de variables de entorno interna esta vacia   *
****************************************************************/
int entorno_empty()
{
    return( ENTORNO.next_empty==0 );

} /* entorno_empty */


/****************************************************************
* Dice si la tabla de variables de entorno interna esta llena   *
****************************************************************/
int entorno_full()
{
    return( ENTORNO.next_empty==NR_ENTORNO );

} /* entorno_full */


/****************************************************************
* Lista en pantalla las variables de entorno. Si all==1 lista   *
* todas las variables, mientras que si all==0 tansolo lista     *
* las variables exportadas.                                     *
****************************************************************/
void entorno_display( int all )
{
    int i;

    if( !entorno_empty() ){
        for( i=0; i<ENTORNO.next_empty; i++ ){
            if( all==1 )
                printf("%s=%s\n", ENTORNO.l_entorno[i].nombre, ENTORNO.l_entorno[i].valor );
            else{
                if( ENTORNO.l_entorno[i].exported )
                    printf("%s=%s\n", ENTORNO.l_entorno[i].nombre, ENTORNO.l_entorno[i].valor );
            }
        }
    }

} /* entorno_display */


/****************************************************************
* Introduzco todas las variables de entorno que mi shell        *
* introduce, aparte del entorno que hereda del padre            *
****************************************************************/
void entorno_loadefault()
{
    char *aux, sh[MAXLENCAD]="";

    aux = strdup("AUTOR=Ivan Castell Rovira");
    entorno_put(aux, 1);

    aux = strdup("VERSION=vertical");
    entorno_put(aux, 1);

    strcpy(sh, "SHELL=");
    strcat(sh, shNAME);
    entorno_put(sh, 1);

    entorno_to_environ();

} /* entorno_loadefault */


/****************************************************************
* Marca como exportable una variable de la tabla ENVIRON.       *
****************************************************************/
void entorno_export( char *cadena )
{
    int i=0, encontrado=0;

    if( strstr(cadena, "=") )	/* export a=10 por ejemplo */
        entorno_put( cadena, 1 );
    else {
        while( (i<ENTORNO.next_empty) && (!encontrado) ) {
            if( !strcmp( ENTORNO.l_entorno[i].nombre, cadena ) )
                encontrado=1;
            else
                i++;
        }
        if( encontrado )
            ENTORNO.l_entorno[i].exported=1;
    }
    entorno_to_environ();

} /* entorno_export */


/****************************************************************
* Creo una nueva variable environ con todas las variables de    *
* la tabla ENTORNO que estan exportadas.                        *
****************************************************************/
void entorno_to_environ()
{
    int i, envi=0;
    char env[MAXLENCAD*1024]; /* Tamaño grande por variable PATH */

    environ = (char **) malloc((NR_ENTORNO)*sizeof(char *));

    for( i=0; i<NR_ENTORNO; i++ )
    {
        if( ENTORNO.l_entorno[i].exported )
        {
            env[0]='\0';
            strcpy( env, ENTORNO.l_entorno[i].nombre );
            strcat( env, "=" );
            strcat( env, ENTORNO.l_entorno[i].valor );

            environ[envi]=strdup( env );
            envi++;
        }
    } /* for */

    environ[envi] = NULL;

} /* entorno_to_environ */


/****************************************************************
* Esta funcion se utiliza para, dada la estructura EXE,         *
* sustituir todas las variables de entorno por sus valores.     *
****************************************************************/
void entorno_to_EXE( int nprocess )
{
    int i, j, ienv;
    char env[MAXLENCAD], aux[MAXLENCAD];
    char *eol, *p;

    for( i=0; i<nprocess; i++ ){
        for( j=1; EXE[i].VCommand[j]!=NULL; j++ ){
            /* j empieza en 1 porque el ejecutable nunca es una variable de entorno. */
            p = strchr( EXE[i].VCommand[j], '$' );
            if( p!=NULL ){ /* Si existe el caracter $ en la cadena */
                env[0]='\0';
                aux[0]='\0';
                eol=strchr( EXE[i].VCommand[j], '\0' );
                *p='\0';
                strcpy( aux, EXE[i].VCommand[j] );
                p++;
                ienv=0;
                while( isalnum(*p)&&(p!=eol) ){
                    env[ienv]=*p;
                    ienv++;
                    p++;
                }
                env[ienv]='\0';
                if( entorno_get(env) ){	/* Si existe env */
                    strcat( aux, entorno_get(env) );
                    if( p!=eol )	/* Si queda cadena despues de env (/$HOME/...) */
                        strcat( aux, p );
                    EXE[i].VCommand[j]=strdup(aux);	/* Modifico variable global */
                }
            } /* if */
        } /* for j */
    } /* for i */

} /* entorno_to_EXE */

