#include <GL/glut.h>

#include <GL/gl.h>

#include <stdio.h>

#define WINDOW_NAME "OpenGL version"

// https://www.khronos.org/opengl/wiki/Core_Language_(GLSL)#OpenGL_and_GLSL_versions

#ifndef GL_SHADING_LANGUAGE_VERSION
#define GL_SHADING_LANGUAGE_VERSION 0x8B8C
#endif

static void displayInfo(void) {
  // https://www.khronos.org/opengl/wiki/GLAPI/glGetString

  printf("GL_VENDOR is %s\n", glGetString(GL_VENDOR));
  printf("GL_RENDERER is %s\n", glGetString(GL_RENDERER));
  printf("GL_VERSION is %s\n", glGetString(GL_VERSION));
  printf("GL_SHADING_LANGUAGE_VERSION is %s\n", glGetString(GL_SHADING_LANGUAGE_VERSION));
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
