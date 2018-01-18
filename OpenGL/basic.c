#include <GL/glut.h>

#include <GL/gl.h>

// https://www.khronos.org/opengl/wiki/OpenGL_Reference
// https://www.khronos.org/opengl/wiki/OpenGL_Type

#define WINDOW_NAME "OpenGL basic"

#define USE_VERTEX_ARRAY

static void init(void) {
#if 1
  // Get parameter values.
  GLclampf color[4];
  glGetFloatv(GL_COLOR_CLEAR_VALUE, color);

  // Specify color buffer default values.
  // default: [0.0, 0.0, 0.0, 0.0] (black)
  const GLfloat red = 1.0;
  const GLfloat green = 1.0;
  const GLfloat blue = 1.0;
  const GLfloat alpha = 1.0;
  glClearColor(red, green, blue, alpha);
#endif

#if 0
  // Server-side GL capabilities.
  const GLenum cap = GL_DEPTH_TEST;
  GLboolean depthTest = glIsEnabled(cap);
  glEnable(cap);
  glDisable(cap);
#endif
}

static void display(void) {
  // Clear color buffer.
  glClear(GL_COLOR_BUFFER_BIT);

#if 1
#ifdef USE_VERTEX_ARRAY
  // Client-side capabilities.
  glEnableClientState(GL_VERTEX_ARRAY);

  const GLint size = 2;
  const GLenum type = GL_FLOAT;
#if 1
  const GLsizei stride = 0; // 0 means continuous data.
#else
  const GLsizei stride = sizeof(GLfloat) * 2;
#endif
  const GLfloat vertex[] = {
     0.0, 0.0, // 0
    -1.0, 0.9, // 1
     1.0, 0.9, // 2
  };
  glVertexPointer(size, type, stride, vertex);

  glColor3f(0.0, 0.0, 1.0);
#if 0
  glBegin(GL_TRIANGLES);
    int i;
    for(i = 0; i < 3; ++i)
      glArrayElement(i);
  glEnd();
#elif 0
  {
    const GLsizei count = 3;
    const GLenum type = GL_UNSIGNED_BYTE; // type of indices.
    const GLubyte indices[] = {0, 1, 2};
    glDrawElements(GL_TRIANGLES, count, type, indices);
  }
#else
  {
    const GLint first = 0; // starting index.
    const GLsizei count = 3;
    glDrawArrays(GL_TRIANGLES, first, count);
  }
#endif
#else
  // Set Vertex.
  glBegin(GL_TRIANGLES);
    glColor3f(0.0, 0.0, 1.0); // default: [1.0, 1.0, 1.0, 0.0] (white)
    glVertex2f(0.0, 0.0);  // ( 0.0, 0.0)
    glVertex2f(-1.0, 0.9); // (-1.0, 0.9)
    glVertex2f(1.0, 0.9);  // ( 1.0, 0.9)
  glEnd();
#endif
#endif

#if 0
  // do not work...
  glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
  glColor3f(0.0, 0.0, 1.0);
  glBegin(GL_TRIANGLES);
    // Either boundary or nonboundary.
    glEdgeFlag(GL_FALSE); // default: GL_TRUE
    glVertex3f(-0.5, 0.5, 0.0);
    glEdgeFlag(GL_TRUE);
    glVertex3f(-0.5, -0.5, 0.0);
    glVertex3f(0.5, 0.5, 0.0);
  glEnd();
#endif

#if 0
  const GLfloat x1 = 0.0, y1 = 0.0;
  const GLfloat x2 = 1.0, y2 = 1.0;
#if 1
  glRectf(x1, y1, x2, y2);
#else
  // equivalent
  glBegin(GL_POLYGON);
    glVertex2f(x1 , y1);
    glVertex2f(x2 , y1);
    glVertex2f(x2 , y2);
    glVertex2f(x1 , y2);
  glEnd();
#endif
#endif

  // Camera(default)
  // pos: (0, 0, 0)
  // Z軸の負の向き

#if 1
  glFlush();
#else
  glFinish();
#endif
}

static void reshape(int width, int height) {
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

  glutMainLoop();

  return 0;
}
