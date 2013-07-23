/**
 * Contains useful functions that can be used in any module.
 */

/**
 * @file utils.h
 * @author Jeremy Attali, Johan Attali
 * @date July 23, 2013
 */

#pragma once

/**
 * Get the path of a given filepath.
 */
void get_file_path(char* filepath, char* path);

/**
 * Get the file name of a given filepath.
 */
void get_file_name(char* filepath, char* filename);

/**
 * Get the extension of a given filepath.
 */
void get_file_extension(const char* filepath, char* ext);
