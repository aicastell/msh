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
* ERRORES.H
* Este modulo se encargara de tratar todos los errores que
* ocurran al realizar alguna llamada al sistema, ademas de
* todos los mensajes de error que detecte el shell.
*
*/

#ifndef _ERRORES_H
#define _ERRORES_H

#include <stdio.h>
#include <stdlib.h>
#include <errno.h>

#include "gexe.h"

extern char *shNAME;

void TratarError( char *syscall );
int IncorrecToken( char *Line );
int IncorrectBackG( int Nprocesos );
void msg_error( int coderr );

#endif /* _ERRORES_H */

