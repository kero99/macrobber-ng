/*
 * ifError.h
 *
 * (c) Author: David Quiroga
 * e-mail:  david [at] clibre [dot] io
 * 
 ****************************************************************
 * Descripción:
 *
 * Macro para gestión sencilla de errores
 * 
 * SPDX-License-Identifier: GPL-3.0
 */

#ifndef IFERROR_FILE_H    /* Para evitar la doble inclusión */
#define IFERROR_FILE_H

#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>
#include <string.h>
#include <errno.h>
#include <fcntl.h> 

static char error_mode = 'e';
static FILE *error_log = NULL;

/* Macro para la gestión de errores */
#define ifError(assertion, ...) ({ 										\
	if(assertion) { 															\
		int errsv = errno;													\
		fprintf (error_log ? error_log : stderr, "[%s:%d]:"		\
			" \n  [+] ", __FILE__, __LINE__);							\
		fprintf (error_log ? error_log : stderr, __VA_ARGS__ ); 	\
	  	if (errsv != 0)    													\
			fprintf (error_log ? error_log : stderr,					\
						"  [+] errno: [%s]\n", strerror(errsv));		\
		if (error_mode =='s') abort();									\
      else if (error_mode == 'e') exit(EXIT_FAILURE);          \
      error_mode = 'e'; errno = 0;                             \
	} 																				\
}) 

#endif
