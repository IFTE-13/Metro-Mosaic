#include <iostream>
#include <GL/freeglut.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <vector>
#include <math.h>

using namespace std;

/// Boolean Variables
bool day = false, night = true;

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

/// Shapes Structure
void quad(float x1, float x2, float y1, float y2, Color color, float Tx = 0, float Ty = 0, float s = 1)
{
    glColor3ub(color.r, color.g, color.b);
    glBegin(GL_QUADS);
    glVertex2f(s * x1 + Tx, s * y1 + Ty);
    glVertex2f(s * x2 + Tx, s * y1 + Ty);
    glVertex2f(s * x2 + Tx, s * y2 + Ty);
    glVertex2f(s * x1 + Tx, s * y2 + Ty);
    glEnd();
}

void circle(float x, float y, float radius, float height, Color color)
{
    int triangleAmount = 360;
    glBegin(GL_TRIANGLE_FAN);
    glColor3ub(color.r, color.g, color.b);
    glVertex2f(x, y); // center of circle
    for (int i = 0; i <= 360; i++)
        glVertex2f(x + (radius * cos(i * 2 * 3.1416 / triangleAmount)), y + (height * sin(i * 2 * 3.1416 / triangleAmount)));
    glEnd();
}

void quad(vector<pair<float, float>> coord, Color color = {255, 255, 255}, float Tx = 0, float Ty = 0, float s = 1)
{
    glColor3ub(color.r, color.g, color.b);
    glBegin(GL_QUADS);
    for (int i = 0; i < coord.size(); i++)
        glVertex2d(Tx + s * coord[i].first, Ty + s * coord[i].second);
    glEnd();
}

void triangle(vector<pair<float, float>> coord, Color color = {255, 255, 255}, float Tx = 0, float Ty = 0, float s = 1)
{
    glColor3ub(color.r, color.g, color.b);
    glBegin(GL_TRIANGLES);
    for (int i = 0; i < coord.size(); i++)
        glVertex2d(Tx + s * coord[i].first, Ty + s * coord[i].second);
    glEnd();
}

void polygon(vector<pair<float, float>> coord, Color color = {255, 255, 255}, float Tx = 0, float Ty = 0, float s = 1)
{
    glColor3ub(color.r, color.g, color.b);
    glBegin(GL_POLYGON);
    for (int i = 0; i < coord.size(); i++)
        glVertex2d(Tx + s * coord[i].first, Ty + s * coord[i].second);
    glEnd();
}

/// Sky Function
void Sky(){
    Color skyColor = {25, 19, 65};
    if(night){
        skyColor = {25, 19, 65};
    } else if(day) {
        skyColor = {245, 204, 160};
    }
    polygon({{0, 0}, {1920, 0}, {1920, 1080}, {0, 1080}}, skyColor);
}

/// Star Function
void Star(float x, float y, Color stars = {136, 139, 134}){
    polygon({{x + 0, y + 0}, {x + 5, y + 0}, {x + 5, y + 5}, {x + 0, y + 5}}, stars);
}

void Stars(){
    float arr[45][2] = {
        {150, 1000}, {250, 920}, {360, 970}, {400, 920}, {540, 950},
        {560, 980}, {710, 1000}, {720, 1030}, {810, 930}, {810, 940},
        {820, 950}, {850, 1040}, {880, 920}, {920, 1020}, {970, 910},
        {1000, 920}, {1000, 950}, {1100, 1030}, {1170, 920}, {1200, 910},
        {1200, 930}, {1200, 1040}, {1280, 1050}, {1290, 1040}, {1310, 910},
        {1370, 980}, {1400, 940}, {1400, 940}, {1450, 1030}, {1460, 950},
        {1490, 930}, {1500, 1050}, {1640, 970}, {1660, 970}, {1780, 960},
        {1790, 960}, {1810, 950}, {1880, 970}, {1930, 1050}, {1960, 970},
        {1970, 1010}, {1980, 940}, {1980, 950}, {1990, 970}
    };

    /// Calls of Star function with coordinates
    for (int i = 0; i < 45; ++i) {
        Star(arr[i][0], arr[i][1]);
    }
}

void display()
{
   //sky
    Sky();

    // Stars
    if(night){
        Stars();
    }

    glFlush();
    glutSwapBuffers();
}

void keyboard(unsigned char key, int x, int y){
    switch(key){
    case 'd': /// for day
        day = true;
        night = false;
        break;
    case 'n': /// for night
        day = false;
        night = true;
        break;
    case 27:
        exit(0);
        break;
    }
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
    glutCreateWindow("Metro Mosaic");
    glutKeyboardFunc(keyboard);
    glutFullScreen();
    init();
    glutDisplayFunc(display);
    glutMainLoop();
}
