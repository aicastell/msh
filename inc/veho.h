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
* VEHO.H
* Este modulo implementa las versiones VErtical y HOrizontal
* de generación de procesos en el shell.
*
*/

#ifndef _VEHO_H
#define _VEHO_H

#include <sys/wait.h>
#include <unistd.h>
#include <stdio.h>
#include <fcntl.h>
#include <stdlib.h>

#include "errores.h"
#include "gexe.h"
#include "pid.h"

#define STDIN  0
#define STDOUT 1
#define STDERR 2

extern char *shNAME;

void veho_run( int mprocess );

#endif /* _VEHO_H */
