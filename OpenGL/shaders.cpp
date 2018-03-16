// GLEW
// OpenGL Extension Wrangler Library
// http://glew.sourceforge.net/
#define USE_GLEW

#ifdef USE_GLEW
#include <GL/glew.h>
#else
#include <GL/gl.h>
#endif

#include <GL/glut.h>

#include <iostream>

#include <stdexcept>
#include <vector>
#include <sstream>

using namespace std;

#define WINDOW_NAME "Test Shader code"

static GLuint createShader(GLenum shaderType, const GLchar** source) {
  GLuint shader = glCreateShader(shaderType);

  try {
    GLint result = GL_FALSE;
    // Compile
    glShaderSource(shader, 2, source, NULL);
    glCompileShader(shader);
    glGetShaderiv(shader, GL_COMPILE_STATUS, &result);
    if (result != GL_TRUE) {
      GLint length = 0;
      glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &length);
      vector<GLchar> infoLog(length + 1);
      infoLog[length] = '\0';
      glGetShaderInfoLog(shader, length, &length, infoLog.data());
      throw logic_error(infoLog.data());
    }
  } catch (const exception& e) {
    cerr << "glCompileShader():" << e.what() << endl;

    glDeleteShader(shader);
    shader = 0;
  }
  return shader;
}
static GLuint createProgram(GLuint shader) {
  GLuint program = glCreateProgram();

  try {
    GLint result = GL_FALSE;
    // OpenGL 4.6   : Multiple shader objects of the same type may be attached to a single program object.
    // OpenGL ES 3.2: Multiple shader objects of the same type may not be attached to a single program object.
    glAttachShader(program, shader);

    // Link
    glLinkProgram(program);
    glGetProgramiv(program, GL_LINK_STATUS, &result);
    if (result != GL_TRUE) {
      GLint length = 0;
      glGetProgramiv(program, GL_INFO_LOG_LENGTH, &length);
      vector<GLchar> infoLog(length + 1);
      infoLog[length] = '\0';
      glGetProgramInfoLog(program, length, &length, infoLog.data());
      throw logic_error(infoLog.data());
    }
  } catch (const exception& e) {
    cerr << "glLinkProgram():" << e.what() << endl;

    glDeleteProgram(program);
    program = 0;
  }
  return program;
}
static void checkError(const GLchar* title) {
  // https://www.khronos.org/opengl/wiki/OpenGL_Error
  GLenum error = glGetError();
  if (error != GL_NO_ERROR) {
    try {
      ostringstream sout;
      sout << error;
      throw logic_error(sout.str());
    } catch (const exception& e) {
      cerr << title << ":" << e.what() << endl;
      throw;
    }
  }
}

// shader code
const GLchar* vShader[] = {
  "#version 430\n",
  "void main() {"
  "}"
};
const GLchar* fShader[] = {
  "#version 430\n",
  "void main() {"
  "}"
};
const GLchar* cShader[] = {
  "#version 430\n",
  "layout(local_size_x = 8, local_size_y = 8) in;"
  "uniform sampler2D inTex;"
  "layout(binding = 0, rgba8) writeonly uniform image2D outTex;"
  "void main() {"
  "  ivec2 pos = ivec2(gl_GlobalInvocationID.xy);"
  "  vec2 normalizedPos = (vec2(pos) + 0.5) / vec2(imageSize(outTex));"
  "  imageStore(outTex, pos, texture(inTex, normalizedPos));"
  "}"
};

static void init(void) {
#ifdef USE_GLEW
  if (glewInit() != GLEW_OK) {
    cerr << "Failed to initialize GLEW..." << endl;
  }
#endif
}

static void display(void) {
}

static void reshape(int width, int height) {
}

static void idle(void) {
  glutPostRedisplay();
}

int main(int argc, char* argv[])
{
  glutInit(&argc, argv);
  glutInitWindowPosition(100, 50);
  glutInitWindowSize(200, 100);
  const unsigned int mode = GLUT_RGBA | GLUT_SINGLE;
  glutInitDisplayMode(mode);

  int windowId = glutCreateWindow(WINDOW_NAME);

  init();

  glutDisplayFunc(display);
  glutReshapeFunc(reshape);
  glutIdleFunc(idle);

  glutMainLoop();

  return 0;
}
