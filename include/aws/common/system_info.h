#ifndef AWS_COMMON_SYSTEM_INFO_H
#define AWS_COMMON_SYSTEM_INFO_H

/**
 * Copyright Amazon.com, Inc. or its affiliates. All Rights Reserved.
 * SPDX-License-Identifier: Apache-2.0.
 */

#include <aws/common/common.h>

enum aws_platform_os {
    AWS_PLATFORM_OS_WINDOWS,
    AWS_PLATFORM_OS_MAC,
    AWS_PLATFORM_OS_UNIX,
};

AWS_EXTERN_C_BEGIN

/* Returns the OS this was built under */
AWS_COMMON_API
enum aws_platform_os aws_get_platform_build_os(void);

/* Returns the number of online processors available for usage. */
AWS_COMMON_API
size_t aws_system_info_processor_count(void);

/* Returns true if a debugger is currently attached to the process. */
AWS_COMMON_API
bool aws_is_debugger_present(void);

/* If a debugger is attached to the process, trip a breakpoint. */
AWS_COMMON_API
void aws_debug_break(void);

#if defined(AWS_HAVE_EXECINFO) || defined(_WIN32) || defined(__APPLE__)
#    define AWS_BACKTRACE_STACKS_AVAILABLE
#endif

/*
 * Records a stack trace from the call site.
 * Returns the number of stack entries/stack depth captured, or 0 if the operation
 * is not supported on this platform
 */
AWS_COMMON_API
size_t aws_backtrace(void **frames, size_t num_frames);

/*
 * Converts stack frame pointers to symbols, if symbols are available
 * Returns an array up to stack_depth long, that needs to be free()ed.
 * stack_depth should be the length of frames.
 * Returns NULL if the platform does not support stack frame translation
 * or an error occurs
 */
char **aws_backtrace_symbols(void *const *frames, size_t stack_depth);

/*
 * Converts stack frame pointers to symbols, using all available system
 * tools to try to produce a human readable result. This call will not be
 * quick, as it shells out to addr2line or similar tools.
 * On Windows, this is the same as aws_backtrace_symbols()
 * Returns an array up to stack_depth long that needs to be free()ed. Missing
 * frames will be NULL.
 * Returns NULL if the platform does not support stack frame translation
 * or an error occurs
 */
char **aws_backtrace_addr2line(void *const *frames, size_t stack_depth);

/**
 * Print a backtrace from either the current stack, or (if provided) the current exception/signal
 *  call_site_data is siginfo_t* on POSIX, and LPEXCEPTION_POINTERS on Windows, and can be null
 */
AWS_COMMON_API
void aws_backtrace_print(FILE *fp, void *call_site_data);

/* Log the callstack from the current stack to the currently configured aws_logger */
AWS_COMMON_API
void aws_backtrace_log(void);

AWS_EXTERN_C_END

#endif /* AWS_COMMON_SYSTEM_INFO_H */
