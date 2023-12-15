#include <iostream>
#include <GL/freeglut.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <vector>
#include <math.h>

using namespace std;

/// Defining Color for Objects
struct Color
{
    int r;
    int g;
    int b;

    Color(int red, int green, int blue) : r(red), g(green), b(blue) {}
};

/// Generating Gradient Colors
Color interpolateColor(Color color1, Color color2, float t) {
    int r = static_cast<int>((1 - t) * color1.r + t * color2.r);
    int g = static_cast<int>((1 - t) * color1.g + t * color2.g);
    int b = static_cast<int>((1 - t) * color1.b + t * color2.b);
    return Color(r, g, b);
}

void init(void)
{
    glClearColor(0.0F, 0.0F, 0.0F, 1);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(0, 1920, 0, 1080, -100, 100);
}

int main(int argc, char **argv)
{
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_MULTISAMPLE);
    glutInitWindowPosition(0, 0);
    glutInitWindowSize(1920, 1080);
    glutFullScreen();
    init();
    glutMainLoop();
}
