/**
 * @file utils.c
 * @author Jeremy Attali, Johan Attali
 * @date July 23, 2013
 */

#include "bina.h"

/**
 * Replace the \ for / in a an absolute or relative file path.
 *
 * @param filepath The file path to adjust.
 */
static void
adjust_file_path(char *filepath)
{
    unsigned int i = 0,
                 l = strlen(filepath);

    while (i != l)
    {
        if (filepath[i] == '\\') {
            filepath[i] = '/';
        }
        ++i;
    }
}


/**
 * Extract path from a file path.
 *
 * @param filepath The file path to use to extract the path.
 * @param path The return path.
 */
void
get_file_path(char *filepath, char *path)
{
    char *t = NULL;

    unsigned int p;

    adjust_file_path(filepath);

    t = strrchr(filepath, '/');

    if (t)
    {
        p = (t - filepath) + 1;
        strncpy(path, filepath, p);
        path[p] = 0;
    }
}


/**
 * Extract the file name from a file path.
 *
 * param filepath The file path to use to extract the file name.
 * param filename The file name.
 */
void
get_file_name(char *filepath, char *filename)
{
    char *t = NULL;

    adjust_file_path(filepath);

    t = strrchr(filepath, '/');

    if (t) {
        strcpy( filename, t + 1);
    }
    else {
        strcpy(filename, filepath);
    }
}

/**
 * Extract the extension of a file. And optionally convert it to uppercase.
 *
 * @param filepath The file path to use.
 * @param ext If a valid extension is found this variable will be used as
 * destination to store it.
 */
void
get_file_extension(const char* filepath, char* ext)
{
    char* t;
    char* f;
    int   l = strlen(filepath);

    f = (char *) malloc(sizeof(char) * (l + 1));
    strcpy(f, filepath);

    adjust_file_path(f);
    t = strrchr(f, '.');

    if (t) {
        strcpy(ext, t + 1);
    }

    if (f) {
        free(f);
    }
}
