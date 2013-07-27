/**
 * Contains the necessary structs and functions to work with shaders.
 *
 * This allows the creation of program and shaders.
 *
 * From OpenGL's wiki:
 *
 * The Vertex Shader is the programmable Shader stage in the rendering
 * pipeline that handles the processing of individual vertices. Vertex shaders
 * are fed Vertex Attribute data, as specified from a vertex array object by
 * a rendering command. A vertex shader receives a single vertex from the
 * vertex stream and generates a single vertex to the output vertex stream.
 * There must be a 1:1 mapping from input vertices to output vertices.  Vertex
 * shaders typically perform transformations to post-projection space, for
 * consumption by the Vertex Post-Processing stage. They can also be used to
 * do per-vertex lighting, or to perform setup work for later shader stages.
 *
 * A Fragment Shader is a user-supplied program that, when executed, will
 * process a Fragment from the rasterization process into a set of colors and
 * a single depth value.  The fragment shader is the OpenGL pipeline stage
 * after a primitive is rasterized. For each sample of the pixels covered by
 * a primitive, a "fragment" is generated. Each fragment has a Window Space
 * position, a few other values, and it contains all of the interpolated
 * per-vertex output values from the last Vertex Processing stage.  The output
 * of a fragment shader is a depth value, a possible stencil value (unmodified
 * by the fragment shader), and zero or more color values to be potentially
 * written to the buffers in the current framebuffers.  Fragment shaders take
 * a single fragment as input and produce a single fragment as output.
 */

/**
 * @file shader.h
 * @author Jeremy Attali, Johan Attali
 * @date July 23, 2013
 */

#pragma once

/**
 * Creates a new OpenGL shader object.
 *
 * It requires a valid source.
 *
 * @param type The shader type to compile.
 * Can be:
 *  - GL_VERTEX_SHADER
 *  - GL_FRAGMENT_SHADER
 * @param source The source of the shader, can be loaded from a file or
 * memory.
 * @return The id of the shader generated and stored in GPU. 0 if something
 * went wrong.
 */
unsigned int shader_create_shader(GLenum type, const char* source);

/**
 * Creates a new OpenGL program object.
 *
 * It requires two valid sources. One vertex shader and one fragment shader.
 *
 * @param vertex The vertex shader of the program.
 * @param fragment The fragement shader of the program.
 * @return The id of the program generated and stored in GPU. 0 if something
 * went wrong.
 */
unsigned int shader_create_program(const char* vertex, const char* fragment);