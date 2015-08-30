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
* GEXE.H (Gestion de struct EXE)
* Este modulo define la estructura EXE, en la cual se
* almacenan todos los comandos de la linea de comandos.
* Ademas se implementan todas las funciones necesarias para su
* correcta gestion.
*
*/

#ifndef _GEXE_H
#define _GEXE_H

#include <string.h>
#include <stdio.h>
#include <ctype.h>

#include "str.h"
#include "errores.h"

#define MAXCOMMANDS   5
#define MAXPARAMS     5

/*
 * Valores pequeños intentado minimizar el uso de memoria. Si es
 * necesario se puede utilizar mas.
 */

struct comando_struct {
    char *VCommand[MAXPARAMS]; /* Vector que se pasa al execvp */
    int BackGround;            /* Verdadero si comando lleva ampersand */
    char *Redirecciones[5];    /* <, >, >>, 2>, 2>>            */
} EXE[MAXCOMMANDS];

int gexe_rellenar( char LineCommand[] );

#endif /* _GEXE_H */

