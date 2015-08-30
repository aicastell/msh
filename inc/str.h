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
* STR.H
* Este modulo se encarga de todo lo relativo a las cadenas.
*
*/

#ifndef _STR_H
#define _STR_H

#include <string.h>

#define MAXLENCAD    256

void strmvback( char *cadena, int n );
void strmchr( char chr, char *cadena );

#endif /* _STR_H */

