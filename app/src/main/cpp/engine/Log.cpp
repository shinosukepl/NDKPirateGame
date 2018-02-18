#include "Log.hpp"
#include <stdarg.h>
#include <android/log.h>

void Log::info(const char* pMessage, ...)
{
    va_list varArgs;
    va_start(varArgs, pMessage);
    __android_log_vprint(ANDROID_LOG_INFO, "PIRATE", pMessage,
        varArgs);
    __android_log_print(ANDROID_LOG_INFO, "PIRATE", "\n");
    va_end(varArgs);
}

void Log::error(const char* pMessage, ...)
{
    va_list varArgs;
    va_start(varArgs, pMessage);
    __android_log_vprint(ANDROID_LOG_ERROR, "PIRATE", pMessage,
        varArgs);
    __android_log_print(ANDROID_LOG_ERROR, "PIRATE", "\n");
    va_end(varArgs);
}

void Log::warn(const char* pMessage, ...)
{
    va_list varArgs;
    va_start(varArgs, pMessage);
    __android_log_vprint(ANDROID_LOG_WARN, "PIRATE", pMessage,
        varArgs);
    __android_log_print(ANDROID_LOG_WARN, "PIRATE", "\n");
    va_end(varArgs);
}

void Log::debug(const char* pMessage, ...)
{
    va_list varArgs;
    va_start(varArgs, pMessage);
    __android_log_vprint(ANDROID_LOG_DEBUG, "PIRATE", pMessage,
        varArgs);
    __android_log_print(ANDROID_LOG_DEBUG, "PIRATE", "\n");
    va_end(varArgs);
}
