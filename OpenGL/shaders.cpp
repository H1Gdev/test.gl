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

static void compileShader(GLuint shader) {
  try {
    GLint result = GL_FALSE;
    // Compile
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
    throw;
  }
}
static void linkProgram(GLuint program) {
  try {
    GLint result = GL_FALSE;
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
    throw;
  }
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

static GLuint createShader(GLenum shaderType, const GLchar** source) {
  GLuint shader = glCreateShader(shaderType);
  try {
    glShaderSource(shader, 2, source, nullptr);
    compileShader(shader);
  } catch (...) {
    glDeleteShader(shader);
    shader = 0;
  }
  return shader;
}
static void terminateProgram(GLuint program, GLboolean shaders = GL_TRUE) {
  if (glIsProgram(program) == GL_TRUE) {
    GLsizei count = 0;
    glGetProgramiv(program, GL_ATTACHED_SHADERS, &count);
    vector<GLuint> attachedShaders(count);
    glGetAttachedShaders(program, count, &count, attachedShaders.data());
    for (GLint i = 0; i < count; ++i) {
      GLuint shader = attachedShaders[i];
      glDetachShader(program, shader);
      if (shaders == GL_TRUE) {
        glDeleteShader(shader);
      }
    }
    glDeleteProgram(program);
  }
}

// shader code
const GLchar* vShader[] = {
  "#version 430\n",
  "in vec3 position;"
  "uniform mat4 mvpMatrix;" // MVP(Model-View-Projection)
  "void main() {"
  "  gl_Position = mvpMatrix * vec4(position, 1.0);"
  "}"
};
const GLchar* fShader[] = {
  "#version 430\n",
  "void main() {"
  "  gl_FragColor = vec4(1.0, 1.0, 1.0, 1.0);"
  "}"
};
const GLchar* cShader[] = {
  // Scale up/down the same aspect ratio textures.
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
#if 0
const GLchar* cShader[] = {
  // Convert from YUV420SP to RGB.
  "#version 430\n",
  "layout(local_size_x = 8, local_size_y = 8) in;"
  "uniform sampler2D inTexY;"
  "uniform sampler2D inTexUV;"
  "layout(binding = 0, rgba8) writeonly uniform image2D outTex;"
  "void main() {"
  "  ivec2 pos = ivec2(gl_GlobalInvocationID.xy);"
  "  float y = texture(inTexY, (vec2(pos) + 0.5) / vec2(textureSize(inTexY, 0))).r;"
  "  vec2 uv = texture(inTexUV, (vec2(pos / 2) + 0.5) / vec2(textureSize(inTexUV, 0))).rg - 0.5;"
  "  float r = y                  + (1.402 * uv.g);"
  "  float g = y - (0.344 * uv.r) - (0.714 * uv.g);"
  "  float b = y + (1.772 * uv.r);"
  "  vec4 color = max(min(vec4(r, g, b, 1.0), 1.0), 0.0);"
  "  imageStore(outTex, pos, color);"
  "}"
};
#endif

static GLuint rProgram = 0;
static GLuint cProgram = 0;

static GLuint textures[] = {0, 0, 0};

static void init(void) {
#ifdef USE_GLEW
  if (glewInit() != GLEW_OK) {
    cerr << "Failed to initialize GLEW..." << endl;
  }
#endif

  // Programs may not contain both a compute and a non-compute shader.
  rProgram = glCreateProgram();
  cProgram = glCreateProgram();

  GLuint shader;
  // Vertex Shader
  shader = createShader(GL_VERTEX_SHADER, vShader);
  // OpenGL 4.6   : Multiple shader objects of the same type may be attached to a single program object.
  // OpenGL ES 3.2: Multiple shader objects of the same type may not be attached to a single program object.
  glAttachShader(rProgram, shader);

  // Fragment Shader
  shader = createShader(GL_FRAGMENT_SHADER, fShader);
  glAttachShader(rProgram, shader);

  // Compute Shader
  shader = createShader(GL_COMPUTE_SHADER, cShader);
  glAttachShader(cProgram, shader);

  linkProgram(rProgram);
  linkProgram(cProgram);

  // Textures
  {
    const GLsizei n = sizeof(textures) / sizeof(GLuint);
#ifdef GL_VERSION_4_5
    glCreateTextures(GL_TEXTURE_2D, n, textures);
#else
    glGenTextures(n, textures);
#endif
  }
}

static void term(void) {
  {
    const GLsizei n = sizeof(textures) / sizeof(GLuint);
    glDeleteTextures(n, textures);
  }

  terminateProgram(cProgram);
  cProgram = 0;
  terminateProgram(rProgram);
  rProgram = 0;
}

static void display(void) {
  // Compute
  glUseProgram(cProgram);
  {
    const GLsizei width = 8;
    const GLsizei height = 8;

    const GLuint x = ((GLuint)width) >> 3;
    const GLuint y = ((GLuint)height) >> 3;
    const GLuint z = 1;
    glDispatchCompute(x, y, z);
    glMemoryBarrier(GL_SHADER_IMAGE_ACCESS_BARRIER_BIT);
  }

  // Render
  glUseProgram(rProgram);
  {
  }
}

static void reshape(int width, int height) {
  glutReshapeWindow(width, height);
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

  // This function will be never called.
  term();

  return 0;
}
