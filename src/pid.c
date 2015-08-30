#include "pid.h"

/****************************************************************
* Inicializo la estructura de los pids al iniciar el shell.     *
****************************************************************/
void pid_init()
{
    int i;

    PID.next_empty = 0;
    for( i=0; i<NR_PID; i++ )
    PID.l_pid[i] = 0;

} /* pid_init */


/****************************************************************
* Dice si la estructura de pids esta vacia.                     *
****************************************************************/
int pid_empty()
{
    return( PID.next_empty==0 );

} /* pid_empty */


/****************************************************************
* Dice si la estructura de pids esta llena.                     *
****************************************************************/
int pid_full()
{
    return( PID.next_empty==NR_PID );

} /* pid_full */


/****************************************************************
* Inserta pid añadiendolo al final de la estructura.            *
****************************************************************/
void pid_put( int pid_bkg )
{
    if( !pid_full() ){
        PID.l_pid[PID.next_empty]=pid_bkg;
        PID.next_empty++;
    }

} /* pid_put */


/****************************************************************
* Hace una espera para cada uno de los pids que hay en PID      *
****************************************************************/
void pid_wait_all( )
{
    int i, STATUS;

    if( !pid_empty() ){
        for( i=0; i<PID.next_empty; i++ )
            waitpid( PID.l_pid[i], &STATUS, 0 );
    }

} /* pid_wait_all */

