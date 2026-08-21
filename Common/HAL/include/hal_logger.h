/***************************************************************
 * Copyright (C) 2008-2022 inx limited, UK - All Rights Reserved
 * You may use, distribute and modify this code under the terms
 * of the LGPLv3 license. You should have received a copy of the
 * LGPLv3 (GNU LESSER GENERAL PUBLIC LICENSE Version 3) license with this file. If
 * not, please visit
 *	<https://www.gnu.org/licenses/lgpl-3.0.txt>
 ***************************************************************/

/** @file logger.h
 * Declares the interface for the logger module.
 *
 * @author: inx limited
 *
 */

#ifndef EHS_HAL_LOGGER_H
#define EHS_HAL_LOGGER_H

/*****************************************************************************/
/* Included files */
#include "globals.h"
#include "hal_string.h"
#include "hal_time.h"      /* EhsTickType + EHS_TIME_ms — used by EHS_TRUSTLESS_*_FATAL */
#include "hal_console.h"   /* EhsConsolePrintf — used by EHS_TRUSTLESS_*_FATAL */
#include "hal_logger.h"
/*****************************************************************************/
/* Define macros  */
/**
 * Filename used for logging
 */
#define EHSH_LOG_FILENAME "ehs_log.000" /* This is to be postpended with an index */

/**
 * Maximum number of entries in a log file
 */
#define EHSH_LOG_MAX_MSG 2048u

/**
 * Maximum number of indexed log files
 */
//#define EHSH_LOG_MAX_MSG 4u


/**
 * Maximum length of a message sent to the logger
 */
//#define EHSH_LOG_MAX_MSG 256u

/**
 * Default log level
 */
#ifndef EHSH_LOG_DEFAULT_LEVEL
#define EHSH_LOG_DEFAULT_LEVEL (EHSH_LOG_LEVEL_ERROR)
#endif
//#define EHSH_LOG_DEFAULT_LEVEL (EHSH_LOG_LEVEL_ERROR | EHSH_LOG_LEVEL_WARNING)
//#define EHSH_LOG_DEFAULT_LEVEL (EHSH_LOG_DEFAULT_LEVEL | EHSH_LOG_LEVEL_INFO | EHSH_LOG_LEVEL_ENTER | EHSH_LOG_LEVEL_EXIT)


/**
 * All log levels
 */
// Added to ENUM NOW#define EHSH_LOG_ALL_LEVEL (EHSH_LOG_DEFAULT_LEVEL | EHSH_LOG_LEVEL_INFO | EHSH_LOG_LEVEL_INFO | EHSH_LOG_LEVEL_ENTER| EHSH_LOG_LEVEL_EXIT)

/*****************************************************************************/
/* Define types */

/**
 * Specifies the level at which to log information
 */
typedef enum
{
    EHSH_LOG_LEVEL_NONE	    = 0x0,
    EHSH_LOG_LEVEL_ERROR	= 0x01,
    EHSH_LOG_LEVEL_WARNING	= 0x02,
    EHSH_LOG_LEVEL_INFO		= 0x04,
    EHSH_LOG_LEVEL_ENTER	= 0x08,
    EHSH_LOG_LEVEL_EXIT		= 0x10,
    EHSH_LOG_LEVEL_ALL = 0xFF
} EhsHLoggerLogLevel;


/**
 * Specifies module ids for specific logging.
 * *************************************************
 * Don't forget to add
 * module text name in EhsLModuleNames in logger.c
 * **************************************************
 */
typedef enum
{
    EHSH_LOG_MODULE_UNDEFINED,
    EHSH_LOG_MODULE_KERNEL,
    EHSH_LOG_MODULE_GRAPHICS,
    EHSH_LOG_MODULE_LOGGER,
    EHSH_LOG_MODULE_HAL_MEMORY,
    EHSH_LOG_MODULE_HAL_PROCESS,
    EHSH_LOG_MODULE_HAL_STRING,
    EHSH_LOG_MODULE_TGT_VIEWPORT,
    EHSH_LOG_MODULE_HAL_NETWORK,
    EHSH_LOG_MODULE_HAL_DEVMANMON,
    EHSH_LOG_MODULE_HAL_FILE,
    EHSH_LOG_MODULE_HAL_CONSOLE,
    EHS_LOG_MODULE_QUANTITY /* final entry - used to determine size of EhsLModuleNames */
} EhsHLoggerModuleId;

/**
 * Which header fields EHSH_LOG_* output includes, per printed line. Bitmask - OR values
 * together in EHS_LOGGER_REPORT_FIELDS (config.mk), e.g.:
 *   EHS_LOGGER_REPORT_FIELDS=(EHS_LOGGER_REPORT_MODULE|EHS_LOGGER_REPORT_SEVERITY)
 * Omitting EHS_LOGGER_REPORT_SOURCEFILE / EHS_LOGGER_REPORT_SOURCE_LINENO also removes
 * __FILE__ / __LINE__ from every EHSH_LOG_* call site's expansion (see EHSH_LOG_MESSAGE
 * below) - not just from the printed line - which is where the actual code-size win is:
 * __FILE__ is a full path string literal baked into the binary at each of the (many) call
 * sites, regardless of whether that line ever prints.
 *
 * These MUST be #define, not a C enum: they're evaluated inside #if directives below (to
 * resolve EHS_LOGGER_HEADER_FORMAT and EHS_LOGGER_FILE_ARG/_LINE_ARG at compile time with
 * zero runtime cost), and the preprocessor cannot see enum members at all - an identifier
 * that isn't a macro evaluates to 0 inside #if, silently. A first version of this used an
 * enum here and every #if (FIELDS & EHS_LOGGER_REPORT_X) was `#if (FIELDS & 0)` - always
 * false - which silently dropped every field except the always-on timestamp.
 */
#define EHS_LOGGER_REPORT_NONE          0x00u
#define EHS_LOGGER_REPORT_MODULE        0x01u
#define EHS_LOGGER_REPORT_SEVERITY      0x02u
#define EHS_LOGGER_REPORT_SOURCEFILE    0x04u
#define EHS_LOGGER_REPORT_SOURCE_LINENO 0x08u
#define EHS_LOGGER_REPORT_ALL           0x0Fu

#ifndef EHS_LOGGER_REPORT_FIELDS
#define EHS_LOGGER_REPORT_FIELDS EHS_LOGGER_REPORT_ALL
#endif

/* Per-field format fragments, resolved once at compile time - so a disabled field costs
 * nothing at runtime, not even a branch. A "hidden" field still consumes its argument
 * (a zero-precision conversion - %.0s / %.0d - accepts the value but emits zero
 * characters) rather than being removed from the format string entirely: several targets'
 * EhsStdioPrintf (e.g. esp32/esp32s3) reuse these exact positional arguments to route to
 * the right native log level (ESP_LOGE/W/I/D) by inspecting the severity argument, so the
 * argument COUNT and TYPE at each position must stay fixed regardless of what's visible.
 * See docs/ert-porting-guide.md "Debug Logging" for the full explanation. */
#if (EHS_LOGGER_REPORT_FIELDS & EHS_LOGGER_REPORT_MODULE)
#define EHS_LOGGER_FMT_MODULE "[%s]"
#else
#define EHS_LOGGER_FMT_MODULE "%.0s"
#endif
#if (EHS_LOGGER_REPORT_FIELDS & EHS_LOGGER_REPORT_SEVERITY)
#define EHS_LOGGER_FMT_SEVERITY "[%s]"
#else
#define EHS_LOGGER_FMT_SEVERITY "%.0s"
#endif
#if (EHS_LOGGER_REPORT_FIELDS & EHS_LOGGER_REPORT_SOURCEFILE)
#define EHS_LOGGER_FMT_SOURCEFILE "[%s]"
#else
#define EHS_LOGGER_FMT_SOURCEFILE "%.0s"
#endif
#if (EHS_LOGGER_REPORT_FIELDS & EHS_LOGGER_REPORT_SOURCE_LINENO)
#define EHS_LOGGER_FMT_SOURCE_LINENO ":%d"
#else
#define EHS_LOGGER_FMT_SOURCE_LINENO "%.0d"
#endif

/* [time] is always shown (not part of this bitmask) and, on targets whose EhsStdioPrintf
 * inspects the severity argument for level routing (esp32/esp32s3), must remain the very
 * next specifier after it - EHS_LOGGER_FMT_SEVERITY is deliberately second here.
 *
 * The newline is trailing, not leading (a pre-existing leading "\n" here predated this
 * whole feature and meant each entry's separator only appeared at the START of the NEXT
 * call - fine on a bare stdout stream, but on esp32/esp32s3 it lands the entry on the line
 * AFTER ESP-IDF's own "E (uptime) tag: " prefix instead of after it, splitting every entry
 * across two lines). Trailing means each call closes its own line on every target. */
#define EHS_LOGGER_HEADER_FORMAT \
    "[%u]" EHS_LOGGER_FMT_SEVERITY EHS_LOGGER_FMT_MODULE EHS_LOGGER_FMT_SOURCEFILE EHS_LOGGER_FMT_SOURCE_LINENO ":\"%s\"\n"

/* __FILE__/__LINE__ for EHSH_LOG_MESSAGE below - omitted entirely (NULL/0) when the
 * corresponding report field is compiled off, so the string literal isn't emitted at all. */
#if (EHS_LOGGER_REPORT_FIELDS & EHS_LOGGER_REPORT_SOURCEFILE)
#define EHS_LOGGER_FILE_ARG __FILE__
#else
#define EHS_LOGGER_FILE_ARG NULL
#endif
#if (EHS_LOGGER_REPORT_FIELDS & EHS_LOGGER_REPORT_SOURCE_LINENO)
#define EHS_LOGGER_LINE_ARG __LINE__
#else
#define EHS_LOGGER_LINE_ARG 0u
#endif

#ifdef EHS_BUILDOPT_STDIO_MESSAGE_TRACE
#define EHS_TRACE_MESSAGE(x,...)  EhsStdioSimplePrintf(x,##__VA_ARGS__) // THis is for random text
#define EHS_TRACE_FUNCTION(x)   EhsStdioSimplePrintf("%s\n",#x)   //this is for stringifying function names for Enter tracing
#else
#define EHS_TRACE_FUNCTION(x)
#define EHS_TRACE_MESSAGE(...)
#endif


/**
 * Is the current module logging a specific level?
 * @param[in] nLevel logging level to check
 * @return true if current module is logging nLevel
 */
// Check if we are supporting per module level checking 
#ifdef EHSL_MODULE_ID
// #define EHSH_LOG_CHECK(nLevel) (((nLevel) && EhsHLoggerModuleLogLevel[EHSL_MODULE_ID] & (nLevel))) //(1) /* bit check against module for the requested log level */
#define EHSH_LOG_CHECK(nLevel) (EhsHLoggerModuleLogLevel[EHSL_MODULE_ID] & (nLevel)) //(1) /* bit check against module for the requested log level */

#else
#define EHSL_MODULE_ID EHSH_LOG_MODULE_UNDEFINED
#define EHSH_LOG_CHECK(nLevel) 0
#endif

/**
 * Returns EhsHLogger_Msg buffer
 */
ehs_char* EhsHLogger_Buffer();

/**
 * Best-effort check that the calling thread has enough stack margin left to safely run
 * the snprintf-class formatting behind EHSH_LOG_*. True (always safe) on targets with no
 * cheap way to read remaining stack. See EHS_LOGGER_MIN_STACK_BYTES.
 */
ehs_bool EhsHLogger_stackOk();

/**
 * Cheap (no formatting) fallback taken by EHSH_LOG_MESSAGE when EhsHLogger_stackOk()
 * says the normal path isn't safe - counts the drop rather than attempting to print it.
 */
void EhsHLogger_lowStackDrop();

/**
 * Count of log calls skipped by EhsHLogger_lowStackDrop() since boot.
 */
EHS_EXTERN ehs_uint32 EhsHLogger_nLowStackDrops;


//#define EHSH_LOG_MESSAGEX(nLevel,...) {}
/**
 * Writes debug standard out and any debug console that's listening.
 * Generate a specific log message (if we're logging at that level)
 * @todo the following is not thread safe - try with local variable in curlies
 * @todo the else function here is for debug this needs to be dumped... This F*ing logging code has wasted hours....
 */
#if defined(EHS_RUNTIME_LOGGER_ENABLED) && defined(EHSL_MODULE_ID)
 #ifdef EHS_ANDROID
  #define EHSH_LOG_MESSAGE(nLevel,...) if(EHSH_LOG_CHECK(nLevel)){LOGE(__VA_ARGS__);}
 #else
  /* EHS_LOGGER_FILE_ARG/EHS_LOGGER_LINE_ARG resolve to NULL/0 instead of __FILE__/__LINE__
   * when EHS_LOGGER_REPORT_SOURCEFILE/_SOURCE_LINENO are compiled off (see hal_logger.h
   * above) - the __FILE__ path string is then never emitted into the binary at this call
   * site at all, which is where the actual code-size saving comes from.
   *
   * EhsHLogger_stackOk() gates the EhsSnprintf call itself, which is the stack-heavy part
   * (its destination buffer is global, but formatting still recurses through vsnprintf on
   * the CALLER's stack) - a low-stack caller instead takes the cheap EhsHLogger_lowStackDrop()
   * path (a counter increment, no formatting). On targets with no cheap stack-remaining
   * primitive, EhsHLogger_stackOk() always returns true and this is a no-op check. */
  #define EHSH_LOG_MESSAGE(nLevel,...) if(EHSH_LOG_CHECK(nLevel) && EhsHLogger_Buffer()){ if(EhsHLogger_stackOk()){EhsSnprintf(EhsHLogger_Buffer(),EHSH_LOG_MAX_MSG,__VA_ARGS__);EhsHLogger_log(EHSL_MODULE_ID,nLevel,EHS_LOGGER_FILE_ARG,EHS_LOGGER_LINE_ARG,EhsHLogger_Buffer());} else {EhsHLogger_lowStackDrop();} }
 #endif
//else{EhsSprintf(EhsHLogger_Msg,__VA_ARGS__); EhsHLogger_log(EHSL_MODULE_ID,nLevel,__FILE__,__LINE__,EhsHLogger_Msg);}
#else
#define EHSH_LOG_MESSAGE(nLevel,...); {}
#endif
/**
 * Macros intended for users of the logger (require definition of EHSL_LOG_LEVEL_xxx
 */

#ifdef EHS_RUNTIME_LOGGER_ENABLED
#define EHSH_LOG_ERROR(...) 	EHSH_LOG_MESSAGE(EHSH_LOG_LEVEL_ERROR,__VA_ARGS__)
#define EHSH_LOG_WARNING(...) 	EHSH_LOG_MESSAGE(EHSH_LOG_LEVEL_WARNING,__VA_ARGS__)
#define EHSH_LOG_INFO(...) 		EHSH_LOG_MESSAGE(EHSH_LOG_LEVEL_INFO,__VA_ARGS__)
#define EHSH_LOG_ENTER(...) 	EHSH_LOG_MESSAGE(EHSH_LOG_LEVEL_ENTER,__VA_ARGS__)
#define EHSH_LOG_EXIT(...) 		EHSH_LOG_MESSAGE(EHSH_LOG_LEVEL_EXIT,__VA_ARGS__)
#else
#define EHSH_LOG_ERROR(...)
#define EHSH_LOG_WARNING(...)
#define EHSH_LOG_INFO(...)
#define EHSH_LOG_ENTER(...)
#define EHSH_LOG_EXIT(...)

#endif
/*lint -emacro( (534), EhsError ) Ignore return code of printf here */
//#define EhsError(err) {EhsConsolePrintf(err); EHSH_LOG_ERROR(err);}
//#define EhsWarning(err) {EhsConsolePrintf(err); EHSH_LOG_WARNING(err);}

/* Defensive NULL guard for kernel/HAL runtime paths. When (ptr) is NULL, log
 * and crash-out cleanly: deny the current app (so the next boot reverts to
 * _prev or default) and reboot. Compiles to a no-op in trusted-image builds
 * (EHS_APP_TRUST_MODEL != 0). The forward declarations match
 * hal_devapps_api.h / hal.h / hal_console.h / hal_time.h — duplicating them
 * here keeps this macro usable from any kernel TU without forcing extra
 * includes that pull in heavier headers.
 *
 * Output discipline. We emit through TWO sinks because the two we have on
 * embedded targets fail in different ways:
 *   - EHSH_LOG_ERROR routes through the runtime logger, which respects
 *     per-module level filters (EhsHLoggerModuleLogLevel[]). On some kernel
 *     TUs that array isn't populated yet at FB-init time and the message
 *     gets dropped.
 *   - EhsConsolePrintf goes straight to the platform console (UART / stdio /
 *     TCPIP buffer) without level filtering. On the rak3112 LoRaWAN worker
 *     trace (TCPIP console) it's the only sink that reliably appears. This one
 *     is kernel-only (see EHS_KERNEL_ONLY_CONSOLE below); components and HAL get
 *     the logger sink alone.
 * Both are cheap relative to the reboot we're about to do, so the kernel emits
 * through both. Then a short EhsSleep gives any background flush task
 * (TCPIP console writer, UART DMA drain) time to push the line out before
 * esp_restart() / sys_reboot() takes the CPU down. 100 ms is empirically
 * enough for the slowest sink (TCPIP) on rak3112 without being noticeable
 * to the user — the device reboots immediately after anyway. */
#if (EHS_APP_TRUST_MODEL == 0)
ehs_bool EhsAppDenyCurrentApp();
ehs_bool EhsHSysReboot();
EHS_GLOBAL void EhsSleep(EhsTickType tSleepTime);
/* EhsConsolePrintf comes via hal_console.h above. */
#ifndef EHS_TRUSTLESS_FLUSH_MS
#define EHS_TRUSTLESS_FLUSH_MS 100
#endif

/* The console sink is kernel-only. EhsConsolePrintf is declared solely under
 * EHS_KERNEL_BUILD / EHS_CONSOLE_IMPLEMENTATION (hal_console.h), so a component
 * or HAL file using these macros would otherwise fail to compile on an implicit
 * declaration - which it did, in inx-lorawan.c. Outside the kernel the record
 * still goes to the logger below; only the extra Lucid-channel copy is dropped,
 * which is the layering CLAUDE.md asks for anyway (diagnostics -> EHSH_LOG_*,
 * console reserved for the tools protocol). */
#if defined(EHS_KERNEL_BUILD) || defined(EHS_CONSOLE_IMPLEMENTATION)
#define EHS_KERNEL_ONLY_CONSOLE(...) EhsConsolePrintf(__VA_ARGS__)
#else
#define EHS_KERNEL_ONLY_CONSOLE(...) ((void)0)
#endif

#define EHS_TRUSTLESS_NULL_FATAL(ptr, msg)                                      \
    do {                                                                         \
        if ((ptr) == NULL) {                                                     \
            EHS_KERNEL_ONLY_CONSOLE("**FATAL: NULL " #ptr " at %s:%d (%s) — "     \
                "denying app and rebooting\n", __FILE__, __LINE__, (msg));       \
            EHSH_LOG_ERROR("FATAL: NULL " #ptr " at %s:%d (%s) — denying app and rebooting", \
                           __FILE__, __LINE__, (msg));                           \
            EhsSleep(EHS_TIME_ms(EHS_TRUSTLESS_FLUSH_MS));                       \
            (void)EhsAppDenyCurrentApp();                                        \
            (void)EhsHSysReboot();                                               \
            for (;;) { /* unreachable on a clean reboot */ }                     \
        }                                                                        \
    } while (0)

/* Stricter variant: reject **non-NULL** sub-page small integers that look
 * like a pointer but cannot be one (e.g. a struct field overwritten with a
 * count or length). NULL is *not* fatal here — most FB-side macros
 * (EHS_FB_OUT_CONNECTED_API2, EHS_FB_FINISH_CONNECTED_API2,
 * EHS_FB_IN_CONNECTED_API2) already short-circuit on NULL, and a callback
 * function with no <OutputPort> / <FinishPort> declared in the SODL
 * legitimately gets NULL pOut / pFinishPort from the parser. Use
 * EHS_TRUSTLESS_NULL_FATAL above when NULL truly indicates corruption.
 *
 * The canonical case for this macro is the LoRaWAN worker-thread cb
 * finding pFIdata->pOut == 0x18 — a value the bare NULL test passes but
 * the FB-side connected-test then segfaults on. The threshold is
 * overridable per target, but every reasonable embedded MMU/MPU layout
 * keeps the first page reserved, so 0x100 catches "obvious garbage"
 * without false positives on real heap/static data. */
#ifndef EHS_TRUSTLESS_PTR_MIN
#define EHS_TRUSTLESS_PTR_MIN 0x100u
#endif
#define EHS_TRUSTLESS_PTR_SANE_FATAL(ptr, msg)                                  \
    do {                                                                         \
        if ((ptr) != NULL &&                                                     \
            (unsigned long)(ptr) < (unsigned long)EHS_TRUSTLESS_PTR_MIN) {        \
            EHS_KERNEL_ONLY_CONSOLE("**FATAL: bogus " #ptr "=%p at %s:%d (%s) — " \
                "denying app and rebooting\n",                                    \
                (const void*)(ptr), __FILE__, __LINE__, (msg));                  \
            EHSH_LOG_ERROR("FATAL: bogus " #ptr "=%p at %s:%d (%s) — denying app and rebooting", \
                           (const void*)(ptr), __FILE__, __LINE__, (msg));       \
            EhsSleep(EHS_TIME_ms(EHS_TRUSTLESS_FLUSH_MS));                       \
            (void)EhsAppDenyCurrentApp();                                        \
            (void)EhsHSysReboot();                                               \
            for (;;) { /* unreachable on a clean reboot */ }                     \
        }                                                                        \
    } while (0)
#else
#define EHS_TRUSTLESS_NULL_FATAL(ptr, msg) ((void)0)
#define EHS_TRUSTLESS_PTR_SANE_FATAL(ptr, msg) ((void)0)
#endif

//@todo for legacy support
//#define EhsError(...) EHSH_LOG_ERROR(__VA_ARGS__) //@todo get rid of this version - use the EHSH version
//#define EhsWarning(...) EHSH_LOG_WARNING(__VA_ARGS__) //@todo get rid of this version - use the EHSH version

/* Compile time switched tracing function */
#ifdef EHS_BUILDOPT_STDIO_ENABLE_FUNCTION_TRACING

/**
 * Used in running debugging EHS via tracing calls. This represents a bitmask
 * of the subsystems that are currently being traced. 0 means trace is disabled.
 */
/* TOdo all these trasing functions - at elast the ernel ones, should go in the kernel code-base*/
ehs_uint32 EhsTraceFlags;

#define EHS_TRACE_FLAG_ATOM 0x8000 /**< Applies to functions that are called at the lowest level: bytes (file handling), tokens (parser), events (kernel) ... */
#define EHS_TRACE_FLAG_ITEM 0x4000 /**< Applies to items at the next level: lines (file handling), statements (parser),...*/
#define EHS_TRACE_FLAG_GROUP 0x2000 /**< Applies to "group" level items: function blocks (parser) ... */
#define EHS_TRACE_FLAG_TOPLEVEL 0x1000

#define EHS_TRACE_FLAG_HAL_FILE 0x0001
#define EHS_TRACE_FLAG_KERNEL 0x0010
#define EHS_TRACE_FLAG_PARSER 0x0020
#define EHS_TRACE_FLAG_GLOBAL 0x0800 /* unspecified functions */

#define EHS_TRACE_FLAG_SET(x) (((x) & EhsTraceFlags) == (x))

/* @todo the following should use __VAR_ARGS__ - */
/**
 * Conditional trace a function with no args @todo these need simplifying to a single __VA_ARG__ type */
/*lint -save -e960 -e961 #/## used only in debug build */
#define EHS_TRACE_FUNC0(subsys,name) {if (EHS_TRACE_FLAG_SET(subsys)) EhsConsolePrintf("%s()\n\r",#name); }
//todo Use the var args istead of all these???
/** Conditional trace a function with one argument */
#define EHS_TRACE_FUNC1(subsys,name,fmt,A) {if (EHS_TRACE_FLAG_SET(subsys)) EhsConsolePrintf("%s("fmt")\n\r",#name,A); }
/** Conditional trace a function with two arguments */
#define EHS_TRACE_FUNC2(subsys,name,fmt,A,B) {if (EHS_TRACE_FLAG_SET(subsys)) EhsConsolePrintf("%s("fmt")\n\r",#name,A,B); }
/** Conditional trace a function with three arguments */
#define EHS_TRACE_FUNC3(subsys,name,fmt,A,B,C) {if (EHS_TRACE_FLAG_SET(subsys)) EhsConsolePrintf("%s("fmt")\n\r",#name,A,B,C); }
/** Conditional trace a function with four arguments */
#define EHS_TRACE_FUNC4(subsys,name,fmt,A,B,C,D) {if (EHS_TRACE_FLAG_SET(subsys)) EhsConsolePrintf("%s("fmt")\n\r",#name,A,B,C,D); }

/** Conditional trace a function with one number argument */
#define EHS_TRACE_FUNCn(subsys,name,A) {if (EHS_TRACE_FLAG_SET(subsys)) EhsConsolePrintf("%s(%d)\n\r",#name,A); }
/** Conditional trace a function with one pointer argument */
#define EHS_TRACE_FUNCp(subsys,name,A) {if (EHS_TRACE_FLAG_SET(subsys)) EhsConsolePrintf("%s(%x)\n\r",#name,A); }
/** Conditional trace a function with one string argument */
#define EHS_TRACE_FUNCs(subsys,name,A) {if (EHS_TRACE_FLAG_SET(subsys)) EhsConsolePrintf("%s(%s)\n\r",#name,A); }
/** Conditional trace a function with arguments: pointer, string */
#define EHS_TRACE_FUNCpp(subsys,name,A,B) {if (EHS_TRACE_FLAG_SET(subsys)) EhsConsolePrintf("%s(%x,%x)\n\r",#name,A,B); }
/** Conditional trace a function with arguments: pointer, string */
#define EHS_TRACE_FUNCps(subsys,name,A,B) {if (EHS_TRACE_FLAG_SET(subsys)) EhsConsolePrintf("%s(%x,%s)\n\r",#name,A,B); }
/** Conditional trace a function with arguments: string, string */
#define EHS_TRACE_FUNCss(subsys,name,A,B) {if (EHS_TRACE_FLAG_SET(subsys)) EhsConsolePrintf("%s(%s,%s)\n\r",#name,A,B); }
/** Conditional trace a function with arguments: pointer, number, pointer */
#define EHS_TRACE_FUNCpnp(subsys,name,A,B,C) {if (EHS_TRACE_FLAG_SET(subsys)) EhsConsolePrintf("%s(%x,%d,%x)\n\r",#name,A,B,C); }
/** Conditional trace a function with arguments: pointer, string, pointer */
#define EHS_TRACE_FUNCpsp(subsys,name,A,B,C) {if (EHS_TRACE_FLAG_SET(subsys)) EhsConsolePrintf("%s(%x,%s,%x)\n\r",#name,A,B,C); }
/** Conditional trace a function with arguments: pointer, number, pointer */
#define EHS_TRACE_FUNCsnp(subsys,name,A,B,C) {if (EHS_TRACE_FLAG_SET(subsys)) EhsConsolePrintf("%s(%s,%d,%x)\n\r",#name,A,B,C); }
/** Conditional trace a function with arguments: pointer, number, number, pointer */
#define EHS_TRACE_FUNCpnnp(subsys,name,A,B,C,D) {if (EHS_TRACE_FLAG_SET(subsys)) EhsConsolePrintf("%s(%x,%d,%d,%x)\n\r",#name,A,B,C,D); }
/*lint restore */
#else
#define EHS_TRACE_FUNC0(subsys,name)
#define EHS_TRACE_FUNC1(subsys,name,fmt,A)
#define EHS_TRACE_FUNC2(subsys,name,fmt,A,B)
#define EHS_TRACE_FUNC3(subsys,name,fmt,A,B,C)
#define EHS_TRACE_FUNC4(subsys,name,fmt,A,B,C,D)
#define EHS_TRACE_FUNCn(subsys,name,A)
#define EHS_TRACE_FUNCp(subsys,name,A)
#define EHS_TRACE_FUNCs(subsys,name,A)
#define EHS_TRACE_FUNCpp(subsys,name,A,B)
#define EHS_TRACE_FUNCps(subsys,name,A,B)
#define EHS_TRACE_FUNCss(subsys,name,A,B)
#define EHS_TRACE_FUNCpnp(subsys,name,A,B,C)
#define EHS_TRACE_FUNCpsp(subsys,name,A,B,C)
#define EHS_TRACE_FUNCsnp(subsys,name,A,B,C)
#define EHS_TRACE_FUNCpnnp(subsys,name,A,B,C,D)

#endif /* EHS_BUILDOPT_STDIO_ENABLE_FUNCTION_TRACING */


/*****************************************************************************/
/* Declare global variables */

/**
 * Indicates the current log level for each module
 */
EHS_EXTERN EhsHLoggerLogLevel EhsHLoggerModuleLogLevel[];

#ifdef EHS_RUNTIME_LOGGER_ENABLED
/**
 * Temporary string used by EHSH_LOG_ macros to write message
 */
EHS_EXTERN ehs_char EhsHLogger_Msg[];
#endif
/*****************************************************************************/
/* Declare function prototypes  */

/**
 * Initialise the logger subsystem
 */
void EhsHLogger_init();

/**
 * Record a log entry
 */
void EhsHLogger_log(EhsHLoggerModuleId nModule, EhsHLoggerLogLevel nLevel,
                               const ehs_char* szFilename, ehs_uint32 nLine, const ehs_char* szMsg);

/**
 * Indicate what we want to log
 * @param[in] szModule Name of the module to log
 * @param[in] nLevels Set of the levels we wish to log (or'd together)
 */
ehs_bool EhsHLogger_setLogLevel(const ehs_char* szModule, EhsHLoggerLogLevel nLevels);

void EhsHSetLogLevels();

#endif /* EHS_HAL_LOGGER_H */
