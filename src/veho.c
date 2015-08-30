#include "veho.h"

/****************************************************************
 * rEDIRECCIONA EstANdAR iNPUT oUTPUT eRR                       *
 * <(0), >(1), >>(2), 2>(3), 2>>(4)                             *
 * i corresponde con el comando que hay que redireccionar.      *
 ***************************************************************/
void rstdioe(int i)
{
    if( EXE[i].Redirecciones[0]!=NULL ){ /* < */
        close( STDIN );
        open( EXE[i].Redirecciones[0], O_RDONLY );
        TratarError("open");
    }
    if( EXE[i].Redirecciones[1]!=NULL ){ /* > */
        close( STDOUT );
        open( EXE[i].Redirecciones[1], O_WRONLY|O_TRUNC|O_CREAT, 0644 );
        TratarError("open");
    }
    if( EXE[i].Redirecciones[2]!=NULL ){ /* >> */
        close( STDOUT );
        open( EXE[i].Redirecciones[2], O_WRONLY|O_CREAT|O_APPEND, 0644 );
        TratarError("open");
    }
    if( EXE[i].Redirecciones[3]!=NULL ){ /* 2> */
        close( STDERR );
        open( EXE[i].Redirecciones[3], O_WRONLY|O_TRUNC|O_CREAT, 0644 );
        TratarError("open");
    }
    if( EXE[i].Redirecciones[4]!=NULL ){ /* 2>> */
        close( STDERR );
        open( EXE[i].Redirecciones[4], O_WRONLY|O_CREAT|O_APPEND, 0644 );
        TratarError("open");
    }

} /* Redireccionar */



/****************************************************************
 * Esta funcion implementa la version vertical del shell.       *
 ***************************************************************/
void EXEVertical( int nprocess )
{
    int STATUS, TUBO[2][2], i, hijo, ret;

    if ( (hijo=fork()) == 0 ) { /*Proceso hijo*/
        for ( i=nprocess-1; i>= 0; i-- ) {
            if ( i == 0 ) { /* Hemos llegado al ultimo comando */
                rstdioe(i);
                execvp( EXE[i].VCommand[0], &EXE[i].VCommand[0] );
                TratarError("execvp");
                exit(0);
            }
            else {
                ret = pipe( TUBO[i%2] );
                if (ret == -1) {
                    printf("Error en pipe\n");
                    exit(0);
                }

                if( fork() != 0 ) { /* Proceso Padre del nieto */
                    close( STDIN );	/* Cerramos Entrada Standart */
                    ret = dup( TUBO[i%2][0] );
                    if (ret == -1) {
                        printf("Error en dup\n");
                        exit(0);
                    }
                    close( TUBO[i%2][0] );
                    close( TUBO[i%2][1] );
                    rstdioe(i);
                    execvp( EXE[i].VCommand[0], &EXE[i].VCommand[0] );
                    TratarError("execvp");
                    exit(0);
                }
                else { /* Proceso nieto */
                    close( STDOUT ); /* Cerramos la Salida Standart */
                    ret = dup( TUBO[i%2][1] );
                    if (ret == -1) {
                        printf("Error en dup\n");
                        exit(0);
                    }
                    close( TUBO[i%2][1] );
                    close( TUBO[i%2][0] );
                } /* end else */
            } /* end else */
        } /* end for */
    }
    /*
    * El padre continua desde aqui
    * esperando a hijo si es necesario.
    */
    else {
        if( !EXE[nprocess-1].BackGround )
            while( hijo!=wait( &STATUS ) );
        else
            pid_put( hijo );
    }

} /* EXEVertical */


/****************************************************************
 * Esta funcion implementa la version horizontal del shell.     *
 ***************************************************************/
void EXEHorizontal( int nprocess )
{
    int STATUS, TUBO[2][2], i, hijo, ret;

    for( i=0; i<nprocess; i++)
    {
        if( i<(nprocess-1) ) {
            ret = pipe( TUBO[i%2] );
            if (ret == -1) {
                printf("Error en pipe\n");
                exit(0);
            }
        }

        if( (hijo=fork())==0 ) { /*Proceso hijo*/
            if( i==(nprocess-1) ) { /* Ultimo hijo antes de ejecutar */
                if( nprocess>1 ){ /* Si hay mas de un proceso hay que leer de TUBO */
                close( STDIN );
                ret = dup( TUBO[(i-1)%2][0] );
                if (ret == -1) {
                    printf("Eror en dup\n");
                }
                close( TUBO[(i-1)%2][0] );
                close( TUBO[(i-1)%2][1] );
                }
                rstdioe(i);
                execvp( EXE[i].VCommand[0], &EXE[i].VCommand[0] );
                TratarError("execvp");
                exit(0); /* Mato al proceso hijo erroneo */
            }
            else {
                if( i==0 ){ /* Primer proceso y quedan mas */
                    /*close( STDIN ); */
                    close( STDOUT );
                    ret = dup( TUBO[i%2][1] );
                    if (ret == -1) {
                        printf("Error en dup\n");
                        exit(0);
                    }
                    close( TUBO[i%2][1] );
                    close( TUBO[i%2][0] );
                    rstdioe(i);
                    execvp( EXE[i].VCommand[0], &EXE[i].VCommand[0] );
                    TratarError("execvp");
                    exit(0); /* Mato al proceso hijo erroneo */
                }
                else { /* Proceso en medio de 2+. Hay que conectar las 2 tuberias */
                    /* Tuberia de la izquierda */
                    close( STDIN );
                    ret = dup( TUBO[(i-1)%2][0] );
                    if (ret == -1) {
                        printf("Error en dup\n");
                        exit(0);
                    }
                    close( TUBO[(i-1)%2][0] );
                    close( TUBO[(i-1)%2][1] );
                    /* Tuberia de la derecha   */
                    close( STDOUT );
                    ret = dup( TUBO[i%2][1] );
                    if (ret == -1) {
                        printf("Error en dup\n");
                        exit(0);
                    }
                    close( TUBO[i%2][1] );
                    close( TUBO[i%2][0] );
                    rstdioe(i);
                    execvp( EXE[i].VCommand[0], &EXE[i].VCommand[0] );
                    TratarError("execvp");
                    exit(0); /* Mato al proceso hijo erroneo */
                }
            }
        }
        else {
            /*
            * Por aqui continua el padre sincronizando
            * la transferencia con sus hijos uno a uno.
            */
            if( i<nprocess-1 ) { /* i no es el ultimo proceso */
                while (hijo!=wait( &STATUS )); /*Padre se bloquea esperando a su hijo*/
                if( nprocess>1 ){ /*Existen tuberias de por medio */
                    close( TUBO[i%2][1] );
                }
            }
            else {
                if( !EXE[nprocess-1].BackGround )
                    while( hijo!=wait( &STATUS ) );
                else
                    pid_put( hijo );
            }
        }

    }/* for */
} /* EXEHorizontal */


/****************************************************************
* Esta funcion selecciona la version que se va a ejecutar de    *
* acuerdo con la variable VERSION del entorno environ.          *
****************************************************************/
void veho_run( int nprocess )
{
    if( !strcmp(getenv("VERSION"), "vertical") ) {
        EXEVertical( nprocess );
        return;
    }

    if( !strcmp(getenv("VERSION"), "horizontal") ) {
        EXEHorizontal( nprocess );
        return;
    }

    printf("%s: invalid value for VERSION.\nVERSION=[vertical, horizontal]\n", shNAME);

} /* veho_run */
