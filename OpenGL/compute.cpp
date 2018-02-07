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

using namespace std;

#define WINDOW_NAME "Use Compute Shader"

// https://www.khronos.org/opengl/wiki/Compute_Shader

//#define MINIMUM_SHADER_CODE
#define TEST_INPUT_OUTPUT

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

// shader code
const char* source[] = {
#ifdef MINIMUM_SHADER_CODE
  "#version 430\n", // version 4.30
  "layout(local_size_x = 8, local_size_y = 8, local_size_z = 8) in;" // Local size
  "void main() {"
  "}"
#endif
#ifdef TEST_INPUT_OUTPUT
  "#version 430\n",
  "layout(local_size_x = 8, local_size_y = 8, local_size_z = 8) in;"
  "uniform uint bias;"
  "layout(std430, binding = 3) buffer ssboLayout"
  "{"
  "  uint data[];"
  "} ssbo;"
  "void main() {"
  "  ssbo.data[gl_WorkGroupID.x] += gl_GlobalInvocationID + bias;"
  "}"
#endif
};

static void compute() {
  GLuint program = glCreateProgram();
  GLuint shader = glCreateShader(GL_COMPUTE_SHADER);

  cout << "glIsProgram()=" << static_cast<bool>(glIsProgram(program)) << endl;
  cout << "glIsShader()=" << static_cast<bool>(glIsShader(shader)) << endl;

  // Limitation
  // https://www.khronos.org/opengl/wiki/Compute_Shader#Limitations
  {
    GLint value = 0;
    char name[3] = {'x', 'y', 'z'};

    // Work group count
    for (GLuint index = 0; index < 3; ++index) {
      glGetIntegeri_v(GL_MAX_COMPUTE_WORK_GROUP_COUNT, index, &value);
      cout << name[index] << " max count is " << value << endl;
    }
    // Work group size(Local size)
    for (GLuint index = 0; index < 3; ++index) {
      glGetIntegeri_v(GL_MAX_COMPUTE_WORK_GROUP_SIZE, index, &value);
      cout << name[index] << " max size is " << value << endl;
    }
    glGetIntegerv(GL_MAX_COMPUTE_WORK_GROUP_INVOCATIONS, &value);
    cout << "max Invocation is " << value << endl;
  }

  glShaderSource(shader, 2, source, NULL);

  int result = GL_FALSE;

  GLint* data = NULL;
  size_t length = 10;

  // Compile
  glCompileShader(shader);
  glGetShaderiv(shader, GL_COMPILE_STATUS, &result);
  if (result != GL_TRUE) {
    GLint length = 0;
    glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &length);
    GLchar* infoLog = new GLchar[length];
    glGetShaderInfoLog(shader, length, &length, infoLog);
    cout << infoLog << endl;
    delete[] infoLog;
    goto END;
  }

  glAttachShader(program, shader);

  // Link
  glLinkProgram(program);
  glGetProgramiv(program, GL_LINK_STATUS, &result);
  if (result != GL_TRUE) {
    GLint length = 0;
    glGetProgramiv(program, GL_INFO_LOG_LENGTH, &length);
    GLchar* infoLog = new GLchar[length];
    glGetProgramInfoLog(program, length, &length, infoLog);
    cout << infoLog << endl;
    delete[] infoLog;
    goto END;
  }

  // Use
  glUseProgram(program);

#ifdef TEST_INPUT_OUTPUT
  {
    // Uniform variable is not input in GLSL.
    GLuint bias = 10;
    GLint location = glGetUniformLocation(program, "bias");
    glUniform1ui(location, bias);
  }

  // Shader Storage Buffer Object
  // OpenGL -> GLSL
  {
    GLuint ssbo;
    glGenBuffers(1, &ssbo);
    glBindBuffer(GL_SHADER_STORAGE_BUFFER, ssbo);
    data = new GLint[length];
    for (GLuint i = 0; i < length; ++i) {
      data[i] = i;
    }
    GLenum usage =  GL_DYNAMIC_COPY;
    glBufferData(GL_SHADER_STORAGE_BUFFER, sizeof(GLint) * length, data, usage);
    GLuint index = 3;
    glBindBufferBase(GL_SHADER_STORAGE_BUFFER, index, ssbo);
    glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0); // unbind
  }
#endif

  // Execute
  {
#if 1
    GLuint x = 10; // from 0 to 10
    GLuint y = 10;
    GLuint z = 10;
    glDispatchCompute(x, y, z);
#else
    GLuint indirect[3] = {10, 10, 10};
    glDispatchComputeIndirect(indirect);
#endif
  }

END:
#ifdef TEST_INPUT_OUTPUT
  if (data) {
    for (GLuint i = 0; i < length; ++i) {
      data[i] = i;
    }
    delete[] data;
  }
#endif

  glDetachShader(program, shader);

  glDeleteShader(shader);
  glDeleteProgram(program);
}

int main(int argc, char* argv[])
{
  glutInit(&argc, argv);
  glutInitWindowPosition(100, 50);
  glutInitWindowSize(200, 100);
  glutInitDisplayMode(GLUT_RGBA | GLUT_SINGLE);

  int windowId = glutCreateWindow(WINDOW_NAME);

  init();

  glutDisplayFunc(display);
  glutReshapeFunc(reshape);

  compute();

  glutMainLoop();

  return 0;
}
