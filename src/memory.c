/**
 * @file memory.c
 * @author Jeremy Attali, Johan Attali
 * @date July 27, 2013
 */

#include "bina.h"
#include "memory.h"
#include "error.h"
#include "log.h"

memory_t*
memory_create(const char* filepath)
{
    memory_t* memory = NULL;
    FILE* fd = NULL;

    log_d("loading: %s into memory", filepath);

    memory = (memory_t*) calloc(1, sizeof(memory_t));
    if (!memory) {
        log_e(BINA_NOT_ENOUGH_MEMORY);
        goto error;
    }

#ifdef ANDROID
    AAsset* asset = NULL;
    /* char buf[BUFSIZ]; */
    /* int nb_read = 0; */
    off_t asset_length = 0;
    const void* buffer = NULL;

    /* Copying filepath */
    strncpy(memory->filepath, filepath, MAX_PATH);

    if (!asset_manager_g) {
        log_e("cannot load: %s - AssetManager is not loaded", filepath);
        goto android_error;
    }

    asset = AAssetManager_open(asset_manager_g, filepath,
                               AASSET_MODE_BUFFER);
    if (!asset) {
        log_e("cannot load: %s - could not found asset", filepath);
        goto android_error;
    }

    /* Getting the pointer of the asset */
    buffer = AAsset_getBuffer(asset);

    /* Getting buffer size */
    memory->size = AAsset_getLength(asset);

    /* Allocating enough memory for buffer */
    memory->buffer = (unsigned char *) calloc(1, memory->size + 1);
    if (!memory->buffer) {
        log_e(BINA_NOT_ENOUGH_MEMORY);
        goto android_error;
    }

    /* Copying buffer into memory object buffer */
    memcpy(memory->buffer, buffer, memory->size);
    memory->buffer[memory->size] = 0;

    AAsset_close(asset);

    return memory;

android_error:
    if (asset) {
        AAsset_close(asset);
    }

    goto error;
#else
    /* Copying filepath */
    snprintf(memory->filepath, MAX_PATH, "assets/%s", filepath);

    if ((fd = fopen(memory->filepath, "rb")) == NULL) {
        log_e("unable to open: %s for reading", filepath);
        goto else_error;
    }

    /* Getting buffer size */
    fseek(fd, 0, SEEK_END);
    memory->size = ftell(fd);
    fseek(fd, 0, SEEK_SET);

    /* Allocating enough memory for buffer */
    memory->buffer = (unsigned char *) calloc(1, memory->size + 1);
    if (!memory->buffer) {
        log_e(BINA_NOT_ENOUGH_MEMORY);
        goto else_error;
    }

    /* Reading buffer */
    fread(memory->buffer, memory->size, 1, fd);
    memory->buffer[memory->size] = 0;

    fclose(fd);
    return memory;

else_error:
    if (fd) {
        fclose(fd);
    }

    goto error;
#endif

error:
    memory_delete(&memory);

    return NULL;
}

void
memory_delete(memory_t** memory)
{
    memory_t* tmp = *memory;

    if (tmp) {
        if (tmp->buffer) {
            free(tmp->buffer);
        }
        free(tmp);
        tmp = NULL;
    }

    *memory = tmp;
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
