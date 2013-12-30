/**
 * Logger functions.
 */

/**
 * @file log.h
 * @author Jeremy Attali
 */

#pragma once

#include <wrapper/sdl.h>

void log_custom(void *userdata, int category, sdl_log_priority_e priority,
                const char *message);

#define LOG_FORMAT_FILE_LINE "%s\x1d%d\x1d"

#define log_v(fmt, ...) \
    SDL_LogVerbose(SDL_LOG_CATEGORY_APPLICATION, \
                   LOG_FORMAT_FILE_LINE fmt, __FILE__, __LINE__, ##__VA_ARGS__)
#define log_d(fmt, ...) \
    SDL_LogDebug(SDL_LOG_CATEGORY_APPLICATION, \
                 LOG_FORMAT_FILE_LINE fmt, __FILE__, __LINE__, ##__VA_ARGS__)
#define log_i(fmt, ...) \
    SDL_LogInfo(SDL_LOG_CATEGORY_APPLICATION, \
                LOG_FORMAT_FILE_LINE fmt, __FILE__, __LINE__, ##__VA_ARGS__)
#define log_w(fmt, ...) \
    SDL_LogWarn(SDL_LOG_CATEGORY_APPLICATION, \
                LOG_FORMAT_FILE_LINE fmt, __FILE__, __LINE__, ##__VA_ARGS__)
#define log_e(fmt, ...) \
    SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, \
                 LOG_FORMAT_FILE_LINE fmt, __FILE__, __LINE__, ##__VA_ARGS__)
#define log_c(fmt, ...) \
    SDL_LogCritical(SDL_LOG_CATEGORY_APPLICATION, \
                    LOG_FORMAT_FILE_LINE fmt, __FILE__, __LINE__, ##__VA_ARGS__)
