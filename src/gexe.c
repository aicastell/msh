#include "gexe.h"

/***************************************************************
* Esta funcion inicializa a NULL todos los campos de EXE       *
***************************************************************/
void InicializaEXE()
{
    int i,j;

    for( i=0; i<MAXCOMMANDS; i++ )
    {
        EXE[i].BackGround = 0;
        for( j=0; j<MAXPARAMS; j++ )
            EXE[i].VCommand[j] = NULL;
        for( j=0; j<5; j++ )
            EXE[i].Redirecciones[j] = NULL;
    }

}/* InicializaEXE */


/****************************************************************
* Relleno la estructura EXE de la forma mas comoda posible      *
* para a continuacion llevar a cabo la ejecucion con exec.      *
*                                                               *
* ENTRADA: LineCommand es la linea de comandos a tratar.        *
*                                                               *
* RETURN:  La funcion devuelve un entero>0 con el numero de     *
*          procesos que hay que ejecutar, si es que no ha       *
*          habido ningun error. Caso que se detecte algun error *
*          en la cadena de entrada, antes o durante su          *
*          tratamiento, se devuelve un codigo de error negativo *
*          que ya sera tratado en la funcion msg_error que hay  *
*          definida en la libreria errores.h.c                  *
****************************************************************/
int gexe_rellenar( char LineCommand[] )
{
    int N, i, j, strlong, param;
    int errorToken, errorBG;

    errorToken = IncorrecToken( LineCommand );
    if( errorToken<0 )
        return( errorToken );

    InicializaEXE();

    /* Separa la linea de comandos por caracteres '|' */
    EXE[0].VCommand[0]=strtok( &LineCommand[0], "|");

    i = 0;
    while( EXE[i].VCommand[0]!=NULL ) {
        EXE[i+1].VCommand[0] = strtok( (void *)0, "|" );
        i++;
    }
    N=i;  /* N es el numero de estructuras EXE activadas */

    if( N==0 )
        return(0);

    /* Marco BackGrounds que encuentre en cada cadena
     * para controlar posibles BackGrounds erroneos. */
    for( i=0; i<N; i++ ){
        strlong = strlen( EXE[i].VCommand[0] );
        if( strlong>0 ) {
            strlong-=1;      /* Primer caracter valido antes del \0 */
            while( isspace( EXE[i].VCommand[0][strlong] ) )
                strlong--;
            if( EXE[i].VCommand[0][strlong]=='&' ){
                EXE[i].BackGround=1;
                EXE[i].VCommand[0][strlong]='\0';
            }
        }
    }

    errorBG = IncorrectBackG( N );
    if( errorBG<0 )
        return( errorBG );

    /* A partir de aqui y solo si no ha habido error con el background
       entra en juego la separacion de comandos */

    /* Recorro todas las estructuras activas */
    for( i=0; i<N; i++ )
    {
        j = 0;
        param = 0;

        while( EXE[i].VCommand[0][j]!='\0' )
        {
            while( isspace(EXE[i].VCommand[0][j]) ) /* Corro cadena a izquierda 1 */
                strmvback( &EXE[i].VCommand[0][j], 1 );

            if( (EXE[i].VCommand[0][j]=='2') && (EXE[i].VCommand[0][j+1]=='>') )
            {
                EXE[i].VCommand[0][j]='\0';
                j++;
                strmvback( &EXE[i].VCommand[0][j], 1 );	/* Me cargo cadena 2> */
                /* Puede entrar >, chr, spc, \0 */

                if( EXE[i].VCommand[0][j]=='>' ) { /*------- 2>> -------*/
                    EXE[i].VCommand[0][j]='\0'; /* Me cargo caracter > */
                    j++;
                    while( isspace(EXE[i].VCommand[0][j]) )
                        strmvback( &EXE[i].VCommand[0][j], 1 );
                    if( EXE[i].VCommand[0][j]=='\0' )
                        return(-5);  /* Syntax error near unespected token '2>>' */
                    EXE[i].Redirecciones[4]=&EXE[i].VCommand[0][j];
                    while( (!isspace(EXE[i].VCommand[0][j]) )
                    && (EXE[i].VCommand[0][j]!='\0')
                    && (EXE[i].VCommand[0][j]!='<' )
                    && (EXE[i].VCommand[0][j]!='>' ) )
                        j++;

                    if( isspace(EXE[i].VCommand[0][j]) ) {
                        EXE[i].VCommand[0][j]='\0';
                        j++;
                    }
                }
                else{ /* -------- 2> --------- */
                    while( isspace(EXE[i].VCommand[0][j]) )
                        strmvback( &EXE[i].VCommand[0][j], 1 );
                    if( EXE[i].VCommand[0][j]=='\0' )
                        return(-4);  /* Syntax error near unespected token '2>' */
                    EXE[i].Redirecciones[3]=&EXE[i].VCommand[0][j];
                    while( (!isspace(EXE[i].VCommand[0][j]) )
                    && (EXE[i].VCommand[0][j]!='\0')
                    && (EXE[i].VCommand[0][j]!='<' )
                    && (EXE[i].VCommand[0][j]!='>' ) )
                        j++;
                    if( isspace(EXE[i].VCommand[0][j]) ){
                        EXE[i].VCommand[0][j]='\0';
                        j++;
                    }
                }
            } /* Ya tenemos cubiertos 2> y 2>> */
            else {
                if( (EXE[i].VCommand[0][j]=='<') ){ /* ----------- < ------------- */
                    EXE[i].VCommand[0][j]='\0';  /* Me cargo caracter < */
                        j++;

                    while( isspace(EXE[i].VCommand[0][j]) )
                        strmvback( &EXE[i].VCommand[0][j], 1 );
                    if( EXE[i].VCommand[0][j]=='\0' )
                        return(-1);  /* Syntax error near unespected token '<' */
                    EXE[i].Redirecciones[0]=&EXE[i].VCommand[0][j];
                    while( (!isspace(EXE[i].VCommand[0][j]) )
                    && (EXE[i].VCommand[0][j]!='\0')
                    && (EXE[i].VCommand[0][j]!='>')
                    && (EXE[i].VCommand[0][j]!='<' ) )
                        j++;

                    if( isspace(EXE[i].VCommand[0][j]) ){
                        EXE[i].VCommand[0][j]='\0';
                        j++;
                    }
                } /*if --------- < ----------- */
                else{
                    if( (EXE[i].VCommand[0][j]=='>') ){
                        EXE[i].VCommand[0][j]='\0';  /* Me cargo caracter > */
                        j++;

                        if( EXE[i].VCommand[0][j]=='>' ){ /*------- >> -------*/
                            EXE[i].VCommand[0][j]='\0';/* Me cargo caracter > */
                            j++;

                            while( isspace(EXE[i].VCommand[0][j]) )
                                strmvback( &EXE[i].VCommand[0][j], 1 );
                            if( EXE[i].VCommand[0][j]=='\0' )
                                return(-3);  /* Syntax error near unespected token '>>' */
                            EXE[i].Redirecciones[2]=&EXE[i].VCommand[0][j];
                            while( (!isspace(EXE[i].VCommand[0][j]) )
                            && (EXE[i].VCommand[0][j]!='\0')
                            && (EXE[i].VCommand[0][j]!='>')
                            && (EXE[i].VCommand[0][j]!='<' ) )
                                j++;
                            if( isspace(EXE[i].VCommand[0][j]) ){
                                EXE[i].VCommand[0][j]='\0';
                                j++;
                            }
                        }
                        else{ /* -------- > --------- */
                            while( isspace(EXE[i].VCommand[0][j]) )
                                strmvback( &EXE[i].VCommand[0][j], 1 );
                            if( EXE[i].VCommand[0][j]=='\0' )
                                return(-2);  /* Syntax error near unespected token '>' */
                            EXE[i].Redirecciones[1]=&EXE[i].VCommand[0][j];
                            while( (!isspace(EXE[i].VCommand[0][j]) )
                            && (EXE[i].VCommand[0][j]!='\0')
                            && (EXE[i].VCommand[0][j]!='>')
                            && (EXE[i].VCommand[0][j]!='<' ) )
                                j++;
                            if( isspace(EXE[i].VCommand[0][j]) ){
                                EXE[i].VCommand[0][j]='\0';
                                j++;
                            }
                        } /* else ------- > --------- */
                    } /* if */
                    else{ /* En este punto solo pueden haber letras o \0 */
                        if( EXE[i].VCommand[0][j]!='\0' ){
                            EXE[i].VCommand[param]=&EXE[i].VCommand[0][j];
                            param++;
                            while( (!isspace(EXE[i].VCommand[0][j]) )
                            && (EXE[i].VCommand[0][j]!='\0')
                            && (EXE[i].VCommand[0][j]!='<')
                            && (EXE[i].VCommand[0][j]!='>') )
                                j++;
                            if( isspace(EXE[i].VCommand[0][j]) ){
                                EXE[i].VCommand[0][j]='\0';
                                j++;
                            }
                        } /* if */
                    } /* else */
                } /* else */
            } /* else */
        } /* while */
    } /* for */

    /*
    * Devuelvo el numero de procesos que hay que generar.
    *
    */
    return( N );

} /*RellenarEXE*/

