lib_LTLIBRARIES = libbina.la
bin_PROGRAMS = bina

LT_AGE      = @LT_AGE@
LT_CURRENT  = @LT_CURRENT@
LT_RELEASE  = @LT_RELEASE@
LT_REVISION = @LT_REVISION@
LT_LDFLAGS  = \
    -no-undefined                                        \
    -version-info $(LT_CURRENT):$(LT_REVISION):$(LT_AGE)

libbinaincludedir = $(includedir)/bina

libbinainclude_HEADERS = \
    algebra.h  \
    bina.h     \
    error.h    \
    main.h     \
    renderer.h \
    log.h

libbina_la_SOURCES = \
    memory.c    \
    algebra.c   \
    log.c       \
    main.c      \
    texture.c   \
    utils.c     \
    renderer.c

libbina_la_CFLAGS = \
    $(SDL_CFLAGS)           \
    -I$(top_srcdir)/include

libbina_la_LDFLAGS = \
    $(LT_LDFLAGS) \
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
    libbina_la_SOURCES += tc_etc.c tc_etc.h
endif

if ENABLE_S3TC
    libbina_la_SOURCES += tc_s3tc.c tc_s3tc.h
endif

bina_SOURCES = \
    bin/x86_64.c

bina_CFLAGS = \
    $(SDL_CFLAGS)           \
    -I$(top_srcdir)/include

bina_LDADD = \
    $(SDL_LIBS)  \
    $(GL_LIBS)   \
    $(GLU_LIBS)  \
    libbina.la
