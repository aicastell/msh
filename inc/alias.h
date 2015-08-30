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
* ALIAS.H
* Este modulo implementa todas las operaciones que se realizan con
* los alias
*
*/

#ifndef _ALIAS_H
#define _ALIAS_H

#include "str.h"
#include "gexe.h"

#define NR_ALIAS     10	/* Hacer que sea variable de entorno */

struct alias  {
    char *nombre;
    char *apodo;
};

struct alias_struct {
    struct alias l_alias[NR_ALIAS];	/* Lista de alias */
    int next_empty;	/* El indice del siguiente alias vacio */
} ALIAS;

void alias_init();
int alias_empty();
int alias_full();
void alias_put( char *cadena );
char *alias_get( char *cadena );
void alias_delete( char *cadena );
void alias_loadefault();
void alias_display();
void alias_display_single( char *cadena );
void alias_handle( char **VCommand );
void alias_to_EXE( int nprocess );

#endif /* _ALIAS_H */
