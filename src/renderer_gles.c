#include "bina.h"

static const char gVertexShader[] = 
    "#ifdef GL_ES\n"
    "precision mediump float;\n"
    "#endif\n"
    "attribute vec4 vPosition;\n"
    "attribute vec4 vColor;\n"
    "varying   vec4 fColor;\n"
    "void main() {\n"
    "  gl_Position = vPosition;\n"
    "  fColor = vColor;\n"
    "}\n";

/* static const char gFragmentShader[] = */ 
/*     "precision mediump float;\n" */
/*     "void main() {\n" */
/*     "  gl_FragColor = vec4(0.0, 1.0, 0.0, 1.0);\n" */
/*     "}\n"; */
static const char gFragmentShader[] = 
    "#ifdef GL_ES\n"
    "precision mediump float;\n"
    "#endif\n"
    "varying vec4 fColor;\n"
    "void main() {\n"
    "  gl_FragColor = fColor;\n"
    "}\n";

static const GLfloat gTriangleVertices[] = {
     1.0f,  1.0f,
    -1.0f, -1.0f,
     1.0f, -1.0f
};

static const GLfloat gTriangleColors[] = {
    1.0f, 0.0f, 0.0f, 1.0f,
    0.0f, 1.0f, 0.0f, 1.0f,
    0.0f, 0.0f, 1.0f, 1.0f
}; 

static const GLfloat gSquareVertices[] = {
    -1.0f,  1.0f,
    -1.0f, -1.0f,
     1.0f,  1.0f,
     1.0f, -1.0f,
};

static const GLfloat gSquareColors[] = {
    1.0f, 0.0f, 0.0f, 1.0f,
    0.0f, 1.0f, 0.0f, 1.0f,
    0.0f, 0.0f, 1.0f, 1.0f,
    1.0f, 0.0f, 1.0f, 1.0f
}; 

static const GLuint gSquareIndices[] = {
    0, 1, 2
};

GLuint gProgram;
GLuint gvPositionAttrib;
GLuint gvColorAttrib;

GLuint gSquareIndicesBuffer;
GLuint gSquareVerticesBuffer;

GLuint gvColorHandle;

/* TODO implement real error code */
int
renderer_init(int w, int h)
{
    /* print_gl_string("Version", GL_VERSION); */
    /* print_gl_string("Vendor", GL_VENDOR); */
    /* print_gl_string("Renderer", GL_RENDERER); */
    /* print_gl_string("Extensions", GL_EXTENSIONS); */

    LOGI("[renderer_init] width: %d height: %d\n", w, h);

    gProgram = shader_create_program(gVertexShader, gFragmentShader);
    if (!gProgram) {
        LOGE("Could not create program.");
        return 1;
    }

    /* attribute vPosition */
    gvPositionAttrib = glGetAttribLocation(gProgram, "vPosition");
    check_gl_error("glGetAttribLocation");
    LOGI("glGetAttribLocation(\"vPosition\") = %d\n", gvPositionAttrib);

    /* attribute vColor */
    gvColorAttrib = glGetAttribLocation(gProgram, "vColor");
    check_gl_error("glGetAttribLocation");
    LOGI("glGetAttribLocation(\"vColor\") = %d\n", gvColorAttrib);

    /* glGenBuffers(1, &gSquareVerticesBuffer); */
    /* glBindBuffer(GL_ARRAY_BUFFER, gSquareVerticesBuffer); */
    /* check_gl_error("glBindBuffer position"); */
    /* glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * 6, */
    /*              gSquareVertices, GL_STATIC_DRAW); */
    /* check_gl_error("glBufferData position"); */
    /* glBindBuffer(GL_ARRAY_BUFFER, 0); */

    glGenBuffers(1, &gSquareIndicesBuffer);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, gSquareIndicesBuffer);
    check_gl_error("glBindBuffer indices");
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLuint) * 3,
                 gSquareIndices, GL_STATIC_DRAW);
    check_gl_error("glBufferData indices");
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

    /* The following two lines enable semi transparent. */
    glEnable(GL_BLEND);
    check_gl_error("glEnable");
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    check_gl_error("glBlendFunc");
 
	glDisable(GL_DEPTH_TEST);
    check_gl_error("glDisable");
	glDepthMask(GL_FALSE);
    check_gl_error("glDepthMask");

    /* Set up viewport */
    glViewport(0, 0, w, h);
    check_gl_error("glViewport");

    return 0;
}

static void
render_triangle()
{
    glClearColor(0.0f, 0.4f, 1.0f, 1.0f);
    check_gl_error("glClearColor");

    glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
    check_gl_error("glClear");

    glUseProgram(gProgram);
    check_gl_error("glUseProgram");

    glEnableVertexAttribArray(gvPositionAttrib);
    check_gl_error("glEnableVertexAttribArray");
    glVertexAttribPointer(gvPositionAttrib, 2, GL_FLOAT, GL_FALSE, 0,
                          gTriangleVertices);
    check_gl_error("glVertexAttribPointer");

    glEnableVertexAttribArray(gvColorAttrib);
    check_gl_error("glEnableVertexAttribArray");
    glVertexAttribPointer(gvColorAttrib, 4, GL_FLOAT, GL_FALSE, 0,
                          gTriangleColors);
    check_gl_error("glVertexAttribPointer");

    glDrawArrays(GL_TRIANGLES, 0, 3);
    check_gl_error("glDrawArrays");
}

static void
render_square()
{
    /* glClearColor(1.0f, 0.4f, 0.0f, 1.0f); */
    glClearColor(0.0f, 0.4f, 1.0f, 1.0f);
    check_gl_error("glClearColor");

    glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
    check_gl_error("glClear");

    glUseProgram(gProgram);
    check_gl_error("glUseProgram");

    glEnableVertexAttribArray(gvPositionAttrib);
    check_gl_error("glEnableVertexAttribArray");
    glVertexAttribPointer(gvPositionAttrib, 2, GL_FLOAT, GL_FALSE, 0,
                          gSquareVertices);
    check_gl_error("glVertexAttribPointer");

    glEnableVertexAttribArray(gvColorAttrib);
    check_gl_error("glEnableVertexAttribArray");
    glVertexAttribPointer(gvColorAttrib, 4, GL_FLOAT, GL_FALSE, 0,
                          gSquareColors);
    check_gl_error("glVertexAttribPointer");

    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
    check_gl_error("glDrawArrays");
}

void
renderer_render()
{
    /* render_triangle(); */
    render_square();
}

