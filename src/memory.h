/**
 * Contains all the necessary structs and functions to deal with memory.
 *
 * This is particularly useful to load assets from different devices into
 * memory and not having to worry about them. We just load them once from
 * their respective location. The implementation will vary depending on
 * devices.
 */

/**
 * @file memory.h
 * @author Jeremy Attali, Johan Attali
 * @date July 27, 2013
 */

#pragma once

typedef struct memory_t
{
    /**
     * The filepath used by the structure.
     */
    char filepath[MAX_PATH];

    /**
     * The file size in bytes.
     */
    unsigned int size;

    /**
     * The position of the cursor within the memory buffer.
     */
    unsigned int position;

    /**
     * The memory buffer.
     */
    unsigned char* buffer;
} memory_t;

/**
 * Creates a new memory object.
 *
 * @param filepath The filepath where the object is stored. This should be
 * generic enough so that it works in any device.
 * @return The new memory object allocated.
 */
memory_t* memory_create(const char* filepath);

/**
 * Frees any memory allocated by #memory_create.
 *
 * @param memory The memory to free.
 */
void memory_delete(memory_t** memory);

/**
 * Read bytes from the memory buffer and put the in #dest.
 *
 * @param memory The memory object that contains the data to read.
 * @param dest The destination buffer.
 * @param size How many bytes are to be read.
 * @return The size read (could be less than asked for at end of the buffer).
 */
unsigned int memory_read(memory_t* memory, void* dest, unsigned int size);
