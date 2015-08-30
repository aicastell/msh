#include "internos.h"

/****************************************************************
* Cada una de los comandos internos que tengo implementados     *
* tiene asignado un valor que lo identifica. Los comandos       *
* simples se reconocen con un cero.                             *
****************************************************************/
int internos_code( char *command )
{
    if( strcmp("exit", command)==0 ) return(10);
    if( strcmp("alias", command)==0 ) return(20);
    if( strcmp("unalias", command)==0 ) return(30);
    if( strcmp("cd", command)==0 ) return(40);
    if( strstr(command, "=") ) return(50);
    if( strcmp("set", command)==0 ) return(60);
    if( strcmp("export", command)==0 ) return(70);

    return(0);

} /* internos_code */


/****************************************************************
* Cada una de los comandos internos que tengo implementados     *
* tiene asignado un valor. Lo ejecuto.                          *
****************************************************************/
void internos_run( int command )
{
    int i;

    switch( command )
    {
    case 10:
        if ( !pid_empty() ) {
            printf("%s: awaiting running jobs ...\n", shNAME);
            pid_wait_all();
        }
        i = chdir( initdir );
        exit(0);
        break;

    case 20:
        alias_handle( &EXE[0].VCommand[1] );
        break;

    case 30:
        if( EXE[0].VCommand[1]!=NULL ){
            alias_delete( EXE[0].VCommand[1] );
            i=2;
            while( EXE[0].VCommand[i]!=NULL )
                alias_delete( EXE[0].VCommand[i++] );
        }
        break;

    case 40:
        if( EXE[0].VCommand[1]==NULL ){ /* cd */
            i = chdir( userHOME );
            TratarError( "cd" );
        }
        else{ /* cd dir */
            i = chdir( EXE[0].VCommand[1] );
            TratarError( "cd" );
        }
        break;

    case 50:
        entorno_put( EXE[0].VCommand[0], 0 );
        break;

    case 60:
        entorno_display(1); /* Display todas */
        break;

    case 70:
        if( EXE[0].VCommand[1]==NULL )
            entorno_display(0);	/* Solo los exportados */
        else {
            i = 1;
            while( EXE[0].VCommand[i]!=NULL )
                entorno_export( EXE[0].VCommand[i++] );
        }
        break;

    } /* switch */

} /* internos_run */

