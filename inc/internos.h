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
* INTERNOS.H
* Esta libreria implementa todas las funciones para detectar y
* ejecutar todos los comandos internos del shell.
*
*/

#ifndef _INTERNOS_H
#define _INTERNOS_H

#include <unistd.h>

#include "str.h"
#include "gexe.h"
#include "alias.h"
#include "errores.h"
#include "entorno.h"
#include "pid.h"

extern char *userHOME;
extern char *initdir;
extern char *shNAME;

int internos_code( char *command );
void internos_run( int command );

#endif /* _INTERNOS_H */

