lib_LTLIBRARIES = libbina.la

libbina_la_SOURCES = \
    bina.h      \
    bina.c      \
    algebra.c   \
    algebra.h   \
    camera.c    \
    camera.h    \
    game.c      \
    game.h      \
    memory.c    \
    memory.h    \
    renderer.c  \
    renderer.h  \
    scene.c     \
    scene.h     \
    shader.c    \
    shader.h    \
    sprite.c    \
    sprite.h    \
    texture.c   \
    texture.h   \
    utils.c     \
    utils.h

libbina_la_CFLAGS = \
    $(SDL_CFLAGS)           \
    -I$(top_srcdir)/include

libbina_la_LDFLAGS = \
    -lm

libbina_la_LIBADD = \
    $(SDL_LIBS)  \
    $(GL_LIBS)   \
    $(GLU_LIBS)

if ENABLE_PNG
    libbina_la_SOURCES += tc_png.c tc_png.h
endif
if ENABLE_ETC
    libbina_la_LIBADD += ../vendor/libktx/libktx.a -lstdc++
    libbina_la_CFLAGS += -I$(top_srcdir)/vendor/libktx/include
endif
if ENABLE_ETC
    libbina_la_SOURCES += tc_etc.c tc_etc.h
endif
if ENABLE_S3TC
    libbina_la_SOURCES += tc_s3tc.c tc_s3tc.h
endif
