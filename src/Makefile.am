bin_PROGRAMS = bina

bina_SOURCES = \
    algebra.c   \
    camera.c    \
    game.c      \
    memory.c    \
    renderer.c  \
    scene.c     \
    shader.c    \
    sprite.c    \
    texture.c   \
    utils.c

bina_LDADD = \
    $(GL_LIBS)   \
    $(GLU_LIBS)  \
    $(GLUT_LIBS)

if ENABLE_GLUT
    bina_SOURCES += main/glut.c
endif
if ENABLE_PNG
    bina_SOURCES += tc_png.c
endif
if ENABLE_ETC
    bina_LDADD += ../vendor/libktx/libktx.a
    bina_CFLAGS += -I$(top_srcdir)/vendor/libktx/include
endif
if ENABLE_S3TC
    bina_SOURCES += tc_s3tc.c
endif
