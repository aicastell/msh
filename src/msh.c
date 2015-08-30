/**
* msh: My Shell
* Copyright (C) 2015
* Angel Ivan Castell Rovira <al004140 at gmail dot com>
*
* This file is part of msh.
*
* msh is free software: you can redistribute it and/or modify
* it under the terms of the GNU General Public License as published by
* the Free Software Foundation, either version 3 of the License, or
* (at your option) any later version.
*
* msh is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
* GNU General Public License for more details.
*
* You should have received a copy of the GNU General Public License
* along with msh.  If not, see <http://www.gnu.org/licenses/>.
*
*
* MSH.C
* Este programa implementa un interprete de comandos (shell)
* sencillo para trabajar en un entorno UNIX/LINUX.
*
*/

#include <stdio.h>
#include <unistd.h>
#include <signal.h>

#include "str.h"
#include "gestores.h"
#include "errores.h"
#include "gexe.h"
#include "veho.h"
#include "alias.h"
#include "internos.h"
#include "entorno.h"
#include "pid.h"

char *shNAME; /* Variable glogal que contiene nombre del shell. */
char *initdir; /* El directorio donde se inicia el shell. */
char *userHOME; /* El home del usuario que ejecuta el shell. */


/**************************************************************
* Esta funcion reserva memoria para todas las variables que lo*
* requieren e inicializa todas las estructuras globales que he*
* definido como son ALIAS, ENTORNO y PID.                     *
**************************************************************/
void LoadShell( char *EXENAME )
{
    char buf[MAXLENCAD];

    printf( "Loading %s shell.\n", EXENAME );
    printf( "Please, wait a moment... \n\n" );

    shNAME = strdup( EXENAME );

    initdir = strdup( getcwd(buf, MAXLENCAD) );

    userHOME = strdup( getenv("HOME") );
    chdir(userHOME);

    alias_init();
    alias_loadefault();

    entorno_init();

    entorno_loadefault();

    pid_init();

}/*LoadShell*/


/****************************************************************
* Esta funcion imprime el prompt que usa el shell.              *
****************************************************************/
void Prompt()
{
    char HOSTNAME[MAXLENCAD];
    char *diractual, buf[MAXLENCAD];

    diractual=getcwd( buf, MAXLENCAD );
    gethostname(HOSTNAME, MAXLENCAD);
    printf("%s:%s\\>", HOSTNAME, diractual);

}/*Prompt*/


/*****************************************************************
* Programa principal                                             *
*****************************************************************/
int main( int argc, char **argv )
{
    char LINECOMMAND[MAXLENCAD];
    int NCOMMAND;
    int code;

    LoadShell( argv[0] );
    signal( SIGALRM, gestor_SIGALRM ); /* Se activa mediante alarm() */

    while ( 1 )
    {
        Prompt();

        alarm( TIMEALARM );     /* Activo una alarma a los 30 sg */

        memset(LINECOMMAND, 0, MAXLENCAD);
        fgets( LINECOMMAND, MAXLENCAD, stdin );    /* Leer una linea del stdin terminada en EOL */

        alarm( OFF );           /* Desactivo SIGALRM mientras estoy ejecutando */

        NCOMMAND=gexe_rellenar( LINECOMMAND );

        if( NCOMMAND>0 ){
            code=internos_code( EXE[0].VCommand[0] );
            entorno_to_EXE( NCOMMAND );
            if( code>0 )
            internos_run( code ); /* En internal.c.h */
            else { /* code=0 */
                alias_to_EXE( NCOMMAND );	/* Sustituyo alias por valores reales */
                veho_run( NCOMMAND );	/* Ejecuta version apropiada segun VERSION */
            }
        }
        else {
            if( NCOMMAND<0 )
                msg_error( NCOMMAND );
        }
    }

}/* main */

