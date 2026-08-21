#ifndef EHS_BANNER_H
#define EHS_BANNER_H

#include "globals.h"

/* Per-line output sink supplied by the target serial console.
 * The function is called once per line of banner / help output. The line
 * may contain ANSI escape sequences (colour); targets free to strip or
 * pass through. The string is NUL-terminated and does not include the
 * trailing newline — the sink is expected to add one. */
typedef void (*ehs_console_println_fn)(const char *line);

/**
 * EhsBanner_PrintHelp — print the inxware banner, tagline and key-command
 * help to a *human* serial console, via the supplied println sink.
 */
void EhsBanner_PrintHelp(ehs_console_println_fn println);

#endif /* EHS_BANNER_H */
