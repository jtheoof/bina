/**
 * @file memory.c
 * @author Jeremy Attali, Johan Attali
 * @date July 27, 2013
 */

#include "bina.h"

memory_t*
memory_create(const char* filepath)
{
    FILE* fd;
    memory_t* memory = NULL;

#ifdef ANDROID
    /* AAssetDir* assetDir = AAssetManager_openDir(mgr, ""); */
    /* const char* filename = (const char*)NULL; */
    /* while ((filename = AAssetDir_getNextFileName(assetDir)) != NULL) { */
    /*     AAsset* asset = AAssetManager_open(mgr, filename, AASSET_MODE_STREAMING); */
    /*     char buf[BUFSIZ]; */
    /*     int nb_read = 0; */
    /*     FILE* out = fopen(filename, "w"); */
    /*     while ((nb_read = AAsset_read(asset, buf, BUFSIZ)) > 0) */
    /*         fwrite(buf, nb_read, 1, out); */
    /*     fclose(out); */
    /*     AAsset_close(asset); */
    /* } */
    /* AAssetDir_close(assetDir); */

    /* AAssetManager manager; */
    /* pngAassetManager_ = assetManager; */
    /* LOGI("Trying to load image..."); */
    /* int HEADER_SIZE = 8; */
    /* string filename = "skybox.png"; */
    /* pngAsset_ = AAssetManager_open(pngAassetManager_, filename.c_str(), */
    /*         AASSET_MODE_UNKNOWN); */
    /* if (pngAsset_ == 0) { */
    /*     LOGW("Asset \"%s\" not found.", filename.c_str()); */
    /*     return 0; */
    /* } */
    /* off_t bufferSize = AAsset_getLength(pngAsset_); */
    /* png_byte* buffer = new png_byte[HEADER_SIZE]; */
    /* int numBytesRead = AAsset_read(pngAsset_, buffer, HEADER_SIZE); */
    /* int numBytesRemaining = AAsset_getRemainingLength(pngAsset_); */
    /* AAsset_seek(pngAsset_, 0, 0); */
    /* AAsset_read(pngAsset_, data, size); */
    /* int numBytesRemaining = AAsset_getRemainingLength(pngAsset_); */
    /* LOGI("Read size: %d, remaining: %d", size, numBytesRemaining); */
    return NULL;
#else
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
