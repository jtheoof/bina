/**
 * @file renderer.c
 * @author Jeremy Attali, Johan Attali
 * @date July 23, 2013
 */

#include "bina.h"

static const char gVertexShader[] = 
    "#ifdef GL_ES\n"
    "precision mediump float;\n"
    "#endif\n"
    "attribute vec4 vPosition;\n"
    "attribute vec4 aColor;\n"
    "attribute vec2 aTexture;\n"
    "varying   vec4 vColor;\n"
    "varying   vec2 vTexture;\n"
    "void main() {\n"
    "  gl_Position = vPosition;\n"
    "  vColor = aColor;\n"
    "  vTexture = aTexture;\n"
    "}\n";

static const char gFragmentShader[] = 
    "#ifdef GL_ES\n"
    "precision mediump float;\n"
    "#endif\n"
    "uniform sampler2D uTexture;\n"
    "varying vec4      vColor;\n"
    "varying vec2      vTexture;\n"
    "void main() {\n"
    "  gl_FragColor = texture2D(uTexture, vTexture);\n"
    /* "  gl_FragColor = vColor;\n" */
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
     1.0f, -1.0f
};

static const GLfloat gSquareTexices[] = {
     0.0f,  1.0f,
     0.0f,  0.0f,
     1.0f,  1.0f,
     1.0f,  0.0f
};

static const GLuint gSquareIndices[] = {
    0, 1, 2
};

static const GLfloat gSquareColors[] = {
    1.0f, 0.0f, 0.0f, 1.0f,
    0.0f, 1.0f, 0.0f, 1.0f,
    0.0f, 0.0f, 1.0f, 1.0f,
    1.0f, 0.0f, 1.0f, 1.0f
}; 

GLuint gProgram;

GLint gvPositionAttrib;
GLint gvColorAttrib;
GLint gvTextureAttrib;

GLint  gTextureUniform;

GLuint gSquareIndicesBuffer;
GLuint gSquareVerticesBuffer;
GLuint gSquareTexicesBuffer;
GLuint gSquareTextureBuffer;

GLuint gvColorHandle;

static void
load_background()
{
    glGenTextures(1, &gSquareTextureBuffer);
	glBindTexture(GL_TEXTURE_2D, gSquareTextureBuffer);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR); 
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

    glTexImage2D(GL_TEXTURE_2D, 0,
                 GL_RGBA, 
                 background_old.width, background_old.height, 0,
                 background_old.alpha ? GL_RGBA : GL_RGB, GL_UNSIGNED_BYTE,
                 /* GL_BGR, GL_UNSIGNED_BYTE, */
                 background_old.pixels);

	glBindTexture(GL_TEXTURE_2D, 0);
}

/* TODO implement real error code */
void
renderer_init(int width, int height)
{
    /* print_gl_string("Version", GL_VERSION); */
    /* print_gl_string("Vendor", GL_VENDOR); */
    /* print_gl_string("Renderer", GL_RENDERER); */
    /* print_gl_string("Extensions", GL_EXTENSIONS); */

    /* LOGI("Scene: width: %d height: %d", width, height); */

    /* gProgram = shader_create_program(gVertexShader, gFragmentShader); */
    /* if (!gProgram) { */
    /*     LOGE("Could not create program."); */
    /*     return 1; */
    /* } */

    /* gvPositionAttrib = glGetAttribLocation(gProgram, "vPosition"); */
    /* gvColorAttrib = glGetAttribLocation(gProgram, "aColor"); */
    /* gvTextureAttrib = glGetAttribLocation(gProgram, "aTexture"); */

    /* gTextureUniform = glGetUniformLocation(gProgram, "uTexture"); */

    /* glGenBuffers(1, &gSquareIndicesBuffer); */
    /* glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, gSquareIndicesBuffer); */
    /* glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLuint) * 3, */
    /*              gSquareIndices, GL_STATIC_DRAW); */
    /* glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0); */

    /* The following two lines enable semi transparent. */
    GL_CHECK(glEnable, GL_BLEND);
    GL_CHECK(glBlendFunc, GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
 
	GL_CHECK(glDisable, GL_DEPTH_TEST);
	GL_CHECK(glDepthMask, GL_FALSE);

    /* Set up viewport */
    camera_set_viewport(0, 0, width, height);
}

static void
render_triangle()
{
    glClearColor(0.0f, 0.4f, 1.0f, 1.0f);

    glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);

    glUseProgram(gProgram);

    glEnableVertexAttribArray(gvPositionAttrib);
    glVertexAttribPointer(gvPositionAttrib, 2, GL_FLOAT, GL_FALSE, 0,
                          gTriangleVertices);

    glEnableVertexAttribArray(gvColorAttrib);
    glVertexAttribPointer(gvColorAttrib, 4, GL_FLOAT, GL_FALSE, 0,
                          gTriangleColors);

    glDrawArrays(GL_TRIANGLES, 0, 3);
}

static void
render_square()
{
    /* glClearColor(1.0f, 0.4f, 0.0f, 1.0f); */
    glClearColor(0.0f, 0.4f, 1.0f, 1.0f);

    glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);

    glUseProgram(gProgram);

    glEnableVertexAttribArray(gvPositionAttrib);
    glVertexAttribPointer(gvPositionAttrib, 2, GL_FLOAT, GL_FALSE, 0,
                          gSquareVertices);

    glEnableVertexAttribArray(gvColorAttrib);
    glVertexAttribPointer(gvColorAttrib, 4, GL_FLOAT, GL_FALSE, 0,
                          gSquareColors);

    glEnableVertexAttribArray(gvTextureAttrib);
    glVertexAttribPointer(gvTextureAttrib, 2, GL_FLOAT, GL_FALSE, 0,
                          gSquareTexices);

    glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, gSquareTextureBuffer);
    glUniform1i(gTextureUniform, 0);
	
    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
}

static void
render_bina()
{
    renderer_pre_render(0.0f, 0.4f, 1.0f, 1.0f);
    sprite_render(background);
}

void
renderer_pre_render(float r, float g, float b, float a)
{
    GL_CHECK(glClearColor, r, g, b, a);
    glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
}

void
renderer_render()
{
    /* render_triangle(); */
    /* render_square(); */
    render_bina();
}

