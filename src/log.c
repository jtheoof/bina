/**
 * @file log.c
 * @author Jeremy Attali
 */

#include "main.h"
#include "log.h"

#define COLOR_RESET   "\033[0m"
#define COLOR_BLACK   "\033[0;30m"
#define COLOR_RED     "\033[0;31m"
#define COLOR_GREEN   "\033[0;32m"
#define COLOR_YELLOW  "\033[0;33m"
#define COLOR_BLUE    "\033[0;34m"
#define COLOR_MAGENTA "\033[0;35m"
#define COLOR_CYAN    "\033[0;36m"
#define COLOR_WHITE   "\033[0;37m"

#define PRIORITY_VERBOSE  COLOR_WHITE
#define PRIORITY_DEBUG    COLOR_GREEN
#define PRIORITY_INFO     COLOR_CYAN
#define PRIORITY_WARN     COLOR_YELLOW
#define PRIORITY_ERROR    COLOR_RED
#define PRIORITY_CRITICAL "\033[0;45;37m"

typedef struct prefix_color_token
{
    char* prefix;
    char* color;
} prefix_color_token_t;

static const prefix_color_token_t prefix_colors[] = {
    { NULL,       COLOR_WHITE       },
    { "VERBOSE",  PRIORITY_VERBOSE  },
    { "DEBUG",    PRIORITY_DEBUG    },
    { "INFO",     PRIORITY_INFO     },
    { "WARN",     PRIORITY_WARN     },
    { "ERROR",    PRIORITY_ERROR    },
    { "CRITICAL", PRIORITY_CRITICAL }
};

/**
 * Tokenize a log message and split it into its file, line and reminder.
 *
 * @param message A typical log message would look like this:
 * "bina.c\x1d42\x1dRemaining message"
 * @param file The file that was extracted (in this example: bina.c).
 * @param line The line that was extracted (in this example: 42).
 * @param reminder What remains of the message.
 */
static void
tokenize_message(const char* message, char** file, int* line, char** reminder)
{
    int len = strlen(message);
    char* del = "\x1d";
    char* sav;
    char* tmp = strdup(message);
    char* tok = strtok_r(tmp, del, &sav);

    *line = 0;
    *file = NULL;
    *reminder = NULL;

    if (!tok || strlen(tok) == len) {
        strdup(message);
    }

    *file = strdup(basename(tok));
    tok = strtok_r(NULL, del, &sav);

    if (!tok) {
        goto cleanup;
    }

    *line = atoi(tok);

    tok = strtok_r(NULL, del, &sav);

    if (!tok) {
        goto cleanup;
    }

    *reminder = strdup(tok);
    free(tmp);

    return;

cleanup:
    if (*file) {
        free(*file);
        *file = NULL;
    }
    *line = 0;
    *reminder = NULL;

    free(tmp);
}

void
log_custom(void *userdata, int category, sdl_log_priority_e priority,
           const char *message)
{
    int line;
    char* file;
    char* message_shown;
    char file_line[MAX_CHAR];
    char file_line_pad[MAX_CHAR];

    prefix_color_token_t token = prefix_colors[priority];

    /* Looking for __FILE__ and __LINE__ */
    tokenize_message(message, &file, &line, &message_shown);

    if (file) {
        snprintf(file_line, MAX_CHAR, "%s:%d", file, line);
        snprintf(file_line_pad, MAX_CHAR, "%15s", file_line);
    } else {
        file_line_pad[0] = 0;
        message_shown = strdup(message);
    }

    fprintf(stderr, "%s%-8s | %15s | %s%s\n", token.color, token.prefix,
            file_line_pad, message_shown, COLOR_RESET);

    if (file) {
        free(file);
    }

    if (message_shown) {
        free(message_shown);
    }
}
