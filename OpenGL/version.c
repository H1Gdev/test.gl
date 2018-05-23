#include <GL/glut.h>

#include <GL/gl.h>

#include <stdio.h>
#include <string.h>

#define WINDOW_NAME "OpenGL version"

// https://www.khronos.org/opengl/wiki/Core_Language_(GLSL)#OpenGL_and_GLSL_versions

#ifndef GL_SHADING_LANGUAGE_VERSION
#define GL_SHADING_LANGUAGE_VERSION 0x8B8C
#endif

#if defined(_WIN32) || defined(_WIN64)
#define strtok_r strtok_s
#endif

static void displayInfo(void) {
  // https://www.khronos.org/opengl/wiki/GLAPI/glGetString

  printf("GL_VENDOR is %s\n", glGetString(GL_VENDOR));
  printf("GL_RENDERER is %s\n", glGetString(GL_RENDERER));
  printf("GL_VERSION is %s\n", glGetString(GL_VERSION));
  printf("GL_SHADING_LANGUAGE_VERSION is %s\n", glGetString(GL_SHADING_LANGUAGE_VERSION));

  printf("GL_EXTENSIONS is\n");
#ifdef GL_NUM_EXTENSIONS
  GLint extensions = 0;
  glGetIntegerv(GL_NUM_EXTENSIONS, &extensions);
  for (GLint i = 0; i < extensions; ++i) {
    printf("[%3d]%s\n", i, glGetStringi(GL_EXTENSIONS, i));
  }
#else
  const char* delimiter = " ";
  char* extensions = (char*)glGetString(GL_EXTENSIONS);
  int i = 0;
  char* extension = strtok_r(extensions, delimiter, &extensions);
  if (extension != nullptr) {
    printf("[%3d]%s\n", i++, extension);
     do {
      extension = strtok_r(nullptr, delimiter, &extensions);
      if (extension == nullptr) {
        break;
      }
      printf("[%3d]%s\n", i++, extension);
    } while (1);
  }
#endif
}

static void display(void) {
}

int main(int argc, char* argv[])
{
  glutInit(&argc, argv);
  glutInitWindowPosition(100, 50);
  glutInitWindowSize(200, 100);
  glutInitDisplayMode(GLUT_RGBA | GLUT_SINGLE);

  int windowId = glutCreateWindow(WINDOW_NAME);

  // call after create window.
  displayInfo();

  glutDisplayFunc(display);

  glutMainLoop();

  return 0;
}
