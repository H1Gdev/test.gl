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
  ""
  "uniform uint bias;"
  ""
  "shared uint bias2;" // shared within work group
  ""
  // binding means "binding point".
  // buffer means "Shader Storage Buffer".
  "layout(std430, binding = 3) buffer ssboLayout"
  "{"
  "  uint data[];"
  "} ssbo;"
  ""
  "void main() {"
  "  bias2 = 0;"
  "  memoryBarrierShared();" // wait
  "  if (gl_WorkGroupID.x == 1 && gl_WorkGroupID.y == 1 && gl_WorkGroupID.z == 1) {"
  "    bias2 = 0;"
  "  }"
  "  memoryBarrierShared();"
  "  if ((gl_WorkGroupID.x % 2) != 0) {"
  "    ssbo.data[gl_WorkGroupID.x] += gl_GlobalInvocationID.x + bias;"
  "  } else {"
  "    ssbo.data[gl_WorkGroupID.x] = bias + bias2;"
  "  }"
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

  GLint result = GL_FALSE;

#ifdef TEST_INPUT_OUTPUT
  GLuint ssbos[] = {0, 0};
#endif

  // Compile
  glShaderSource(shader, 2, source, NULL); // Set shader code
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
  // https://www.khronos.org/opengl/wiki/Shader_Storage_Buffer_Object
  // OpenGL -> GLSL
  {
    // Create buffer object.
#ifdef GL_VERSION_4_5
    glCreateBuffers(2, ssbos);
#else
    glGenBuffers(2, ssbos);
#endif
    GLuint ssbo = ssbos[0];

    // Bind buffer object.
    glBindBuffer(GL_SHADER_STORAGE_BUFFER, ssbo);

    // Create and initialize data store.(any pre-existing data store is deleted.)
    GLint data[] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9}; // initial value(input data)
    GLsizei size = data ? sizeof(data) : 1024; // data store size
    GLenum usage = GL_STATIC_READ; // only hint to GL implementation...
    glBufferData(GL_SHADER_STORAGE_BUFFER, size, data, usage);

    // GL_READ_WRITE

    // Bind buffer target.
    GLuint index = 3; // SSBO binding point
#if 0
    // binding in GLSL.
    GLuint blockIndex = glGetProgramResourceIndex(program, GL_SHADER_STORAGE_BLOCK, "ssboLayout");
    if (blockIndex != GL_INVALID_INDEX) {
      glShaderStorageBlockBinding(program, blockIndex, index);
    }
#endif
    glBindBufferBase(GL_SHADER_STORAGE_BUFFER, index, ssbo);

    // Update data store.
    glBufferSubData(GL_SHADER_STORAGE_BUFFER, sizeof(GLint) * 8, sizeof(GLint) * 2, data);

    // Unbind buffer object.
    glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0);
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

  // Wait
  glMemoryBarrier(GL_SHADER_STORAGE_BARRIER_BIT);

END:
#ifdef TEST_INPUT_OUTPUT
  {
    GLuint ssbo = ssbos[0];
    glBindBuffer(GL_SHADER_STORAGE_BUFFER, ssbo);
    // Map buffer object for read from GPU.
#if 1
    GLvoid* data = glMapBuffer(GL_SHADER_STORAGE_BUFFER, GL_READ_ONLY);
    for (GLuint i = 0; i < 10; ++i) {
      cout << "data[" << i << "] is " << ((GLint*)data)[i] << endl;
    }
    glUnmapBuffer(GL_SHADER_STORAGE_BUFFER);
#else
    GLint data[10];
    glGetBufferSubData(GL_SHADER_STORAGE_BUFFER, 0, sizeof(data), data);
    for (GLuint i = 0; i < 10; ++i) {
      cout << "data[" << i << "] is " << data[i] << endl;
    }
#endif
    glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0);

    glDeleteBuffers(2, ssbos);
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
