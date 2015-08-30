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
* GESTORES.H
* En este modulo se definen todos los gestores de excepciones que voy a manejar
* en este shell
*
*/

#ifndef _GESTORES_H
#define _GESTORES_H

#include <stdio.h>
#include <signal.h>
#include <stdlib.h>

#define TIMEALARM 30 /*Segundos para alarm()*/
#define OFF       0  /*Alarm( OFF ) desactiva alarmas*/

extern char *shNAME;

void gestor_SIGALRM();

#endif /* GESTORES_H */
