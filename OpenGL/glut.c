#include <GL/glut.h>

// GLUT
// OpenGL Utility Toolkit
// https://www.opengl.org/resources/libraries/glut/

#define WINDOW_NAME "Test GLUT"

static void display(void) {
}

static void reshape(int width, int height) {
}

int main(int argc, char* argv[])
{
  // Initialization
  glutInit(&argc, argv);
  glutInitWindowPosition(100, 50);
  glutInitWindowSize(200, 100);
  const unsigned int mode = GLUT_RGBA | GLUT_SINGLE; // default
  glutInitDisplayMode(mode);

  // Window Management
  int windowId = glutCreateWindow(WINDOW_NAME);

  // Callback Registration
  glutDisplayFunc(display);
  glutReshapeFunc(reshape);

  // Beginning Event Processing
  glutMainLoop();

  return 0;
}
