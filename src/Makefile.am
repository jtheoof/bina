bin_PROGRAMS = bina

bina_SOURCES = \
    bina.h      \
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

bina_CFLAGS = \
    -I$(top_srcdir)/include

bina_LDADD = \
    $(GL_LIBS)   \
    $(GLU_LIBS)  \
    $(GLUT_LIBS)

if ENABLE_GLUT
    bina_SOURCES += main/glut.c
endif
if ENABLE_PNG
    bina_SOURCES += tc_png.c tc_png.h
endif
if ENABLE_ETC
    bina_LDADD += ../vendor/libktx/libktx.a -lstdc++
    bina_CFLAGS += -I$(top_srcdir)/vendor/libktx/include
endif
if ENABLE_ETC
    bina_SOURCES += tc_etc.c tc_etc.h
endif
if ENABLE_S3TC
    bina_SOURCES += tc_s3tc.c tc_s3tc.h
endif
