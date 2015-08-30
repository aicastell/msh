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
* ENTORNO.H
* Este modulo implementa todas las operaciones que se vayan
* a realizar con las variables de entorno.
*
*/

#ifndef _ENTORNO_H
#define _ENTORNO_H

#include "str.h"
#include "gexe.h"

#define NR_ENTORNO	100	/* TODO Hacer que sea una macro de compilación */

extern char **environ;
extern char *shNAME;

struct entorno {
    char *nombre;	/* nombre de la variable de entorno */
    char *valor;	/* valor de la variable de entorno */
    int exported;	/* ¿es una variable exportada? */
};

struct entorno_struct {
    struct entorno l_entorno[NR_ENTORNO];
    int next_empty;
} ENTORNO;

void entorno_init();
int entorno_empty();
int entorno_full();
void entorno_put( char *cadena, int exported );
void entorno_display( int all );
void entorno_export( char *cadena );
void entorno_loadefault();
void entorno_to_environ();
void entorno_to_EXE( int nprocess );

#endif /* _ENTORNO_H */

