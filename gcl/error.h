#ifndef GCL_ERROR_H
#define GCL_ERROR_H

#include <gcl/compat.h>

void gcl_set_program_name(const char *name);

#ifndef gcl_warning
#define gcl_warning(...) \
	_gcl_warning(__FILE__, __LINE__, __func__, __VA_ARGS__)
#endif

#ifndef gcl_error
#define gcl_error(errnum, ...) \
	_gcl_error(__FILE__, __LINE__, __func__, errnum, __VA_ARGS__)
#endif

void _gcl_warning(const char *file, unsigned int line, const char *func,
		  const char *format, ...)
	GCL_FORMAT_STRING(4);

void _gcl_error(const char *file, unsigned int line, const char *func,
		int errnum, const char *format, ...)
	GCL_FORMAT_STRING(5)
	GCL_NO_RETURN;

#endif
