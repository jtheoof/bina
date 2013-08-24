bin_PROGRAMS = bina

bina_SOURCES = \
    main/glut.c \
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

if ENABLE_S3TC
    bina_SOURCES += tc_s3tc.c
endif

bina_LDADD = \
    $(GL_LIBS)   \
    $(GLU_LIBS)  \
    $(GLUT_LIBS)
