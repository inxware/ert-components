/***************************************************************
 * Copyright (C) 2008-2022 inx limited, UK - All Rights Reserved
 * You may use, distribute and modify this code under the terms
 * of the LGPLv3 license. You should have received a copy of the
 * LGPLv3 (GNU LESSER GENERAL PUBLIC LICENSE Version 3) license with this file. If
 * not, please visit
 *	<https://www.gnu.org/licenses/lgpl-3.0.txt>
 ***************************************************************/

#ifndef HAVE_CONFIGFILE_H
#define HAVE_CONFIGFILE_H

extern unsigned int debug_level;

#define DEBUGPRINTF(t, s, a...) if (stderr && debug_level > 1){ \
                                  fprintf(stderr, "%s: %s(): ", t, __FUNCTION__); \
                                  fprintf(stderr, s "\n", ##a); \
                                }
#define TRACE(s, a...) if (debug_level & HID_DEBUG_TRACES) { DEBUGPRINTF("  TRACE", s, ##a) }
#define NOTICE(s, a...) if (debug_level & HID_DEBUG_NOTICES) { DEBUGPRINTF(" NOTICE", s, ##a) }
#define WARNING(s, a...) if (debug_level & HID_DEBUG_WARNINGS) { DEBUGPRINTF("WARNING", s, ##a) }
#define ERROR(s, a...) if (debug_level & HID_DEBUG_ERRORS) { DEBUGPRINTF("  ERROR", s, ##a) }
#define MESSAGE(s, a...) DEBUGPRINTF("  MESSAGE", s, ##a)

#endif
