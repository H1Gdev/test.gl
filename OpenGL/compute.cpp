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

// gl_NumWorkGroups (2, 1, 1)
//
// [WorkGroup]  // smallest amount that user can execute.
//  gl_WorkGroupID (0, 0, 0)
//  gl_WorkGroupSize (2, 1, 1)  // Local size
//   [Invocation]
//    gl_LocalInvocationID  (0, 0, 0)
//    gl_GlobalInvocationID (0, 0, 0)
//    gl_LocalInvocationIndex 0
//   [Invocation]
//    gl_LocalInvocationID  (1, 0, 0)   // [0, 2), [0, 1), [0, 1)
//    gl_GlobalInvocationID (1, 0, 0)
//    gl_LocalInvocationIndex 1
// [WorkGroup]
//  gl_WorkGroupID (1, 0, 0)    // [0, 2), [0, 1), [0, 1)
//  gl_WorkGroupSize (2, 1, 1)
//   [Invocation]
//    gl_LocalInvocationID  (0, 0, 0)
//    gl_GlobalInvocationID (2, 0, 0)
//    gl_LocalInvocationIndex 2
//   [Invocation]
//    gl_LocalInvocationID  (1, 0, 0)
//    gl_GlobalInvocationID (3, 0, 0)
//    gl_LocalInvocationIndex 3

//#define MINIMUM_SHADER_CODE
//#define TEST_SSBO_INPUT_OUTPUT
#define TEST_TEXTURE_INPUT_OUTPUT

#ifdef TEST_TEXTURE_INPUT_OUTPUT
#define USE_COMMON_TEXTURE         // Use the same texture object and image binding index.
#define USE_SAMPLER_TEXTURE        // Use sampler for input texture.
//#define USE_FRAMEBUFFER_TO_READ  // Cannot use glGetTexImage() in OpenGL ES, so...
#endif

static void init(void) {
#ifdef USE_GLEW
  if (glewInit() != GLEW_OK) {
    cerr << "Failed to initialize GLEW..." << endl;
  }
#endif
}

static void display(void) {
}

// shader code
static const GLchar* source[] = {
#ifdef MINIMUM_SHADER_CODE
  "#version 430\n", // version 4.30
  "layout(local_size_x = 8, local_size_y = 8, local_size_z = 8) in;" // Local size
  "void main() {"
  "}"
#endif
#ifdef TEST_SSBO_INPUT_OUTPUT
  "#version 430\n",
  "layout(local_size_x = 8, local_size_y = 8, local_size_z = 8) in;"
  ""
  // https://www.khronos.org/opengl/wiki/Type_Qualifier_(GLSL)
  "uniform uint bias;"
  ""
  "shared uint bias2[8];" // shared within work group
  ""
  // https://www.khronos.org/opengl/wiki/Interface_Block_(GLSL)
  // https://www.khronos.org/opengl/wiki/Layout_Qualifier_(GLSL)
  "layout(std430, binding = 3) buffer ssboLayout"
  // binding means "binding point".
  // buffer means "Shader Storage Buffer".
  "{"
  "  uint data[];"
  "} ssbo;"
  ""
  "void main() {"
#if 0
  "  if (gl_GlobalInvocationID.y == 0 && gl_GlobalInvocationID.z == 0) {"
  "    ssbo.data[gl_LocalInvocationID.x] = gl_LocalInvocationID.x;"
  "  }"
#endif
#if 0
  "  if (gl_WorkGroupID == vec3(0, 0, 0)) {"
  "    bias2[gl_LocalInvocationID.x] = gl_GlobalInvocationID.x;"
  "    memoryBarrierShared();" // wait
  "    barrier();"
  "    ssbo.data[gl_LocalInvocationID.x] = bias2[gl_LocalInvocationID.x + 1];"
  "  }"
#endif
  "  if ((gl_WorkGroupID.x % 2) != 0) {"
  "    ssbo.data[gl_WorkGroupID.x] += gl_GlobalInvocationID.x + bias;"
  "  } else {"
  "    ssbo.data[gl_WorkGroupID.x] = bias + bias2[gl_LocalInvocationID.x + 1];"
  "  }"
  "}"
#endif
#ifdef TEST_TEXTURE_INPUT_OUTPUT
  "#version 430\n"
  // https://stackoverflow.com/questions/1914115/converting-color-value-from-float-0-1-to-byte-0-255
  "#define COLOR_F2B(f) (clamp(int(floor((f) * 256.0)), 0, 255))\n"
  "#define COLOR_B2F(b) (float(b) / 255.0)\n",
  "layout(local_size_x = 1, local_size_y = 1) in;"
#ifdef USE_COMMON_TEXTURE
#ifdef USE_SAMPLER_TEXTURE
  "uniform sampler2D inTex;"
#else
  "layout(binding = 1, rgba8) readonly uniform image2D inTex;"
#endif
  "layout(binding = 1, rgba8) writeonly uniform image2D outTex;"
#else
#ifdef USE_SAMPLER_TEXTURE
  "uniform sampler2D inTex;"
#else
  "layout(binding = 6, rgba8) readonly uniform image2D inTex;"
#endif
  "layout(binding = 7, rgba8) writeonly uniform image2D outTex;"
#endif
  "void main() {"
  "  ivec2 pos = ivec2(gl_GlobalInvocationID.xy);"
#ifdef USE_SAMPLER_TEXTURE
#if 1
  // https://stackoverflow.com/questions/40574677/how-to-normalize-image-coordinates-for-texture-space-in-opengl
  "  ivec2 size = textureSize(inTex, 0);"
  "  vec2 posCenter = vec2(pos) + 0.5;" // center of texel
  "  vec2 posNormalized = posCenter / vec2(size);" // = vec2(posCenter.x / float(size.x), posCenter.y / float(size.y));
  "  vec4 texel = texture(inTex, posNormalized);"
#else
  // Like imageLoad.(No interpolation.)
  "  vec4 texel = texelFetch(inTex, pos, 0);"
#endif
#else
  "  vec4 texel = imageLoad(inTex, pos);"
#endif
  "  imageStore(outTex, pos, texel);"
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
    // Minimum values are defined.
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

    // Max all shared variables total size
    glGetIntegerv(GL_MAX_COMPUTE_SHARED_MEMORY_SIZE, &value);
    cout << "max all shared variables total size is " << value << endl;
  }
#ifdef TEST_TEXTURE_INPUT_OUTPUT
  // https://www.khronos.org/opengl/wiki/Textures_-_more
  {
    GLint value = 0;

    glGetIntegerv(GL_MAX_TEXTURE_SIZE, &value);
    cout << "max texture size is " << value << "x" << value << endl;

    glGetIntegerv(GL_MAX_3D_TEXTURE_SIZE, &value);
    cout << "max 3D texture size is " << value << "x" << value << endl;

    glGetIntegerv(GL_MAX_ARRAY_TEXTURE_LAYERS, &value);
    cout << "max array texture layers are " << value << endl;
  }
#endif

  GLint result = GL_FALSE;

  // WorkGroups
  GLuint x = 10; // from 0 to 9
  GLuint y = 10;
  GLuint z = 10;


#ifdef TEST_SSBO_INPUT_OUTPUT
  GLuint ssbos[] = {0, 0};
#endif
#ifdef TEST_TEXTURE_INPUT_OUTPUT
  GLuint textures[] = {0, 0};
#ifdef USE_SAMPLER_TEXTURE
  GLuint samplers[] = {0};
#endif
#ifdef USE_FRAMEBUFFER_TO_READ
  GLuint framebuffers[] = {0};
#endif
#endif

  // Compile
  glShaderSource(shader, 2, source, nullptr); // Set shader code
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

#ifdef TEST_SSBO_INPUT_OUTPUT
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
#ifdef TEST_TEXTURE_INPUT_OUTPUT
  {
#ifdef USE_SAMPLER_TEXTURE
    glGenSamplers(1, samplers);

    GLuint sampler = samplers[0];
#if 1
    // use Bilinear.
    glSamplerParameteri(sampler, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glSamplerParameteri(sampler, GL_TEXTURE_MAG_FILTER, GL_LINEAR); // affect.
#else
    // use Nearest neighbor.
    glSamplerParameteri(sampler, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glSamplerParameteri(sampler, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
#endif
    //         (Position)   -0.50  -0.25    0.00  1.00    1.25  1.50
    // GL_REPEAT             0.50   0.75    0.00  1.00    0.25  0.50
    // GL_MIRRORED_REPEAT    0.50   0.25    0.00  1.00    0.75  0.50
    // GL_CLAMP_TO_EDGE      0.00   0.00    0.00  1.00    1.00  1.00
    glSamplerParameteri(sampler, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glSamplerParameteri(sampler, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
#endif

    // Create texture object.
#ifdef GL_VERSION_4_5
    glCreateTextures(GL_TEXTURE_2D, 2, textures);
#else
    glGenTextures(2, textures);
#endif
    GLuint texture;

    const GLsizei width = 2;
    const GLsizei height = 2;

    // GL_RGBA, GL_UNSIGNED_BYTE
    GLbyte color[width * height * 4] = {
      // R     G     B     A
      0x52, 0x47, 0x42, 0xff,
      0x42, 0x49, 0x54, 0xff,
      0x53, 0x53, 0x42, 0xff,
      0xff, 0xff, 0xff, 0xff,
    };

#ifdef USE_COMMON_TEXTURE
    // Input and Output Texture
    texture = textures[0];
    glActiveTexture(GL_TEXTURE0 + 1);
    glBindTexture(GL_TEXTURE_2D, texture);
    // Once a texture is specified with this command, the format and dimensions of all levels become immutable unless it is a proxy texture.
    glTexStorage2D(GL_TEXTURE_2D, 1, GL_RGBA8, width, height);
    // Input settings
    glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, width, height, GL_RGBA, GL_UNSIGNED_BYTE, color);
#ifdef USE_SAMPLER_TEXTURE
    glUniform1i(glGetUniformLocation(program, "inTex"), 1);
    glBindSampler(1, sampler);
#else
    glBindImageTexture(1, texture, 0, GL_FALSE, 0, GL_READ_WRITE, GL_RGBA8);
#endif

    // Output settings
#ifdef USE_SAMPLER_TEXTURE
    glBindImageTexture(1, texture, 0, GL_FALSE, 0, GL_READ_WRITE, GL_RGBA8);
#endif
#else
    // Input Texture
    texture = textures[0];
    glActiveTexture(GL_TEXTURE0 + 1);
    glBindTexture(GL_TEXTURE_2D, texture);
    glTexStorage2D(GL_TEXTURE_2D, 1, GL_RGBA8, width, height); // cannot use glTexImage2D()..
    glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, width, height, GL_RGBA, GL_UNSIGNED_BYTE, color);
#ifdef USE_SAMPLER_TEXTURE
    glUniform1i(glGetUniformLocation(program, "inTex"), 1);
    glBindSampler(1, sampler);
#else
    GLuint bindingIndex = 6;
#if 0
    // Change image binding index.
    glUniform1i(glGetUniformLocation(program, "inTex"), bindingIndex);
#endif
    glBindImageTexture(bindingIndex, texture, 0, GL_FALSE, 0, GL_READ_ONLY, GL_RGBA8);
#endif

    // Output Texture
    texture = textures[1];
    glActiveTexture(GL_TEXTURE0 + 2);
    glBindTexture(GL_TEXTURE_2D, texture);
    glTexStorage2D(GL_TEXTURE_2D, 1, GL_RGBA8, width, height); // cannot use glTexImage2D()..
    glBindImageTexture(7, texture, 0, GL_FALSE, 0, GL_WRITE_ONLY, GL_RGBA8);
#endif

    x = width;
    y = height;
    z = 1;
  }
#endif

  // Execute
  {
#if 1
    glDispatchCompute(x, y, z); // GLuint
#else
    GLuint indirect[3] = {x, y, z};
    glDispatchComputeIndirect(indirect); // GLintptr because GLuintptr is not defined ?
#endif
  }

  // Barrier
  // Do not wait immediately.
#ifdef TEST_SSBO_INPUT_OUTPUT
  glMemoryBarrier(GL_SHADER_STORAGE_BARRIER_BIT);
#endif
#ifdef TEST_TEXTURE_INPUT_OUTPUT
  glMemoryBarrier(GL_SHADER_IMAGE_ACCESS_BARRIER_BIT);
#endif

END:
#ifdef TEST_SSBO_INPUT_OUTPUT
  {
    GLuint ssbo = ssbos[0];
    glBindBuffer(GL_SHADER_STORAGE_BUFFER, ssbo);
    // Read from GPU.
#if 1
    // Map buffer object
    GLvoid* data = glMapBuffer(GL_SHADER_STORAGE_BUFFER, GL_READ_ONLY);
    if (data) {
      for (GLuint i = 0; i < 10; ++i) {
        cout << "data[" << i << "] is " << ((GLint*)data)[i] << endl;
      }
    }
    glUnmapBuffer(GL_SHADER_STORAGE_BUFFER);
#else
    // Get data store
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
#ifdef TEST_TEXTURE_INPUT_OUTPUT
  {
    // Read from GPU.
    const GLsizei width = 2;
    const GLsizei height = 2;
    GLbyte outColor[width * height * 4];
#ifdef USE_COMMON_TEXTURE
    GLuint texture = textures[0];
#else
    GLuint texture = textures[1];
#endif

#ifdef USE_FRAMEBUFFER_TO_READ
    // Create framebuffer object.
    glGenFramebuffers(1, framebuffers);

    GLuint framebuffer = framebuffers[0];
    glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, texture, 0);

    glReadPixels(0, 0, width, height, GL_RGBA, GL_UNSIGNED_BYTE, outColor);
    glBindFramebuffer(GL_FRAMEBUFFER, 0); // unbind

    glDeleteFramebuffers(1, framebuffers);
#else
    glBindTexture(GL_TEXTURE_2D, texture);
    glGetTexImage(GL_TEXTURE_2D, 0, GL_RGBA, GL_UNSIGNED_BYTE, outColor);
    glBindTexture(GL_TEXTURE_2D, 0); // unbind
#endif

#ifdef USE_SAMPLER_TEXTURE
    glDeleteSamplers(1, samplers);
#endif

    glDeleteTextures(2, textures);
  }
#endif

  glDetachShader(program, shader);

  glDeleteShader(shader);
  glDeleteProgram(program);
}

int main(int argc, char* argv[]) {
  glutInit(&argc, argv);
  glutInitWindowPosition(100, 50);
  glutInitWindowSize(200, 100);
  glutInitDisplayMode(GLUT_RGBA | GLUT_SINGLE);

  int windowId = glutCreateWindow(WINDOW_NAME);

  init();

  glutDisplayFunc(display);

  compute();

  glutMainLoop();

  return 0;
}
