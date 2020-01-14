#include "debug.h"

#include <psp2/appmgr.h>
#include <psp2/kernel/processmgr.h>
#include <psp2/kernel/threadmgr.h>
#include <psp2/io/fcntl.h>
#include <psp2/io/stat.h>
#include <psp2/rtc.h>
#include <stdio.h>
#include <stdarg.h>
#include <string.h>

#define MAX_PATH 255
#define MAX_BUF 1024

int _log_fd = 0;

int log_init(const char *dir)
{
    SceDateTime log_time;
    sceRtcGetCurrentClockLocalTime(&log_time);
    sceIoMkdir(dir, 0777);

    char log_file[MAX_PATH];
    SceUID id = sceKernelGetProcessId();
    snprintf(log_file, sizeof(log_file), "%s/vitaui-%04d-%02d-%02d(%d).log",
        dir, log_time.year, log_time.month, log_time.day, id);
    _log_fd = sceIoOpen(log_file, SCE_O_WRONLY | SCE_O_CREAT | SCE_O_TRUNC, 0777);

    memset(log_file, 0, sizeof(log_file));
    sceAppMgrAppParamGetString(id, 12, log_file, sizeof(log_file));
    return log_printf(L_INFO, NULL, 0, "VitaUI start [%s]", log_file);
}

int log_flush()
{
    sceIoSyncByFd(_log_fd);
    return sceIoClose(_log_fd);
}

const char *log_level(int level)
{
    switch (level)
    {
    case L_DEBUG:
        return "DEBUG";
    case L_INFO:
        return "INFO";
    case L_WARN:
        return "WARN";
    case L_ERR:
        return "ERROR";
    }
    return "FATAL";
}

int log_printf(int level, const char *file, int line, const char *format, ...)
{
    int len;
    char buf[MAX_BUF + 1];
    SceDateTime log_time;
    sceRtcGetCurrentClockLocalTime(&log_time);
    len = snprintf(buf, sizeof(buf), "%02d:%02d:%02d [%s] (0x%x)\t",
                   log_time.hour, log_time.minute, log_time.second,
                   log_level(level), sceKernelGetThreadId());

    if (file != NULL && (file = strrchr(file, '/')))
        len += snprintf(buf + len, sizeof(buf) - len, "%s:%d\t", file + 1, line);

    va_list args;
    va_start(args, format);
    len += vsnprintf(buf + len, sizeof(buf) - len, format, args);
    va_end(args);
    buf[len] = '\n';

    return sceIoWrite(_log_fd, buf, len + 1);
}
