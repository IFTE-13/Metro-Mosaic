#include <iostream>
#include <GL/freeglut.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <vector>
#include <math.h>

using namespace std;

/// Boolean Variables
bool day = false, night = true;

/// Float Variables for Translation
float cloudTime1 = 0;
float cloudTime2 = 0;
float planePosition = 3600;
float carrierPosition = 00;
float carrierTwoPosition = 400;

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

/// Cloud Function
void Clouds(float Tx, float Ty, float s, Color color){
    /// Left Portion
    circle(Tx + s * 2, Ty + s * 30, 60, 60, color);
    circle(Tx + s * 13, Ty + s * 22, 93, 93, color);
    circle(Tx + s * 24, Ty + s * 24, 83, 90, color);
    circle(Tx + s * 40, Ty + s * 32, 120, 120, color);
    circle(Tx + s * 54, Ty + s * 36, 77, 70, color);
    circle(Tx + s * 60, Ty + s * 30, 77, 70, color);
    circle(Tx + s * 66, Ty + s * 40, 77, 84, color);
    circle(Tx + s * 74, Ty + s * 40, 100, 100, color);
    circle(Tx + s * 85, Ty + s * 37, 77, 84, color);
    circle(Tx + s * 85, Ty + s * 32, 77, 84, color);
    circle(Tx + s * 95, Ty + s * 40, 107, 100, color);
    circle(Tx + s * 108, Ty + s * 34, 67, 60, color);
    circle(Tx + s * 122, Ty + s * 31, 100, 100, color);
    circle(Tx + s * 132, Ty + s * 25, 100, 100, color);
    circle(Tx + s * 144, Ty + s * 16, 60, 60, color);

    /// Quads for filling the empty portion
    quad({{0, -10}, {220, -10}, {220, 25}, {0, 25}}, color, Tx, Ty, s);
    quad({{25, 10}, {220, 10}, {220, 32}, {25, 32}}, color, Tx, Ty, s);

    /// Right Portion
    circle(Tx + s * 102, Ty + s * 30, 60, 60, color);
    circle(Tx + s * 113, Ty + s * 16, 93, 93, color);
    circle(Tx + s * 124, Ty + s * 24, 83, 90, color);
    circle(Tx + s * 140, Ty + s * 32, 120, 120, color);
    circle(Tx + s * 154, Ty + s * 36, 77, 70, color);
    circle(Tx + s * 160, Ty + s * 30, 77, 70, color);
    circle(Tx + s * 166, Ty + s * 40, 77, 84, color);
    circle(Tx + s * 174, Ty + s * 40, 100, 100, color);
    circle(Tx + s * 185, Ty + s * 37, 77, 84, color);
    circle(Tx + s * 185, Ty + s * 32, 77, 84, color);
    circle(Tx + s * 195, Ty + s * 40, 107, 100, color);
    circle(Tx + s * 208, Ty + s * 34, 100, 100, color);
    circle(Tx + s * 222, Ty + s * 31, 100, 100, color);
    circle(Tx + s * 232, Ty + s * 25, 100, 100, color);
    circle(Tx + s * 244, Ty + s * 16, 60, 60, color);

}

/// Timer Function for updating Cloud Positions
void updateCloud(int value) {
    cloudTime1 += 0.01;
    cloudTime2 -= 0.01;
    glutPostRedisplay();
    glutTimerFunc(16, updateCloud, 10);
}

void cloud1() {
    Color cloud = {114, 80, 203};
    if(night){
        cloud = {114, 80, 203};
    } else if(day){
        cloud = {228, 123, 50};
    }
    Clouds(0, 100 + 20 * sin(cloudTime1), 10, cloud);
}

void cloud2(){
    Color cloud = {53, 41, 107};
    if(night){
        cloud = {53, 41, 107};
    } else if(day){
        cloud = {153, 77, 28};
    }
    Clouds(-100, 285 + 20 * sin(cloudTime2), 10, cloud);
}

void cloud3(){
    Color cloud = {36, 27, 82};
    if(night){
        cloud = {36, 27, 82};
    } else if(day){
        cloud = {107, 36, 12};
    }
    Clouds(-250, 480 + 20 * sin(cloudTime1), 10, cloud);
}

/// Plane Function
void plane(float x, float y, Color window = {112, 146, 243}, Color body = {0, 0, 0}){
        polygon({{x + 0, y + 0}, {x + 100, y + 0}, {x + 100, y + 15}, {x + 0, y + 15}}, body);
        polygon({{x - 15, y + 0}, {x + 0, y + 0}, {x + 0, y + 15}, {x - 15, y + 15}}, {191, 205, 252});
        polygon({{x - 10, y + 15}, {x + 0, y + 15}, {x + 0, y + 20}, {x - 10, y + 20}}, {191, 205, 252});

        /// Windows
        polygon({{x + 8, y + 5}, {x + 13, y + 5}, {x + 13, y + 10}, {x + 8, y + 10}}, window);
        polygon({{x + 21, y + 5}, {x + 26, y + 5}, {x + 26, y + 10}, {x + 21, y + 10}}, window);
        polygon({{x + 34, y + 5}, {x + 39, y + 5}, {x + 39, y + 10}, {x + 34, y + 10}}, window);
        polygon({{x + 47, y + 5}, {x + 52, y + 5}, {x + 52, y + 10}, {x + 47, y + 10}}, window);
        polygon({{x + 60, y + 5}, {x + 65, y + 5}, {x + 65, y + 10}, {x + 60, y + 10}}, window);
        polygon({{x + 73, y + 5}, {x + 78, y + 5}, {x + 78, y + 10}, {x + 73, y + 10}}, window);

        polygon({{x - 15, y + 0}, {x + 0, y + 0}, {x + 0, y + 5}, {x - 15, y + 5}}, body);
        polygon({{x + 20, y + 15}, {x + 50, y + 15}, {x + 50, y + 20}, {x + 20, y + 20}}, body);
        polygon({{x + 30, y + 20}, {x + 70, y + 20}, {x + 70, y + 25}, {x + 30, y + 25}}, body);
        polygon({{x + 20, y + 0}, {x + 40, y + 0}, {x + 40, y - 5}, {x + 20, y - 5}}, body);
        polygon({{x + 25, y - 5}, {x + 50, y - 5}, {x + 50, y - 10}, {x + 25, y - 10}}, body);
        polygon({{x + 30, y - 10}, {x + 60, y - 10}, {x + 60, y - 15}, {x + 30, y - 15}}, body);

        polygon({{x + 100, y + 3}, {x + 140, y + 3}, {x + 140, y + 7}, {x + 100, y + 7}}, {191, 205, 252});
        polygon({{x + 100, y + 7}, {x + 160, y + 7}, {x + 160, y + 12}, {x + 100, y + 12}}, window);
}

/// Timer Function for updating Plane Positions
void updatePlane(int value) {
    if(planePosition > 0){
        planePosition -= 50;
    }
    else{
        planePosition = 40000;
    }

    glutPostRedisplay();
    glutTimerFunc(1, updatePlane, 0);
}

void movePlane(){
    glPushMatrix();
    glScalef(0.5, 0.5,0.0);
    plane(planePosition, 1500);
    glPopMatrix();
}

/// First Carrier Function
void carrier(float x, float y, Color window = {227, 81, 144}, Color body = {0, 0, 0}){
        polygon({{x + 0, y + 0}, {x + 100, y + 0}, {x + 100, y + 30}, {x + 0, y + 30}}, body);
        polygon({{x + 85, y + 15}, {x + 100, y + 15}, {x + 100, y + 30}, {x + 85, y + 30}}, window);
        polygon({{x + 50, y + 23}, {x + 80, y + 23}, {x + 80, y + 30}, {x + 50, y + 30}}, window);
        polygon({{x + 10, y + 23}, {x + 40, y + 23}, {x + 40, y + 30}, {x + 10, y + 30}}, window);

        /// Windows
        polygon({{x + 10, y + 5}, {x + 20, y + 5}, {x + 20, y + 15}, {x + 10, y + 15}}, window);
        polygon({{x + 30, y + 5}, {x + 40, y + 5}, {x + 40, y + 15}, {x + 30, y + 15}}, window);
        polygon({{x + 50, y + 5}, {x + 60, y + 5}, {x + 60, y + 15}, {x + 50, y + 15}}, window);
        polygon({{x + 70, y + 5}, {x + 80, y + 5}, {x + 80, y + 15}, {x + 70, y + 15}}, window);
}

/// Timer Function for updating First Carrier Positions
void updateCarrier(int value) {
    if(carrierPosition < 3780){
        carrierPosition += 20;
    }
    else{
        carrierPosition = -8000;
    }

    glutPostRedisplay();
    glutTimerFunc(1, updateCarrier, 0);
}

void moveCarrer(){
    glPushMatrix();
    glScalef(0.5, 0.5,0.0);
    carrier(carrierPosition, 1000);
    glPopMatrix();
}

/// Second Carrier Function
void carrierTwo(float x, float y, Color window = {46, 190, 186}, Color body = {0, 0, 0}){
        polygon({{x + 0, y + 0}, {x + 100, y + 0}, {x + 100, y + 30}, {x + 0, y + 30}}, body);
        polygon({{x + 0, y + 15}, {x + 10, y + 15}, {x + 10, y + 25}, {x + 0, y + 25}}, window);
        polygon({{x + 20, y + 20}, {x + 30, y + 20}, {x + 30, y + 25}, {x + 20, y + 25}}, window);
        polygon({{x + 40, y + 20}, {x + 50, y + 20}, {x + 50, y + 25}, {x + 40, y + 25}}, window);
        polygon({{x + 60, y + 20}, {x + 70, y + 20}, {x + 70, y + 25}, {x + 60, y + 25}}, window);
        polygon({{x + 20, y + 10}, {x + 80, y + 10}, {x + 80, y + 15}, {x + 20, y + 15}}, {21, 107, 104});
}

/// Timer Function for updating Second Carrier Positions
void updateCarrierTwo(int value) {
    if(carrierTwoPosition > 0){
        carrierTwoPosition -= 10;
    }
    else{
        carrierTwoPosition = 5000;
    }

    glutPostRedisplay();
    glutTimerFunc(1, updateCarrierTwo, 0);
}

void moveCarrerTwo(){
    glPushMatrix();
    glScalef(0.5, 0.5,0.0);
    carrierTwo(carrierTwoPosition, 550);
    glPopMatrix();
}

/// Building One
void Building_One(float x, float y, int m = 1, Color Pillar = {135, 82, 214}, Color PillarShadow = {33, 72, 136}, Color DarkPillar = {26, 46, 74}, Color DarkPillarShadow = {6, 23, 52}, Color windowDark = {28, 0, 149}, Color windowLightTop = {34, 203, 197}, Color windowLightBottom = {41, 94, 135}){
    if(night){
        windowDark = {28, 0, 149};
        windowLightTop = {34, 203, 197};
        windowLightBottom = {41, 94, 135};
        Pillar = {135, 82, 214};
        PillarShadow = {33, 72, 136};
        DarkPillar = {26, 46, 74};
        DarkPillarShadow = {6, 23, 52};
    } else if(day){
        windowDark = {227, 100, 20};
        windowLightTop = {251, 139, 36};
        windowLightBottom = {145, 15, 64};
        Pillar = {245, 204, 160};
        PillarShadow = {153, 77, 28};
        DarkPillar = {228, 143, 69};
        DarkPillarShadow = {107, 36, 12};
    }
    /// Bottom Portion
    /// Left Portion
    polygon({{x + 0, y + 0}, {x + 15, y + 0}, {x + 15, y + 245}, {x + 0, y + 245}}, PillarShadow);

    /// Pillar One
    polygon({{x + 15, y + 0}, {x + 25, y + 0}, {x + 25, y + 245}, {x + 15, y + 245}}, Pillar);
    polygon({{x + 25, y + 0}, {x + 40, y + 0}, {x + 40, y + 245}, {x + 25, y + 245}}, PillarShadow);

    /// Pillar Two
    polygon({{x + 40, y + 0}, {x + 50, y + 0}, {x + 50, y + 245}, {x + 40, y + 245}}, Pillar);
    polygon({{x + 50, y + 0}, {x + 65, y + 0}, {x + 65, y + 245}, {x + 50, y + 245}}, PillarShadow);

    /// Pillar Three
    polygon({{x + 65, y + 0}, {x + 75, y + 0}, {x + 75, y + 245}, {x + 65, y + 245}}, Pillar);

    /// Pillar Four
    polygon({{x + 105, y + 0}, {x + 115, y + 0}, {x + 115, y + 245}, {x + 105, y + 245}}, DarkPillar);
    polygon({{x + 115, y + 0}, {x + 135, y + 0}, {x + 135, y + 245}, {x + 115, y + 245}}, DarkPillarShadow);

    /// Right Portion
    polygon({{x + 75, y + 0}, {x + 85, y + 0}, {x + 85, y + 245}, {x + 75, y + 245}}, DarkPillar);

    /// Pillar One
    polygon({{x + 85, y + 0}, {x + 105, y + 0}, {x + 105, y + 245}, {x + 85, y + 245}}, DarkPillarShadow);

    /// Window Column One
    polygon({{x + 5, y + 0}, {x + 15, y + 0}, {x + 15, y + 10}, {x + 5, y + 10}}, windowLightBottom);

    polygon({{x + 5, y + 20}, {x + 15, y + 20}, {x + 15, y + 30}, {x + 5, y + 30}}, windowLightTop);
    polygon({{x + 5, y + 30}, {x + 15, y + 30}, {x + 15, y + 40}, {x + 5, y + 40}}, windowLightBottom);

    polygon({{x + 5, y + 50}, {x + 15, y + 50}, {x + 15, y + 60}, {x + 5, y + 60}}, windowLightTop);
    polygon({{x + 5, y + 60}, {x + 15, y + 60}, {x + 15, y + 70}, {x + 5, y + 70}}, windowLightBottom);

    for (int i = 0; i< 6; i++){
        int startY = 80 + i * 30;
        polygon({{x + 5, y + startY}, {x + 15, y + startY}, {x + 15, y + (startY + 20)}, {x + 5, y + (startY + 20)}}, windowDark);
    }

    /// Window Column Two
    polygon({{x + 30, y + 0}, {x + 40, y + 0}, {x + 40, y + 10}, {x + 30, y + 10}}, windowDark);

    polygon({{x + 30, y + 20}, {x + 40, y + 20}, {x + 40, y + 30}, {x + 30, y + 30}}, windowLightTop);
    polygon({{x + 30, y + 30}, {x + 40, y + 30}, {x + 40, y + 40}, {x + 30, y + 40}}, windowLightBottom);

    polygon({{x + 30, y + 50}, {x + 40, y + 50}, {x + 40, y + 60}, {x + 30, y + 60}}, windowLightTop);
    polygon({{x + 30, y + 60}, {x + 40, y + 60}, {x + 40, y + 70}, {x + 30, y + 70}}, windowLightBottom);

    polygon({{x + 30, y + 80}, {x + 40, y + 80}, {x + 40, y + 90}, {x + 30, y + 90}}, windowLightTop);
    polygon({{x + 30, y + 90}, {x + 40, y + 90}, {x + 40, y + 100}, {x + 30, y + 100}}, windowLightBottom);

    polygon({{x + 30, y + 110}, {x + 40, y + 110}, {x + 40, y + 130}, {x + 30, y + 130}}, windowDark);

    polygon({{x + 30, y + 140}, {x + 40, y + 140}, {x + 40, y + 160}, {x + 30, y + 160}}, windowDark);

    polygon({{x + 30, y + 170}, {x + 40, y + 170}, {x + 40, y + 180}, {x + 30, y + 180}}, windowLightTop);
    polygon({{x + 30, y + 180}, {x + 40, y + 180}, {x + 40, y + 190}, {x + 30, y + 190}}, windowLightBottom);

    polygon({{x + 30, y + 200}, {x + 40, y + 200}, {x + 40, y + 210}, {x + 30, y + 210}}, windowLightTop);
    polygon({{x + 30, y + 210}, {x + 40, y + 210}, {x + 40, y + 220}, {x + 30, y + 220}}, windowLightBottom);

    polygon({{x + 30, y + 230}, {x + 40, y + 230}, {x + 40, y + 240}, {x + 30, y + 240}}, windowLightTop);
    polygon({{x + 30, y + 240}, {x + 40, y + 240}, {x + 40, y + 250}, {x + 30, y + 250}}, windowLightBottom);

    /// Window Column Three
    polygon({{x + 55, y + 0}, {x + 65, y + 0}, {x + 65, y + 10}, {x + 55, y + 10}}, windowDark);

    polygon({{x + 55, y + 20}, {x + 65, y + 20}, {x + 65, y + 30}, {x + 55, y + 30}}, windowLightTop);
    polygon({{x + 55, y + 30}, {x + 65, y + 30}, {x + 65, y + 40}, {x + 55, y + 40}}, windowLightBottom);

    polygon({{x + 55, y + 50}, {x + 65, y + 50}, {x + 65, y + 70}, {x + 55, y + 70}}, windowDark);

    polygon({{x + 55, y + 80}, {x + 65, y + 80}, {x + 65, y + 90}, {x + 55, y + 90}}, windowLightTop);
    polygon({{x + 55, y + 90}, {x + 65, y + 90}, {x + 65, y + 100}, {x + 55, y + 100}}, windowLightBottom);

    polygon({{x + 55, y + 110}, {x + 65, y + 110}, {x + 65, y + 120}, {x + 55, y + 120}}, windowLightTop);
    polygon({{x + 55, y + 120}, {x + 65, y + 120}, {x + 65, y + 130}, {x + 55, y + 130}}, windowLightBottom);

    polygon({{x + 55, y + 140}, {x + 65, y + 140}, {x + 65, y + 160}, {x + 55, y + 160}}, windowDark);

    polygon({{x + 55, y + 170}, {x + 65, y + 170}, {x + 65, y + 180}, {x + 55, y + 180}}, windowLightTop);
    polygon({{x + 55, y + 180}, {x + 65, y + 180}, {x + 65, y + 190}, {x + 55, y + 190}}, windowLightBottom);

    polygon({{x + 55, y + 200}, {x + 65, y + 200}, {x + 65, y + 220}, {x + 55, y + 220}}, windowDark);

    polygon({{x + 55, y + 230}, {x + 65, y + 230}, {x + 65, y + 240}, {x + 55, y + 240}}, windowLightTop);
    polygon({{x + 55, y + 240}, {x + 65, y + 240}, {x + 65, y + 250}, {x + 55, y + 250}}, windowLightBottom);

    /// Window Column Four
    polygon({{x + 85, y + 0}, {x + 98, y + 0}, {x + 98, y + 10}, {x + 85, y + 10}}, windowDark);

    polygon({{x + 85, y + 20}, {x + 98, y + 20}, {x + 98, y + 40}, {x + 85, y + 40}}, windowDark);

    polygon({{x + 85, y + 50}, {x + 98, y + 50}, {x + 98, y + 70}, {x + 85, y + 70}}, windowDark);

    polygon({{x + 85, y + 80}, {x + 98, y + 80}, {x + 98, y + 90}, {x + 85, y + 90}}, windowLightTop);
    polygon({{x + 85, y + 90}, {x + 98, y + 90}, {x + 98, y + 100}, {x + 85, y + 100}}, windowLightBottom);

    polygon({{x + 85, y + 110}, {x + 98, y + 110}, {x + 98, y + 130}, {x + 85, y + 130}}, windowDark);

    polygon({{x + 85, y + 140}, {x + 98, y + 140}, {x + 98, y + 160}, {x + 85, y + 160}}, windowDark);

    polygon({{x + 85, y + 170}, {x + 98, y + 170}, {x + 98, y + 190}, {x + 85, y + 190}}, windowDark);

    polygon({{x + 85, y + 200}, {x + 98, y + 200}, {x + 98, y + 210}, {x + 85, y + 210}}, windowLightTop);
    polygon({{x + 85, y + 210}, {x + 98, y + 210}, {x + 98, y + 220}, {x + 85, y + 220}}, windowLightBottom);

    polygon({{x + 85, y + 230}, {x + 98, y + 230}, {x + 98, y + 245}, {x + 85, y + 245}}, windowDark);


   /// Window Column Five
    polygon({{x + 115, y + 0}, {x + 130, y + 0}, {x + 130, y + 10}, {x + 115, y + 10}}, windowDark);

    polygon({{x + 115, y + 20}, {x + 130, y + 20}, {x + 130, y + 40}, {x + 115, y + 40}}, windowDark);

    polygon({{x + 115, y + 50}, {x + 130, y + 50}, {x + 130, y + 60}, {x + 115, y + 60}}, windowLightTop);
    polygon({{x + 115, y + 60}, {x + 130, y + 60}, {x + 130, y + 70}, {x + 115, y + 70}}, windowLightBottom);

    polygon({{x + 115, y + 80}, {x + 130, y + 80}, {x + 130, y + 90}, {x + 115, y + 90}}, windowLightTop);
    polygon({{x + 115, y + 90}, {x + 130, y + 90}, {x + 130, y + 100}, {x + 115, y + 100}}, windowLightBottom);

    polygon({{x + 115, y + 110}, {x + 130, y + 110}, {x + 130, y + 120}, {x + 115, y + 120}}, windowLightTop);
    polygon({{x + 115, y + 120}, {x + 130, y + 120}, {x + 130, y + 130}, {x + 115, y + 130}}, windowLightBottom);

    polygon({{x + 115, y + 140}, {x + 130, y + 140}, {x + 130, y + 160}, {x + 115, y + 160}}, windowDark);

    polygon({{x + 115, y + 170}, {x + 130, y + 170}, {x + 130, y + 180}, {x + 115, y + 180}}, windowLightTop);
    polygon({{x + 115, y + 180}, {x + 130, y + 180}, {x + 130, y + 190}, {x + 115, y + 190}}, windowLightBottom);

    polygon({{x + 115, y + 200}, {x + 130, y + 200}, {x + 130, y + 220}, {x + 115, y + 220}}, windowDark);

    polygon({{x + 115, y + 230}, {x + 130, y + 230}, {x + 130, y + 237}, {x + 115, y + 237}}, windowLightTop);
    polygon({{x + 115, y + 237}, {x + 130, y + 237}, {x + 130, y + 245}, {x + 115, y + 245}}, windowLightBottom);

    polygon({{x + 135, y + 0}, {x + 145, y + 0}, {x + 145, y + 245}, {x + 135, y + 245}}, DarkPillar);

    /// Terrace
    polygon({{x + 0, y + 245}, {x + 75, y + 245}, {x + 75, y + 250}, {x + 0, y + 250}}, Pillar);
    polygon({{x + 75, y + 245}, {x + 145, y + 245}, {x + 145, y + 250}, {x + 75, y + 250}}, DarkPillar);


}

/// Initializing all Clouds
void drawClouds(){
    cloud3();
    cloud2();
    cloud1();
}

/// Initializing Buildings
void Building(){
    // Building One
    Building_One(0, 0);
    polygon({{0, 250}, {0, 255}, {140,255}, {140, 250}}, {0, 0, 0});
    Building_One(0, 255);


    // Building Eight
    Building_One(700, 0);

    // Building Twelve
    Building_One(1380, 0);
    polygon({{1420, 250}, {1500, 250}, {1500, 260}, {1420, 260}}, {19, 23, 69});
    polygon({{1400, 260}, {1500, 260}, {1500, 290}, {1400, 290}}, {19, 23, 69});

    // Thirteenth Building
    Building_One(1500, 0);
    polygon({{1505, 250}, {1505, 255}, {1640,255}, {1640, 250}}, {0, 0, 0});
    Building_One(1500, 255);
}

void display()
{
   /// Sky
    Sky();

    /// Stars
    if(night){
        Stars();
    }

    /// Clouds
    drawClouds();

    // Building
    Building();

    /// Plane and Carrier
    movePlane();
    moveCarrer();
    moveCarrerTwo();

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

void updates(){
    glutTimerFunc(100, updateCloud, 0);
    glutTimerFunc(100, updatePlane, 0);
    glutTimerFunc(100, updateCarrier, 0);
    glutTimerFunc(100, updateCarrierTwo, 0);
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
    updates();
    glutMainLoop();
}
