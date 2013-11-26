#include <gcl/error.h>

#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define PROGNAME_BUFSIZE 64

static char program_name[PROGNAME_BUFSIZE] = "";

/* This function is not thread-safe! */
void gcl_set_program_name(const char *name)
{
	strncpy(program_name, name, PROGNAME_BUFSIZE - 1);
	program_name[PROGNAME_BUFSIZE - 1] = '\0';
}

void _gcl_warning(const char *file, unsigned int line, const char *func,
		  const char *format, ...)
{
	va_list args;

	fflush(stdout);

	if (strlen(program_name) > 0)
		fprintf(stderr, "%s (%s:%u, %s): ", program_name, file, line, func);

	va_start(args, format);
	vfprintf(stderr, format, args);
	va_end(args);

	fputc('\n', stderr);
}

void _gcl_error(const char *file, unsigned int line, const char *func,
		int errnum, const char *format, ...)
{
	va_list args;

	fflush(stdout);

	if (strlen(program_name) > 0)
		fprintf(stderr, "%s (%s:%u, %s): ", program_name, file, line, func);

	va_start(args, format);
	vfprintf(stderr, format, args);
	va_end(args);

	if (errnum != 0)
		fprintf(stderr, ": %s", strerror(errnum));

	fputc('\n', stderr);

	abort();
}
