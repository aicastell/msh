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
* PID.H
* Este modulo implementa todas las operaciones que se vayan a
* realizar con los pids en background.
*
*/

#ifndef _PID_H
#define _PID_H

#include <sys/types.h>
#include <sys/wait.h>

#define NR_PID	100	/* Hacer que sea variable de entorno */

struct pid_struct {
    int l_pid[NR_PID]; /* Lista de pids en background */
    int next_empty; /* El indice del siguiente pid vacio */
} PID;

void pid_init();
int pid_full();
int pid_empty();
void pid_put( int pid_bkg );
void pid_wait_all();

#endif /* _PID_H */

