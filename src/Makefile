program_NAME := main
program_C_SRCS := $(wildcard *.c)
program_CXX_SRCS := $(wildcard *.cpp)
program_C_OBJS := ${program_C_SRCS:.c=.o}
program_CXX_OBJS := ${program_CXX_SRCS:.cpp=.o}
program_OBJS := $(program_C_OBJS) $(program_CXX_OBJS)
program_INCLUDE_DIRS :=
program_LIBRARY_DIRS :=
program_LIBRARIES := glut png GLEW GLU

CFLAGS := -std=c99 -DHAVE_STDLIB_H -DHAVE_PNG_H -DHAVE_GLUT_H -DHAVE_GLEW_H -DHAVE_GLU_H
CPPFLAGS += $(foreach includedir,$(program_INCLUDE_DIRS),-I$(includedir))
LDFLAGS += $(foreach librarydir,$(program_LIBRARY_DIRS),-L$(librarydir))
LDFLAGS += $(foreach library,$(program_LIBRARIES),-l$(library))

.PHONY: all clean distclean

all: simple

simple:
	gcc main_glut.c bina.c shader.c texture.c camera.c renderer_gles.c -g -o main ${CFLAGS} ${LDFLAGS}

clean:
	rf -f ${program_NAME}

distclean: clean
