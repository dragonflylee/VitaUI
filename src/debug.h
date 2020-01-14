#ifndef _VITA_DEBUG_H_
#define _VITA_DEBUG_H_

#define LOG(fmt, ...) log_printf(L_INFO, __FILE__, __LINE__, fmt, ##__VA_ARGS__);

typedef enum
{
    L_DEBUG,
    L_INFO,
    L_WARN,
    L_ERR
} LOG_LEVEL;

int log_init();
int log_flush();
int log_printf(int level, const char *file, int line, const char *format, ...);

#endif // _VITA_DEBUG_H_