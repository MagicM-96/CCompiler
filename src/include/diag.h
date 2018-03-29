#ifndef _DHBWCC_DIAG_H
#define _DHBWCC_DIAG_H

typedef enum {
  OUT_OF_MEMORY = 1,
  FILE_ACCESS = 2,
  PROCESS_RESOURCES = 3,
  /* add new error codes above */
  NUM_OS_ERRORS
} os_error_t;

extern void fatal_os_error (os_error_t err, int errnum, const char *fn, const int line, const char *msg, ...);
#define FATAL_OS_ERROR(err,errnum,...) fatal_os_error(err, errnum, __FILE__, __LINE__, __VA_ARGS__);

typedef enum {
  INVALID_INPUT = 1,
  INVALID_SYNTAX = 2,
  INVALID_SEMANTICS = 3,
  /* add new error codes above */
  NUM_COMPILER_ERRORS
} compiler_error_t;

extern void fatal_compiler_error (compiler_error_t err, int errnum, const char *msg, ...);
#define FATAL_COMPILER_ERROR(err,errnum,...) fatal_compiler_error(err, errnum, __VA_ARGS__);

#endif
