noinst_LIBRARIES = libktx.a

libktx_a_SOURCES = \
    lib/checkheader.c \
    lib/errstr.c \
    lib/etcdec.cxx \
    lib/etcunpack.cxx \
    lib/loader.c \
    lib/swap.c

libktx_a_CPPFLAGS = \
    -I$(top_srcdir)/include \
    -I$(srcdir)/include \
    -I$(srcdir)/lib

libktx_a_CXXFLAGS = \
    -Wno-unused-result

libktx_a_CFLAGS = \
    -Wno-pointer-sign
