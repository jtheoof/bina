/**
 * @file memory.c
 * @author Jeremy Attali, Johan Attali
 * @date July 27, 2013
 */

#include "bina.h"

memory_t*
memory_create(const char* filepath)
{
#ifdef ANDROID
    LOGE(BINA_NOT_IMPLEMENTED);
    return NULL;
#else
    FILE* fd;
    memory_t* memory = NULL;

    if ((fd = fopen(filepath, "rb")) == NULL) {
        LOGE("Unable to open: %s for reading", filepath);
        goto error;
    }

    memory = (memory_t*) calloc(1, sizeof(memory_t));
    if (!memory) {
        LOGE(BINA_NOT_ENOUGH_MEMORY);
        goto error;
    }

    /* Getting buffer size */
    fseek(fd, 0, SEEK_END);
    memory->size = ftell(fd);
    fseek(fd, 0, SEEK_SET);

    /* Allocating enough memory for buffer */
    memory->buffer = (unsigned char *) calloc(1, memory->size + 1);
    if (!memory->buffer) {
        LOGE(BINA_NOT_ENOUGH_MEMORY);
        goto error;
    }

    /* Copying filepath */
    strncpy(memory->filepath, filepath, MAX_PATH);

    /* Reading buffer */
    fread(memory->buffer, memory->size, 1, fd);
    memory->buffer[memory->size] = 0;

    fclose(fd);
    return memory;
#endif

error:
    memory_delete(&memory);
    if (fd) {
        fclose(fd);
    }

    return NULL;
}

void
memory_delete(memory_t** memory)
{
    memory_t* tmp = *memory;
    free(tmp->buffer);
    free(tmp);
    *memory = NULL;
}


unsigned int
memory_read(memory_t* memory, void* dest, unsigned int size)
{
	if ((memory->position + size) > memory->size) {
        size = memory->size - memory->position;
    }

	memcpy(dest, &memory->buffer[memory->position], size);

	memory->position += size;

	return size;
}
