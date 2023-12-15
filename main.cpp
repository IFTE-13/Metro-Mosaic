#include <iostream>
#include <GL/freeglut.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <vector>
#include <math.h>
#include <windows.h>
#include <mmsystem.h>

using namespace std;

/// Boolean Variables
bool day = false, night = true, signBoard = false, on = true, laser = false, rain = false, rainSound = false;

/// Float Variables for Translation
float cloudTime1 = 0;
float cloudTime2 = 0;
float planePosition = 3600;
float carrierPosition = 00;
float carrierTwoPosition = 400;
float GhostPosition = 1;
float DonutePosition = 0;
float objectPositionX = 0;
float objectPositionY = 0;
float rainPosition = 0;

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

void Sun(float x, float y){
    circle(x, y, 94, 94, {255, 173, 132});
    circle(x, y, 93, 93, {255, 227, 130});
    circle(x, y, 92, 92, {255, 196, 126});
    circle(x, y, 91, 91, {255, 247, 138});
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

/// Timer Function for making the First Building's Window Color Toggle
int i, j, k= 0;
bool two, three = true;
void updateLights(int value) {

    if(on && i < 10){
        on = false;
        i += 0.5;
    }
    else{
        on = true;
        i = 0;
    }

    glutPostRedisplay();
    glutTimerFunc(2000, updateLights, 0);
}

void updateLights2(int value) {

    if(on && j < 1){
        two = false;
        j += 1;
    }
    else{
        two = true;
        j = 0;
    }

    glutPostRedisplay();
    glutTimerFunc(500, updateLights2, 0);
}

void updateLights3(int value) {

    if(on && k < 1){
        three = false;
        k += 1;
    }
    else{
        three = true;
        k = 0;
    }

    glutPostRedisplay();
    glutTimerFunc(1000, updateLights3, 0);
}

void LightOnOff(){
    if(night){
        if(on) {
            polygon({{30, 80}, {40, 80}, {40, 100}, {30, 100}}, {28, 0, 149});
            polygon({{85, 255 + 200}, {98, 255 + 200}, {98, 255 + 220}, {85, 255 + 220}}, {28, 0, 149});
            polygon({{420 + 5, 340}, {420 + 65, 340}, {420 + 65, 360}, {420 +  5, 360}}, {43, 71, 136});
            polygon({{420 + 5, 360}, {420 + 65, 360}, {420 + 65, 370}, {420 + 5, 370}}, {61, 56, 130});
            polygon({{930 + 100, 255 + 185}, {930 + 110, 255 + 185}, {930 + 110, 255 + 200}, {930 + 100, 255 + 200}}, {34, 203, 197});
            polygon({{930 + 120, 255 + 185}, {930 + 130, 255 + 185}, {930 + 130, 255 + 200}, {930 + 120, 255 + 200}}, {34, 203, 197});
            polygon({{930 + 120, 255 + 155}, {930 + 130, 255 + 155}, {930 + 130, 255 + 170}, {930 + 120, 255 + 170}}, {34, 203, 197});
        }
        if(two){
            polygon({{30, 255 + 80}, {40, 255 + 80}, {40, 255 + 100}, {30, 255 + 100}}, {28, 0, 149});
            polygon({{320 + 10, 40}, {320 + 15, 40}, {320 + 15, 46}, {320 + 10, 46}}, {34, 203, 197});
            polygon({{320 + 25, 40}, {320 + 30, 40}, {320 + 30, 46}, {320 + 25, 46}}, {34, 203, 197});
            polygon({{320 + 50, 40}, {320 + 55, 40}, {320 + 55, 46}, {320 + 50, 46}}, {34, 203, 197});
            polygon({{1720 + 87, 290}, {1720 + 98, 290}, {1720 + 98, 310}, {1720 + 87, 310}}, {28, 0, 149});
        }
        if(three){
            polygon({{1850 + 10, 40}, {1850 + 15, 40}, {1850 + 15, 46}, {1850 + 10, 46}}, {34, 203, 197});
            polygon({{1850 + 25, 40}, {1850 + 30, 40}, {1850 + 30, 46}, {1850 + 25, 46}}, {34, 203, 197});
            polygon({{520 + 10, 70}, {520 + 20, 70}, {520 + 20, 85}, {520 + 10, 85}}, {34, 203, 197});
            polygon({{520 + 10, 40}, {520 + 20, 40}, {520 + 20, 55}, {520 + 10, 55}}, {34, 203, 197});
            polygon({{1720 + 117, 320}, {1720 + 127, 320}, {1720 + 127, 340}, {1720 + 117, 340}}, {28, 0, 149});
        }
    }
}

/// Signboard's Ghost
void Ghost(float x, float y, Color Body = {218, 78, 209}){
    polygon({{x + 0, y + 0}, {x + 3, y + 0}, {x + 3, y + 15}, {x + 0, y + 15}}, Body);
    polygon({{x + 3, y + 15}, {x + 6, y + 15}, {x + 6, y + 20}, {x + 3, y + 20}}, Body);
    polygon({{x + 6, y + 20}, {x + 15, y + 20}, {x + 15, y + 23}, {x + 6, y + 23}}, Body);
    polygon({{x + 15, y + 15}, {x + 18, y + 15}, {x + 18, y + 20}, {x + 15, y + 20}}, Body);
    polygon({{x + 18, y + 0}, {x + 21, y + 0}, {x + 21, y + 15}, {x + 18, y + 15}}, Body);

    // Eyes
    polygon({{x + 6, y + 9}, {x + 9, y + 9}, {x + 9, y + 12}, {x + 6, y + 12}}, Body);
    polygon({{x + 12, y + 9}, {x + 15, y + 9}, {x + 15, y + 12}, {x + 12, y + 12}}, Body);

    // Legs
    polygon({{x + 3, y + 0}, {x + 6, y + 0}, {x + 6, y + 3}, {x + 3, y + 3}}, Body);
    polygon({{x + 6, y + 3}, {x + 9, y + 3}, {x + 9, y + 6}, {x + 6, y + 6}}, Body);
    polygon({{x + 9, y + 0}, {x + 12, y + 0}, {x + 12, y + 3}, {x + 9, y + 3}}, Body);
    polygon({{x + 12, y + 3}, {x + 15, y + 3}, {x + 15, y + 6}, {x + 12, y + 6}}, Body);
    polygon({{x + 15, y + 0}, {x + 18, y + 0}, {x + 18, y + 3}, {x + 15, y + 3}}, Body);
}

/// Timer Function for making the Ghost Translate on Y-axis
void moveGhost(){
    glPushMatrix();
    if(night) if(signBoard) Ghost(-5 + 32, 510 + 60 + GhostPosition);
    glPopMatrix();
}

void updateGhost(int value) {
    if(GhostPosition < 2){
        GhostPosition += 1;
    }
    else{
        GhostPosition = 1;
    }

    glutPostRedisplay();
    glutTimerFunc(200, updateGhost, 0);
}

/// Signboard's Donute
void SignBoard_Donute(float x, float y, Color Donute = {218, 78, 209}){
    polygon({{x + 2, y + 0}, {x + 7, y + 0}, {x + 7, y + 2}, {x + 2, y + 2}}, Donute);
    polygon({{x + 0, y + 2}, {x + 2, y + 2}, {x + 2, y + 9}, {x + 0, y + 9}}, Donute);
    polygon({{x + 7, y + 2}, {x + 9, y + 2}, {x + 7, y + 9}, {x + 9, y + 9}}, Donute);
    polygon({{x + 2, y + 9}, {x + 7, y + 9}, {x + 7, y + 11}, {x + 2, y + 11}}, Donute);
}

/// Timer Function for making the Donutes Translate on Y-axis
void updateDonute(int value) {
    if(DonutePosition > -10){
        DonutePosition -= 2;
    }
    else{
        DonutePosition = 0;
    }

    glutPostRedisplay();
    glutTimerFunc(50, updateDonute, 0);
}

/// Building One's Signboard
void Building_One_SignBoard(float x, float y, Color shadow = {19, 23, 69}, Color light = {244, 29, 27}){
    /// Polls
    polygon({{x + 5, y + 0}, {x + 10, y + 0}, {x + 10, y + 35}, {x + 5, y + 35}}, shadow);
    polygon({{x + 85, y + 0}, {x + 90, y + 0}, {x + 90, y + 35}, {x + 85, y + 35}}, shadow);

    /// Layer One
    if(night) if(signBoard) polygon({{x - 10, y + 35}, {x + 105, y + 35}, {x + 105, y + 90}, {x - 10, y + 90}}, {77, 104, 159});

    /// Layer Two
    polygon({{x - 5, y + 40}, {x + 100, y + 40}, {x + 100, y + 85}, {x - 5, y + 85}}, {103, 152, 191});

    /// Layer Three
    if(night) if(signBoard) polygon({{x - 2, y + 43}, {x + 97, y + 43}, {x + 97, y + 82}, {x - 2, y + 82}}, {79, 211, 217});

    /// Layer Four
    polygon({{x, y + 45}, {x + 95, y + 45}, {x + 95, y + 80}, {x, y + 80}}, {103, 152, 191});

    /// Layer Five
    polygon({{x + 2, y + 47}, {x + 93, y + 47}, {x + 93, y + 78}, {x + 2, y + 78}}, {19, 23, 69});
}

/// Initializing Signboard
void SignBoard(float x, float y){
    Building_One_SignBoard(x + 25, y + 10);
    if(signBoard){
        if(night){
            SignBoard_Donute(x + 60 + DonutePosition, y + 65, {218, 78, 209});
            SignBoard_Donute(x + 75 + DonutePosition, y + 67, {250, 254, 124});
            SignBoard_Donute(x + 90 + DonutePosition, y + 65, {218, 78, 209});
            SignBoard_Donute(x + 105 + DonutePosition, y + 67, {250, 254, 124});
        }
    }
}

/// Building One Network Tower
void NetworkTower(float x, float y, Color shadow = {19, 23, 69}, Color light = {244, 29, 27}){

    polygon({{x + 25, y}, {x + 30, y}, {x + 30, y + 15}, {x + 25, y + 15}}, shadow);
    if(night) polygon({{x + 24, y + 15}, {x + 31, y + 15}, {x + 31, y + 20}, {x + 24, y + 20}}, light);

    polygon({{x + 30, y}, {x + 50, y}, {x + 50, y + 10}, {x + 30, y + 10}}, shadow);

    polygon({{x + 50, y}, {x + 55, y}, {x + 55, y + 20}, {x + 50, y + 20}}, shadow);

    polygon({{x + 65, y}, {x + 70, y}, {x + 70, y + 10}, {x + 65, y + 10}}, shadow);
}

/// Building One's Terrace
void Building_One_Terrace(float x, float y){
    polygon({{x + 20, y}, {x + 75, y}, {x + 75, y + 5}, {x + 20, y + 5}}, {122, 165, 198});
    polygon({{x + 20, y + 5}, {x + 75, y + 5}, {x + 75, y + 10}, {x + 20, y + 10}}, {155, 204, 239});

    polygon({{x + 75, y}, {x + 130, y}, {x + 130, y + 5}, {x + 75, y + 5}}, {78, 105, 131});
    polygon({{x + 75, y + 5}, {x + 130, y + 5}, {x + 130, y + 10}, {x + 75, y + 10}}, {90, 125, 149});

    NetworkTower(x + 20, y + 10);
}

/// Building Two
void Building_Two(float x, float y, int m = 1){
    if(night){
        /// Base
        polygon({{x + 0, y + 0}, {x + 70, y + 0}, {x + 70, y + 10}, {x + 0, y + 10}}, {49, 33, 196});
        polygon({{x + 70, y + 0}, {x + 140, y + 0}, {x + 140, y + 10}, {x + 70, y + 10}}, {29, 1, 50});

        /// First Floor
        polygon({{x + 0, y + 10}, {x + 70, y + 10}, {x + 70, y + 20}, {x + 0, y + 20}}, {22, 37, 128});
        polygon({{x + 0, y + 20}, {x + 70, y + 20}, {x + 70, y + 30}, {x + 0, y + 30}}, {33, 25, 108});

        polygon({{x + 70, y + 10}, {x + 140, y + 10}, {x + 140, y + 20}, {x + 70, y + 20}}, {49, 0, 49});
        polygon({{x + 70, y + 20}, {x + 140, y + 20}, {x + 140, y + 30}, {x + 70, y + 30}}, {41, 0, 54});

        /// Second Floor
        polygon({{x + 0, y + 30}, {x + 70, y + 30}, {x + 70, y + 40}, {x + 0, y + 40}}, {48, 38, 193});
        polygon({{x + 70, y + 30}, {x + 140, y + 30}, {x + 140, y + 40}, {x + 70, y + 40}}, {36, 0, 51});

        polygon({{x + 0, y + 40}, {x + 70, y + 40}, {x + 70, y + 50}, {x + 0, y + 50}}, {22, 37, 128});
        polygon({{x + 0, y + 50}, {x + 70, y + 50}, {x + 70, y + 60}, {x + 0, y + 60}}, {33, 25, 108});

        polygon({{x + 70, y + 40}, {x + 140, y + 40}, {x + 140, y + 50}, {x + 70, y + 50}}, {49, 0, 49});
        polygon({{x + 70, y + 50}, {x + 140, y + 50}, {x + 140, y + 60}, {x + 70, y + 60}}, {41, 0, 54});

        /// Third Floor
        polygon({{x + 0, y + 60}, {x + 70, y + 60}, {x + 70, y + 70}, {x + 0, y + 70}}, {48, 38, 193});
        polygon({{x + 70, y + 60}, {x + 140, y + 60}, {x + 140, y + 70}, {x + 70, y + 70}}, {27, 2, 50});

        polygon({{x + 0, y + 70}, {x + 70, y + 70}, {x + 70, y + 80}, {x + 0, y + 80}}, {22, 37, 128});
        polygon({{x + 0, y + 80}, {x + 70, y + 80}, {x + 70, y + 90}, {x + 0, y + 90}}, {33, 25, 108});

        polygon({{x + 70, y + 70}, {x + 140, y + 70}, {x + 140, y + 80}, {x + 70, y + 80}}, {49, 0, 49});
        polygon({{x + 70, y + 80}, {x + 140, y + 80}, {x + 140, y + 90}, {x + 70, y + 90}}, {41, 0, 54});

        /// Middle Portion
        polygon({{x + 0, y + 90}, {x + 70, y + 90}, {x + 70, y + 100}, {x + 0, y + 100}}, {77, 50, 202});
        polygon({{x + 70, y + 90}, {x + 140, y + 90}, {x + 140, y + 100}, {x + 70, y + 100}}, {43, 17, 78});

        polygon({{x + 0, y + 100}, {x + 140, y + 100}, {x + 140, y + 110}, {x + 0, y + 110}}, {27, 2, 50});

        polygon({{x + 0, y + 110}, {x + 70, y + 110}, {x + 70, y + 125}, {x + 0, y + 125}}, {77, 50, 202});
        polygon({{x + 70, y + 110}, {x + 140, y + 110}, {x + 140, y + 125}, {x + 70, y + 125}}, {43, 17, 78});

        /// Fourth Floor
        polygon({{x + 0, y + 125}, {x + 70, y + 125}, {x + 70, y + 135}, {x + 0, y + 135}}, {29, 53, 142});
        polygon({{x + 0, y + 135}, {x + 70, y + 135}, {x + 70, y + 145}, {x + 0, y + 145}}, {40, 24, 106});

        polygon({{x + 70, y + 125}, {x + 140, y + 125}, {x + 140, y + 145}, {x + 70, y + 145}}, {33, 11, 58});

        /// Fifth Floor
        polygon({{x + 0, y + 145}, {x + 70, y + 145}, {x + 70, y + 155}, {x + 0, y + 155}}, {36, 50, 127});
        polygon({{x + 70, y + 145}, {x + 140, y + 145}, {x + 140, y + 155}, {x + 70, y + 155}}, {40, 24, 106});

        polygon({{x + 0, y + 155}, {x + 70, y + 155}, {x + 70, y + 165}, {x + 0, y + 165}}, {29, 53, 142});
        polygon({{x + 0, y + 165}, {x + 70, y + 165}, {x + 70, y + 175}, {x + 0, y + 175}}, {40, 24, 106});

        polygon({{x + 70, y + 155}, {x + 140, y + 155}, {x + 140, y + 175}, {x + 70, y + 175}}, {33, 11, 58});

        /// Sixth Floor
        polygon({{x + 0, y + 175}, {x + 70, y + 175}, {x + 70, y + 185}, {x + 0, y + 185}}, {36, 50, 127});
        polygon({{x + 70, y + 175}, {x + 140, y + 175}, {x + 140, y + 185}, {x + 70, y + 185}}, {40, 24, 106});

        polygon({{x + 0, y + 185}, {x + 70, y + 185}, {x + 70, y + 195}, {x + 0, y + 195}}, {29, 53, 142});
        polygon({{x + 0, y + 195}, {x + 70, y + 195}, {x + 70, y + 205}, {x + 0, y + 205}}, {40, 24, 106});

        polygon({{x + 70, y + 185}, {x + 140, y + 185}, {x + 140, y + 205}, {x + 70, y + 205}}, {33, 11, 58});

        /// First Floor's Windows
        polygon({{x + 10, y + 10}, {x + 20, y + 10}, {x + 20, y + 25}, {x + 10, y + 25}}, {5, 67, 157});
        polygon({{x + 30, y + 10}, {x + 40, y + 10}, {x + 40, y + 25}, {x + 30, y + 25}}, {34, 203, 197});
        polygon({{x + 50, y + 10}, {x + 60, y + 10}, {x + 60, y + 25}, {x + 50, y + 25}}, {5, 67, 157});

        polygon({{x + 80, y + 10}, {x + 90, y + 10}, {x + 90, y + 25}, {x + 80, y + 25}}, {29, 18, 90});
        polygon({{x + 100, y + 10}, {x + 110, y + 10}, {x + 110, y + 25}, {x + 100, y + 25}}, {29, 18, 90});
        polygon({{x + 120, y + 10}, {x + 130, y + 10}, {x + 130, y + 25}, {x + 120, y + 25}}, {29, 18, 90});

        /// Second Floor's Windows
        polygon({{x + 10, y + 40}, {x + 20, y + 40}, {x + 20, y + 55}, {x + 10, y + 55}}, {5, 67, 157});
        polygon({{x + 30, y + 40}, {x + 40, y + 40}, {x + 40, y + 55}, {x + 30, y + 55}}, {34, 203, 197});
        polygon({{x + 50, y + 40}, {x + 60, y + 40}, {x + 60, y + 55}, {x + 50, y + 55}}, {39, 80, 92});

        polygon({{x + 80, y + 40}, {x + 90, y + 40}, {x + 90, y + 55}, {x + 80, y + 55}}, {39, 80, 92});
        polygon({{x + 100, y + 40}, {x + 110, y + 40}, {x + 110, y + 55}, {x + 100, y + 55}}, {39, 80, 92});
        polygon({{x + 120, y + 40}, {x + 130, y + 40}, {x + 130, y + 55}, {x + 120, y + 55}}, {5, 67, 157});

        /// Third Floor's Windows
        polygon({{x + 10, y + 70}, {x + 20, y + 70}, {x + 20, y + 85}, {x + 10, y + 85}}, {5, 67, 157});
        polygon({{x + 30, y + 70}, {x + 40, y + 70}, {x + 40, y + 85}, {x + 30, y + 85}}, {34, 203, 197});
        polygon({{x + 50, y + 70}, {x + 60, y + 70}, {x + 60, y + 85}, {x + 50, y + 85}}, {5, 67, 157});

        polygon({{x + 80, y + 70}, {x + 90, y + 70}, {x + 90, y + 85}, {x + 80, y + 85}}, {34, 203, 197});
        polygon({{x + 100, y + 70}, {x + 110, y + 70}, {x + 110, y + 85}, {x + 100, y + 85}}, {25, 67, 157});
        polygon({{x + 120, y + 70}, {x + 130, y + 70}, {x + 130, y + 85}, {x + 120, y + 85}}, {5, 67, 157});

        /// Fourth Floor's Windows
        polygon({{x + 10, y + 125}, {x + 20, y + 125}, {x + 20, y + 140}, {x + 10, y + 140}}, {5, 67, 157});
        polygon({{x + 30, y + 125}, {x + 40, y + 125}, {x + 40, y + 140}, {x + 30, y + 140}}, {34, 203, 197});
        polygon({{x + 50, y + 125}, {x + 60, y + 125}, {x + 60, y + 140}, {x + 50, y + 140}}, {5, 67, 157});

        polygon({{x + 80, y + 125}, {x + 90, y + 125}, {x + 90, y + 140}, {x + 80, y + 140}}, {5, 67, 157});
        polygon({{x + 100, y + 125}, {x + 110, y + 125}, {x + 110, y + 140}, {x + 100, y + 140}}, {5, 67, 157});
        polygon({{x + 120, y + 125}, {x + 130, y + 125}, {x + 130, y + 140}, {x + 120, y + 140}}, {5, 67, 157});

        /// Fifth Floor's Windows
        polygon({{x + 10, y + 155}, {x + 20, y + 155}, {x + 20, y + 170}, {x + 10, y + 170}}, {34, 203, 197});
        polygon({{x + 30, y + 155}, {x + 40, y + 155}, {x + 40, y + 170}, {x + 30, y + 170}}, {34, 203, 197});
        polygon({{x + 50, y + 155}, {x + 60, y + 155}, {x + 60, y + 170}, {x + 50, y + 170}}, {51, 111, 188});

        polygon({{x + 80, y + 155}, {x + 90, y + 155}, {x + 90, y + 170}, {x + 80, y + 170}}, {5, 67, 157});
        polygon({{x + 100, y + 155}, {x + 110, y + 155}, {x + 110, y + 170}, {x + 100, y + 170}}, {5, 67, 157});
        polygon({{x + 120, y + 155}, {x + 130, y + 155}, {x + 130, y + 170}, {x + 120, y + 170}}, {5, 67, 157});

        /// Sixth Floor's Windows
        polygon({{x + 10, y + 185}, {x + 20, y + 185}, {x + 20, y + 200}, {x + 10, y + 200}}, {34, 203, 197});
        polygon({{x + 30, y + 185}, {x + 40, y + 185}, {x + 40, y + 200}, {x + 30, y + 200}}, {34, 203, 197});
        polygon({{x + 50, y + 185}, {x + 60, y + 185}, {x + 60, y + 200}, {x + 50, y + 200}}, {51, 111, 188});

        polygon({{x + 80, y + 185}, {x + 90, y + 185}, {x + 90, y + 200}, {x + 80, y + 200}}, {51, 111, 188});
        polygon({{x + 100, y + 185}, {x + 110, y + 185}, {x + 110, y + 200}, {x + 100, y + 200}}, {51, 111, 188});
        polygon({{x + 120, y + 185}, {x + 130, y + 185}, {x + 130, y + 200}, {x + 120, y + 200}}, {51, 111, 188});

        /// Terrace
        polygon({{x + 0, y + 205}, {x + 70, y + 205}, {x + 70, y + 220}, {x + 0, y + 220}}, {123, 82, 198});
        polygon({{x + 70, y + 205}, {x + 140, y + 205}, {x + 140, y + 220}, {x + 70, y + 220}}, {16, 40, 59});
    }
    else if(day){
        Color WhiteOne = {245, 204, 160};
        Color WhiteTwo = {153, 77, 28};
        Color GrayOne = {228, 143, 69};
        Color GrayTwo = {107, 36, 12};
        Color GrayThree = {107, 36, 12};
        Color WindowTypeOne = {251, 139, 36};
        Color WindowTypeTwo = {227, 100, 20};
        /// Base
        polygon({{x + 0, y + 0}, {x + 70, y + 0}, {x + 70, y + 10}, {x + 0, y + 10}}, WhiteOne);
        polygon({{x + 70, y + 0}, {x + 140, y + 0}, {x + 140, y + 10}, {x + 70, y + 10}}, GrayOne);

        /// First Floor
        polygon({{x + 0, y + 10}, {x + 70, y + 10}, {x + 70, y + 20}, {x + 0, y + 20}}, WhiteTwo);
        polygon({{x + 0, y + 20}, {x + 70, y + 20}, {x + 70, y + 30}, {x + 0, y + 30}}, GrayTwo);

        polygon({{x + 70, y + 10}, {x + 140, y + 10}, {x + 140, y + 30}, {x + 70, y + 30}}, GrayThree);

        /// Second Floor
        polygon({{x + 0, y + 30}, {x + 70, y + 30}, {x + 70, y + 40}, {x + 0, y + 40}}, WhiteOne);
        polygon({{x + 70, y + 30}, {x + 140, y + 30}, {x + 140, y + 40}, {x + 70, y + 40}}, GrayOne);

        polygon({{x + 0, y + 40}, {x + 70, y + 40}, {x + 70, y + 50}, {x + 0, y + 50}}, WhiteTwo);
        polygon({{x + 0, y + 50}, {x + 70, y + 50}, {x + 70, y + 60}, {x + 0, y + 60}}, GrayTwo);

        polygon({{x + 70, y + 40}, {x + 140, y + 40}, {x + 140, y + 60}, {x + 70, y + 60}}, GrayThree);

        /// Third Floor
        polygon({{x + 0, y + 60}, {x + 70, y + 60}, {x + 70, y + 70}, {x + 0, y + 70}}, WhiteOne);
        polygon({{x + 70, y + 60}, {x + 140, y + 60}, {x + 140, y + 70}, {x + 70, y + 70}}, GrayOne);

        polygon({{x + 0, y + 70}, {x + 70, y + 70}, {x + 70, y + 80}, {x + 0, y + 80}}, WhiteTwo);
        polygon({{x + 0, y + 80}, {x + 70, y + 80}, {x + 70, y + 90}, {x + 0, y + 90}}, GrayTwo);

        polygon({{x + 70, y + 70}, {x + 140, y + 70}, {x + 140, y + 90}, {x + 70, y + 90}}, GrayThree);

        /// Middle Portion
        polygon({{x + 0, y + 90}, {x + 70, y + 90}, {x + 70, y + 100}, {x + 0, y + 100}}, WhiteOne);
        polygon({{x + 70, y + 90}, {x + 140, y + 90}, {x + 140, y + 100}, {x + 70, y + 100}}, GrayOne);

        polygon({{x + 0, y + 100}, {x + 140, y + 100}, {x + 140, y + 110}, {x + 0, y + 110}}, {27, 2, 50});

        polygon({{x + 0, y + 110}, {x + 70, y + 110}, {x + 70, y + 125}, {x + 0, y + 125}}, WhiteOne);
        polygon({{x + 70, y + 110}, {x + 140, y + 110}, {x + 140, y + 125}, {x + 70, y + 125}}, GrayOne);

        /// Fourth Floor
        polygon({{x + 0, y + 125}, {x + 70, y + 125}, {x + 70, y + 135}, {x + 0, y + 135}}, WhiteTwo);
        polygon({{x + 0, y + 135}, {x + 70, y + 135}, {x + 70, y + 145}, {x + 0, y + 145}}, GrayTwo);

        polygon({{x + 70, y + 125}, {x + 140, y + 125}, {x + 140, y + 145}, {x + 70, y + 145}}, GrayThree);

        /// Fifth Floor
        polygon({{x + 0, y + 145}, {x + 70, y + 145}, {x + 70, y + 155}, {x + 0, y + 155}}, WhiteOne);
        polygon({{x + 70, y + 145}, {x + 140, y + 145}, {x + 140, y + 155}, {x + 70, y + 155}}, GrayOne);

        polygon({{x + 0, y + 155}, {x + 70, y + 155}, {x + 70, y + 165}, {x + 0, y + 165}}, WhiteTwo);
        polygon({{x + 0, y + 165}, {x + 70, y + 165}, {x + 70, y + 175}, {x + 0, y + 175}}, GrayTwo);

        polygon({{x + 70, y + 155}, {x + 140, y + 155}, {x + 140, y + 175}, {x + 70, y + 175}}, GrayThree);

        /// Sixth Floor
        polygon({{x + 0, y + 175}, {x + 70, y + 175}, {x + 70, y + 185}, {x + 0, y + 185}}, WhiteOne);
        polygon({{x + 70, y + 175}, {x + 140, y + 175}, {x + 140, y + 185}, {x + 70, y + 185}}, GrayOne);

        polygon({{x + 0, y + 185}, {x + 70, y + 185}, {x + 70, y + 195}, {x + 0, y + 195}}, WhiteTwo);
        polygon({{x + 0, y + 195}, {x + 70, y + 195}, {x + 70, y + 205}, {x + 0, y + 205}}, GrayTwo);

        polygon({{x + 70, y + 185}, {x + 140, y + 185}, {x + 140, y + 205}, {x + 70, y + 205}}, GrayThree);

        /// First Floor's Windows
        polygon({{x + 10, y + 10}, {x + 20, y + 10}, {x + 20, y + 25}, {x + 10, y + 25}}, WindowTypeTwo);
        polygon({{x + 30, y + 10}, {x + 40, y + 10}, {x + 40, y + 25}, {x + 30, y + 25}}, WindowTypeOne);
        polygon({{x + 50, y + 10}, {x + 60, y + 10}, {x + 60, y + 25}, {x + 50, y + 25}}, WindowTypeTwo);

        polygon({{x + 80, y + 10}, {x + 90, y + 10}, {x + 90, y + 25}, {x + 80, y + 25}}, WindowTypeTwo);
        polygon({{x + 100, y + 10}, {x + 110, y + 10}, {x + 110, y + 25}, {x + 100, y + 25}}, WindowTypeTwo);
        polygon({{x + 120, y + 10}, {x + 130, y + 10}, {x + 130, y + 25}, {x + 120, y + 25}}, WindowTypeTwo);

        /// Second Floor's Windows
        polygon({{x + 10, y + 40}, {x + 20, y + 40}, {x + 20, y + 55}, {x + 10, y + 55}}, WindowTypeTwo);
        polygon({{x + 30, y + 40}, {x + 40, y + 40}, {x + 40, y + 55}, {x + 30, y + 55}}, WindowTypeOne);
        polygon({{x + 50, y + 40}, {x + 60, y + 40}, {x + 60, y + 55}, {x + 50, y + 55}}, WindowTypeTwo);

        polygon({{x + 80, y + 40}, {x + 90, y + 40}, {x + 90, y + 55}, {x + 80, y + 55}}, WindowTypeTwo);
        polygon({{x + 100, y + 40}, {x + 110, y + 40}, {x + 110, y + 55}, {x + 100, y + 55}}, WindowTypeTwo);
        polygon({{x + 120, y + 40}, {x + 130, y + 40}, {x + 130, y + 55}, {x + 120, y + 55}}, WindowTypeTwo);

        /// Third Floor's Windows
        polygon({{x + 10, y + 70}, {x + 20, y + 70}, {x + 20, y + 85}, {x + 10, y + 85}}, WindowTypeTwo);
        polygon({{x + 30, y + 70}, {x + 40, y + 70}, {x + 40, y + 85}, {x + 30, y + 85}}, WindowTypeOne);
        polygon({{x + 50, y + 70}, {x + 60, y + 70}, {x + 60, y + 85}, {x + 50, y + 85}}, WindowTypeTwo);

        polygon({{x + 80, y + 70}, {x + 90, y + 70}, {x + 90, y + 85}, {x + 80, y + 85}}, WindowTypeTwo);
        polygon({{x + 100, y + 70}, {x + 110, y + 70}, {x + 110, y + 85}, {x + 100, y + 85}}, WindowTypeTwo);
        polygon({{x + 120, y + 70}, {x + 130, y + 70}, {x + 130, y + 85}, {x + 120, y + 85}}, WindowTypeTwo);

        /// Fourth Floor's Windows
        polygon({{x + 10, y + 125}, {x + 20, y + 125}, {x + 20, y + 140}, {x + 10, y + 140}}, WindowTypeTwo);
        polygon({{x + 30, y + 125}, {x + 40, y + 125}, {x + 40, y + 140}, {x + 30, y + 140}}, WindowTypeOne);
        polygon({{x + 50, y + 125}, {x + 60, y + 125}, {x + 60, y + 140}, {x + 50, y + 140}}, WindowTypeTwo);

        polygon({{x + 80, y + 125}, {x + 90, y + 125}, {x + 90, y + 140}, {x + 80, y + 140}}, WindowTypeTwo);
        polygon({{x + 100, y + 125}, {x + 110, y + 125}, {x + 110, y + 140}, {x + 100, y + 140}}, WindowTypeTwo);
        polygon({{x + 120, y + 125}, {x + 130, y + 125}, {x + 130, y + 140}, {x + 120, y + 140}}, WindowTypeTwo);

        /// Fifth Floor's Windows
        polygon({{x + 10, y + 155}, {x + 20, y + 155}, {x + 20, y + 170}, {x + 10, y + 170}}, WindowTypeOne);
        polygon({{x + 30, y + 155}, {x + 40, y + 155}, {x + 40, y + 170}, {x + 30, y + 170}}, WindowTypeOne);
        polygon({{x + 50, y + 155}, {x + 60, y + 155}, {x + 60, y + 170}, {x + 50, y + 170}}, WindowTypeTwo);

        polygon({{x + 80, y + 155}, {x + 90, y + 155}, {x + 90, y + 170}, {x + 80, y + 170}}, WindowTypeTwo);
        polygon({{x + 100, y + 155}, {x + 110, y + 155}, {x + 110, y + 170}, {x + 100, y + 170}}, WindowTypeTwo);
        polygon({{x + 120, y + 155}, {x + 130, y + 155}, {x + 130, y + 170}, {x + 120, y + 170}}, WindowTypeTwo);

        /// Sixth Floor's Windows
        polygon({{x + 10, y + 185}, {x + 20, y + 185}, {x + 20, y + 200}, {x + 10, y + 200}}, WindowTypeOne);
        polygon({{x + 30, y + 185}, {x + 40, y + 185}, {x + 40, y + 200}, {x + 30, y + 200}}, WindowTypeOne);
        polygon({{x + 50, y + 185}, {x + 60, y + 185}, {x + 60, y + 200}, {x + 50, y + 200}}, WindowTypeTwo);

        polygon({{x + 80, y + 185}, {x + 90, y + 185}, {x + 90, y + 200}, {x + 80, y + 200}}, WindowTypeTwo);
        polygon({{x + 100, y + 185}, {x + 110, y + 185}, {x + 110, y + 200}, {x + 100, y + 200}}, WindowTypeTwo);
        polygon({{x + 120, y + 185}, {x + 130, y + 185}, {x + 130, y + 200}, {x + 120, y + 200}}, WindowTypeTwo);

        /// Terrace
        polygon({{x + 0, y + 205}, {x + 70, y + 205}, {x + 70, y + 220}, {x + 0, y + 220}}, WhiteOne);
        polygon({{x + 70, y + 205}, {x + 140, y + 205}, {x + 140, y + 220}, {x + 70, y + 220}}, GrayOne);
    }
}

/// Other Building's Network Tower
void NetworkTowerTwo(float x, float y, Color shadow = {19, 23, 69}, Color light = {244, 29, 27}){

    polygon({{x + 25, y}, {x + 100, y}, {x + 100, y + 10}, {x + 25, y + 10}}, shadow);
    polygon({{x + 45, y}, {x + 80, y}, {x + 80, y + 25}, {x + 45, y + 25}}, shadow);
    polygon({{x + 95, y}, {x + 100, y}, {x + 100, y + 25}, {x + 95, y + 25}}, shadow);
    polygon({{x + 75, y}, {x + 80, y}, {x + 80, y + 40}, {x + 75, y + 40}}, shadow);

    if(night) polygon({{x + 74, y + 40}, {x + 81, y + 40}, {x + 81, y + 47}, {x + 74, y + 47}}, light);
}

void Tomb(float x, float y, int m = 1, Color buildingWall = {132, 82, 211}, Color buildingWall2 = {33, 43, 80}, Color shadow = {56, 99, 164}, Color shadow2 = {16, 41, 83} , Color LeftWindow = {40, 82, 149}, Color RightWIndow = {9, 36, 78}){
    if(night){
        buildingWall = {132, 82, 211};
        buildingWall2 = {33, 43, 80};
        shadow = {56, 99, 164};
        shadow2 = {16, 41, 83};
        LeftWindow = {40, 82, 149};
        RightWIndow = {9, 36, 78};
    }
    else if(day){
        buildingWall = {245, 204, 160};
        buildingWall2 = {228, 143, 69};
        shadow = {153, 77, 28};
        shadow2 = {107, 36, 12};
        LeftWindow = {251, 139, 36};
        RightWIndow = {227, 100, 20};
    }
    polygon({{x + m * 0, y + 0}, {x + m * 40, y + 0}, {x + m * 40, y + 8}, {x + m * 0, y + 8}}, buildingWall);
    polygon({{x + m * 40, y + 0}, {x + m * 80, y + 0}, {x + m * 80, y + 8}, {x + m * 40, y + 8}}, buildingWall2);

    polygon({{x + m * 5, y + 8}, {x + m * 40, y + 8}, {x + m * 40, y + 16}, {x + m * 5, y + 16}}, shadow);
    polygon({{x + m * 40, y + 8}, {x + m * 75, y + 8}, {x + m * 75, y + 16}, {x + m * 40, y + 16}}, shadow2);

    /// First Floor Left Window
    polygon({{x + m * 10, y + 8}, {x + m * 15, y + 8}, {x + m * 15, y + 14}, {x + m * 10, y + 14}}, LeftWindow);
    polygon({{x + m * 25, y + 8}, {x + m * 30, y + 8}, {x + m * 30, y + 14}, {x + m * 25, y + 14}}, LeftWindow);

    /// First Floor Right Window
    polygon({{x + m * 50, y + 8}, {x + m * 55, y + 8}, {x + m * 55, y + 14}, {x + m * 50, y + 14}}, RightWIndow);
    polygon({{x + m * 65, y + 8}, {x + m * 70, y + 8}, {x + m * 70, y + 14}, {x + m * 65, y + 14}}, RightWIndow);

    polygon({{x + m * 0, y + 16}, {x + m * 40, y + 16}, {x + m * 40, y + 24}, {x + m * 0, y + 24}}, buildingWall);
    polygon({{x + m * 40, y + 16}, {x + m * 80, y + 16}, {x + m * 80, y + 24}, {x + m * 40, y + 24}}, buildingWall2);

    polygon({{x + m * 5, y + 24}, {x + m * 40, y + 24}, {x + m * 40, y + 32}, {x + m * 5, y + 32}}, shadow);
    polygon({{x + m * 40, y + 24}, {x + m * 75, y + 24}, {x + m * 75, y + 32}, {x + m * 40, y + 32}}, shadow2);

    /// Second Floor Left Window
    polygon({{x + m * 10, y + 24}, {x + m * 15, y + 24}, {x + m * 15, y + 30}, {x + m * 10, y + 30}}, LeftWindow);
    polygon({{x + m * 25, y + 24}, {x + m * 30, y + 24}, {x + m * 30, y + 30}, {x + m * 25, y + 30}}, LeftWindow);

    /// Third Floor Right Window
    polygon({{x + m * 50, y + 24}, {x + m * 55, y + 24}, {x + m * 55, y + 30}, {x + m * 50, y + 30}}, RightWIndow);
    polygon({{x + m * 65, y + 24}, {x + m * 70, y + 24}, {x + m * 70, y + 30}, {x + m * 65, y + 30}}, RightWIndow);

    polygon({{x + m * 0, y + 32}, {x + m * 40, y + 32}, {x + m * 40, y + 40}, {x + m * 0, y + 40}}, buildingWall);
    polygon({{x + m * 40, y + 32}, {x + m * 80, y + 32}, {x + m * 80, y + 40}, {x + m * 40, y + 40}}, buildingWall2);

    polygon({{x + m * 5, y + 40}, {x + m * 40, y + 40}, {x + m * 40, y + 48}, {x + m * 5, y + 48}}, shadow);
    polygon({{x + m * 40, y + 40}, {x + m * 75, y + 40}, {x + m * 75, y + 48}, {x + m * 40, y + 48}}, shadow2);

    /// Third Floor Left Window
    polygon({{x + m * 10, y + 40}, {x + m * 15, y + 40}, {x + m * 15, y + 46}, {x + m * 10, y + 46}}, LeftWindow);
    polygon({{x + m * 25, y + 40}, {x + m * 30, y + 40}, {x + m * 30, y + 46}, {x + m * 25, y + 46}}, LeftWindow);

    /// Third Floor Right Window
    polygon({{x + m * 50, y + 40}, {x + m * 55, y + 40}, {x + m * 55, y + 46}, {x + m * 50, y + 46}}, RightWIndow);
    polygon({{x + m * 65, y + 40}, {x + m * 70, y + 40}, {x + m * 70, y + 46}, {x + m * 65, y + 46}}, RightWIndow);

    polygon({{x + m * 0, y + 48}, {x + m * 40, y + 48}, {x + m * 40, y + 56}, {x + m * 0, y + 56}}, buildingWall);
    polygon({{x + m * 40, y + 48}, {x + m * 80, y + 48}, {x + m * 80, y + 56}, {x + m * 40, y + 56}}, buildingWall2);

    polygon({{x + m * 5, y + 56}, {x + m * 40, y + 56}, {x + m * 40, y + 64}, {x + m * 5, y + 64}}, shadow);
    polygon({{x + m * 40, y + 56}, {x + m * 75, y + 56}, {x + m * 75, y + 64}, {x + m * 40, y + 64}}, shadow2);

    /// Fourth Floor Left Window
    polygon({{x + m * 10, y + 56}, {x + m * 15, y + 56}, {x + m * 15, y + 62}, {x + m * 10, y + 62}}, LeftWindow);
    polygon({{x + m * 25, y + 56}, {x + m * 30, y + 56}, {x + m * 30, y + 62}, {x + m * 25, y + 62}}, LeftWindow);

    /// Fourth Floor Right Window
    polygon({{x + m * 50, y + 56}, {x + m * 55, y + 56}, {x + m * 55, y + 62}, {x + m * 50, y + 62}}, RightWIndow);
    polygon({{x + m * 65, y + 56}, {x + m * 70, y + 56}, {x + m * 70, y + 62}, {x + m * 65, y + 62}}, RightWIndow);

    polygon({{x + m * 0, y + 64}, {x + m * 40, y + 64}, {x + m * 40, y + 72}, {x + m * 0, y + 72}}, buildingWall);
    polygon({{x + m * 40, y + 64}, {x + m * 80, y + 64}, {x + m * 80, y + 72}, {x + m * 40, y + 72}}, buildingWall2);

    polygon({{x + m * 5, y + 72}, {x + m * 40, y + 72}, {x + m * 40, y + 80}, {x + m * 5, y + 80}}, shadow);
    polygon({{x + m * 40, y + 72}, {x + m * 75, y + 72}, {x + m * 75, y + 80}, {x + m * 40, y + 80}}, shadow2);

    /// Top Layer
    polygon({{x + m * 10, y + 80}, {x + m * 40, y + 80}, {x + m * 40, y + 85}, {x + m * 10, y + 85}}, {155, 204, 239});
    polygon({{x + m * 40, y + 80}, {x + m * 75, y + 80}, {x + m * 75, y + 85}, {x + m * 40, y + 85}}, {122, 165, 198});
}

void Third_Building(float x, float y, int m = 1, Color windowLightTop = {34, 203, 197}, Color windowLightBottom = {41, 94, 135}, Color windowDark = {28, 0, 149}, Color left = {135, 82, 214}, Color right = {26, 46, 74}, Color rightShadow = {6, 23, 52}, Color leftShadow = {33, 72, 136}){

    Building_One(200, 0);
    Building_One(1720, 0);

    if(night){
        windowLightTop = {34, 203, 197};
        windowLightBottom = {41, 94, 135};
        windowDark = {28, 0, 149};
        left = {135, 82, 214};
        right = {26, 46, 74};
        rightShadow = {6, 23, 52};
        leftShadow = {33, 72, 136};
    } else if(day){
        windowDark = {227, 100, 20};
        windowLightTop = {251, 139, 36};
        windowLightBottom = {145, 15, 64};
        left = {245, 204, 160};
        right = {228, 143, 69};
        rightShadow = {107, 36, 12};
        leftShadow = {153, 77, 28};
    }

    /// Terrace Layer
    polygon({{x + m * 10, y + 250}, {x + m * 75, y + 250}, {x + m * 75, y + 255}, {x + m * 10, y + 255}}, {122, 165, 198});
    polygon({{x + m * 10, y + 255}, {x + m * 75, y + 255}, {x + m * 75, y + 260}, {x + m * 10, y + 260}}, {155, 204, 239});

    polygon({{x + m * 75, y + 250}, {x + m * 140, y + 250}, {x + m * 140, y + 255}, {x + m * 75, y + 255}}, {78, 105, 131});
    polygon({{x + m * 75, y + 255}, {x + m * 140, y + 255}, {x + m * 140, y + 260}, {x + m * 75, y + 260}}, {90, 125, 149});

    polygon({{x + m * 15, y + 260}, {x + m * 80, y + 260}, {x + m * 80, y + 265}, {x + m * 15, y + 265}}, {122, 165, 198});
    polygon({{x + m * 15, y + 265}, {x + m * 80, y + 265}, {x + m * 80, y + 270}, {x + m * 15, y + 270}}, {155, 204, 239});

    polygon({{x + m * 80, y + 260}, {x + m * 135, y + 260}, {x + m * 135, y + 265}, {x + m * 80, y + 265}}, {78, 105, 131});
    polygon({{x + m * 80, y + 265}, {x + m * 135, y + 265}, {x + m * 135, y + 270}, {x + m * 80, y + 270}}, {90, 125, 149});

    polygon({{x + m * 20, y + 270}, {x + m * 130, y + 270}, {x + m * 130, y + 275}, {x + m * 20, y + 275}}, {0, 0, 0});

    /// Top Portion
    /// Left Section
    polygon({{x + m * 15, y + 275}, {x + m * 75, y + 275}, {x + m * 75, y + 400}, {x + m * 15, y + 400}}, left);


    polygon({{x + m * 50, y + 280}, {x + m * 65, y + 280}, {x + m * 65, y + 395}, {x + m * 50, y + 395}}, leftShadow);
    polygon({{x + m * 15, y + 280}, {x + m * 40, y + 280}, {x + m * 40, y + 395}, {x + m * 15, y + 395}}, leftShadow);

    /// Window Column One
    polygon({{x + m * 20, y + 290}, {x + m * 40, y + 290}, {x + m * 40, y + 300}, {x + m * 20, y + 300}}, windowLightBottom);
    polygon({{x + m * 20, y + 300}, {x + m * 40, y + 300}, {x + m * 40, y + 310}, {x + m * 20, y + 310}}, windowLightTop);

    polygon({{x + m * 20, y + 320}, {x + m * 40, y + 320}, {x + m * 40, y + 330}, {x + m * 20, y + 330}}, windowLightBottom);
    polygon({{x + m * 20, y + 330}, {x + m * 40, y + 330}, {x + m * 40, y + 340}, {x + m * 20, y + 340}}, windowLightTop);

    polygon({{x + m * 20, y + 350}, {x + m * 40, y + 350}, {x + m * 40, y + 360}, {x + m * 20, y + 360}}, windowLightBottom);
    polygon({{x + m * 20, y + 360}, {x + m * 40, y + 360}, {x + m * 40, y + 370}, {x + m * 20, y + 370}}, windowLightTop);

    polygon({{x + m * 20, y + 380}, {x + m * 40, y + 380}, {x + m * 40, y + 390}, {x + m * 20, y + 390}}, windowLightTop);



    /// Window Column Two
    polygon({{x + m * 55, y + 290}, {x + m * 65, y + 290}, {x + m * 65, y + 300}, {x + m * 55, y + 300}}, windowLightBottom);
    polygon({{x + m * 55, y + 300}, {x + m * 65, y + 300}, {x + m * 65, y + 310}, {x + m * 55, y + 310}}, windowLightTop);

    polygon({{x + m * 55, y + 320}, {x + m * 65, y + 320}, {x + m * 65, y + 340}, {x + m * 55, y + 340}}, windowDark);

    polygon({{x + m * 55, y + 350}, {x + m * 65, y + 350}, {x + m * 65, y + 370}, {x + m * 55, y + 370}}, windowDark);

    polygon({{x + m * 55, y + 380}, {x + m * 65, y + 380}, {x + m * 65, y + 390}, {x + m * 55, y + 390}}, windowDark);

    /// Right Section
    /// Pillar One
    polygon({{x + m * 75, y + 275}, {x + m * 135, y + 275}, {x + m * 135, y + 400}, {x + m * 75, y + 400}}, right);

    /// Shadowed Portion One
    polygon({{x + m * 85, y + 280}, {x + m * 105, y + 280}, {x + m * 105, y + 395}, {x + m * 85, y + 395}}, rightShadow);

    /// Window Column Three
    polygon({{x + m * 87, y + 290}, {x + m * 98, y + 290}, {x + m * 98, y + 300}, {x + m * 87, y + 300}}, windowLightBottom);
    polygon({{x + m * 87, y + 300}, {x + m * 98, y + 300}, {x + m * 98, y + 310}, {x + m * 87, y + 310}}, windowLightTop);

    polygon({{x + m * 87, y + 320}, {x + m * 98, y + 320}, {x + m * 98, y + 330}, {x + m * 87, y + 330}}, windowLightBottom);
    polygon({{x + m * 87, y + 330}, {x + m * 98, y + 330}, {x + m * 98, y + 340}, {x + m * 87, y + 340}}, windowLightTop);

    polygon({{x + m * 87, y + 350}, {x + m * 98, y + 350}, {x + m * 98, y + 370}, {x + m * 87, y + 370}}, windowDark);

    polygon({{x + m * 87, y + 380}, {x + m * 98, y + 380}, {x + m * 98, y + 390}, {x + m * 87, y + 390}}, windowLightBottom);

    /// Shadowed Portion Two
    polygon({{x + m * 115, y + 280}, {x + m * 135, y + 280}, {x + m * 135, y + 395}, {x + m * 115, y + 395}}, rightShadow);

    /// Window Column Four
    polygon({{x + m * 117, y + 290}, {x + m * 127, y + 290}, {x + m * 127, y + 310}, {x + m * 117, y + 310}}, windowDark);

    polygon({{x + m * 117, y + 320}, {x + m * 127, y + 320}, {x + m * 127, y + 330}, {x + m * 117, y + 330}}, windowLightBottom);
    polygon({{x + m * 117, y + 330}, {x + m * 127, y + 330}, {x + m * 127, y + 340}, {x + m * 117, y + 340}}, windowLightTop);

    polygon({{x + m * 117, y + 350}, {x + m * 127, y + 350}, {x + m * 127, y + 370}, {x + m * 117, y + 370}}, windowDark);

    polygon({{x + m * 117, y + 380}, {x + m * 127, y + 380}, {x + m * 127, y + 390}, {x + m * 117, y + 390}}, windowDark);

    /// Top Portion's Terrace Layer
    polygon({{x + m * 20, y + 400}, {x + m * 75, y + 400}, {x + m * 75, y + 405}, {x + m * 20, y + 405}}, {122, 165, 198});
    polygon({{x + m * 20, y + 405}, {x + m * 75, y + 405}, {x + m * 75, y + 410}, {x + m * 20, y + 410}}, {155, 204, 239});

    polygon({{x + m * 75, y + 400}, {x + m * 130, y + 400}, {x + m * 130, y + 405}, {x + m * 75, y + 405}}, {78, 105, 131});
    polygon({{x + m * 75, y + 405}, {x + m * 130, y + 405}, {x + m * 130, y + 410}, {x + m * 75, y + 410}}, {90, 125, 149});

    polygon({{x + m * 25, y + 410}, {x + m * 80, y + 410}, {x + m * 80, y + 415}, {x + m * 25, y + 415}}, {122, 165, 198});
    polygon({{x + m * 25, y + 415}, {x + m * 80, y + 415}, {x + m * 80, y + 420}, {x + m * 25, y + 420}}, {155, 204, 239});

    polygon({{x + m * 80, y + 410}, {x + m * 125, y + 410}, {x + m * 125, y + 415}, {x + m * 80, y + 415}}, {78, 105, 131});
    polygon({{x + m * 80, y + 415}, {x + m * 125, y + 415}, {x + m * 125, y + 420}, {x + m * 80, y + 420}}, {90, 125, 149});

    Tomb(1755, 420);
    Tomb(235, 420);
}

void NetworkTowerThree(float x, float y, Color shadow = {19, 23, 69}, Color light = {244, 29, 27}){
    polygon({{x + 25, y}, {x + 30, y}, {x + 30, y + 15}, {x + 25, y + 15}}, shadow);

    polygon({{x + 35, y}, {x + 50, y}, {x + 50, y + 10}, {x + 35, y + 10}}, shadow);

    polygon({{x + 50, y}, {x + 55, y}, {x + 55, y + 25}, {x + 50, y + 25}}, shadow);

    if(night) polygon({{x + 49, y + 25}, {x + 56, y + 25}, {x + 56, y + 30}, {x + 49, y + 30}}, light);

    polygon({{x + 65, y}, {x + 70, y}, {x + 70, y + 40}, {x + 65, y + 40}}, shadow);

    if(night) polygon({{x + 64, y + 40}, {x + 71, y + 40}, {x + 71, y + 45}, {x + 64, y + 45}}, light);
}

void Fifth_Building(float x, float y, int m = 1, Color buildingWall = {130, 86, 199}, Color buildingWall2 = {32, 35, 76}){
    if(night){
        buildingWall = {130, 86, 199};
        buildingWall2 = {32, 35, 76};
    }
    else if(day){
        buildingWall = {228, 143, 69};
        buildingWall2 = {153, 77, 28};
    }
    polygon({{x + m * 0, y + 0}, {x + m * 70, y + 0}, {x + m * 70, y + 495}, {x + m * 0, y + 495}}, buildingWall);
    polygon({{x + m * 70, y + 0}, {x + m * 130, y + 0}, {x + m * 130, y + 495}, {x + m * 70, y + 495}}, buildingWall2);

    if(night){
    /// Bottom Window
    polygon({{x + m * 5, y + 0}, {x + m * 65, y + 0}, {x + m * 65, y + 10}, {x + m * 5, y + 10}}, {46, 21, 87});

    /// Second Window
    polygon({{x + m * 5, y + 20}, {x + m * 65, y + 20}, {x + m * 65, y + 40}, {x + m * 5, y + 40}}, {38, 46, 127});
    polygon({{x + m * 5, y + 40}, {x + m * 65, y + 40}, {x + m * 65, y + 50}, {x + m * 5, y + 50}}, {47, 25, 89});

    /// Third Window
    polygon({{x + m * 5, y + 60}, {x + m * 65, y + 60}, {x + m * 65, y + 80}, {x + m * 5, y + 80}}, {41, 221, 132});
    polygon({{x + m * 5, y + 80}, {x + m * 65, y + 80}, {x + m * 65, y + 90}, {x + m * 5, y + 90}}, {32, 130, 81});

    /// Forth Window
    polygon({{x + m * 5, y + 100}, {x + m * 65, y + 100}, {x + m * 65, y + 120}, {x + m * 5, y + 120}}, {43, 71, 136});
    polygon({{x + m * 5, y + 120}, {x + m * 65, y + 120}, {x + m * 65, y + 130}, {x + m * 5, y + 130}}, {61, 56, 130});

    /// Fifth Window
    polygon({{x + m * 5, y + 140}, {x + m * 65, y + 140}, {x + m * 65, y + 160}, {x + m * 5, y + 160}}, {43, 221, 207});
    polygon({{x + m * 5, y + 160}, {x + m * 65, y + 160}, {x + m * 65, y + 170}, {x + m * 5, y + 170}}, {34, 165, 146});

    /// Sixth Window
    polygon({{x + m * 5, y + 180}, {x + m * 65, y + 180}, {x + m * 65, y + 200}, {x + m * 5, y + 200}}, {45, 87, 135});
    polygon({{x + m * 5, y + 200}, {x + m * 65, y + 200}, {x + m * 65, y + 210}, {x + m * 5, y + 210}}, {32, 62, 97});

    /// Seventh Window
    polygon({{x + m * 5, y + 220}, {x + m * 65, y + 220}, {x + m * 65, y + 240}, {x + m * 5, y + 240}}, {102, 255, 147});
    polygon({{x + m * 5, y + 240}, {x + m * 65, y + 240}, {x + m * 65, y + 250}, {x + m * 5, y + 250}}, {67, 164, 97});

    /// Eight Window
    polygon({{x + m * 5, y + 260}, {x + m * 65, y + 260}, {x + m * 65, y + 290}, {x + m * 5, y + 290}}, {51, 87, 145});

    /// Ninth Window
    polygon({{x + m * 5, y + 300}, {x + m * 65, y + 300}, {x + m * 65, y + 320}, {x + m * 5, y + 320}}, {38, 46, 127});
    polygon({{x + m * 5, y + 320}, {x + m * 65, y + 320}, {x + m * 65, y + 330}, {x + m * 5, y + 330}}, {47, 25, 89});

    /// Tenth Window
    polygon({{x + m * 5, y + 340}, {x + m * 65, y + 340}, {x + m * 65, y + 360}, {x + m * 5, y + 360}}, {43, 221, 207});
    polygon({{x + m * 5, y + 360}, {x + m * 65, y + 360}, {x + m * 65, y + 370}, {x + m * 5, y + 370}}, {34, 165, 146});

    /// Eleventh Window
    polygon({{x + m * 5, y + 380}, {x + m * 65, y + 380}, {x + m * 65, y + 400}, {x + m * 5, y + 400}}, {41, 221, 132});
    polygon({{x + m * 5, y + 400}, {x + m * 65, y + 400}, {x + m * 65, y + 410}, {x + m * 5, y + 410}}, {32, 130, 81});

    /// Twelfth Window
    polygon({{x + m * 5, y + 420}, {x + m * 65, y + 420}, {x + m * 65, y + 440}, {x + m * 5, y + 440}}, {43, 71, 136});
    polygon({{x + m * 5, y + 440}, {x + m * 65, y + 440}, {x + m * 65, y + 450}, {x + m * 5, y + 450}}, {61, 56, 130});

    /// Thirteenth Window
    polygon({{x + m * 5, y + 460}, {x + m * 65, y + 460}, {x + m * 65, y + 480}, {x + m * 5, y + 480}}, {102, 255, 147});
    polygon({{x + m * 5, y + 480}, {x + m * 65, y + 480}, {x + m * 65, y + 490}, {x + m * 5, y + 490}}, {67, 164, 97});

    } else if(day){

        Color windowBottom = {153, 77, 28};
        Color windowTop = {107, 36, 12};

        /// Bottom Window
        polygon({{x + m * 5, y + 0}, {x + m * 65, y + 0}, {x + m * 65, y + 10}, {x + m * 5, y + 10}}, windowTop);

        /// Second Window
        polygon({{x + m * 5, y + 20}, {x + m * 65, y + 20}, {x + m * 65, y + 40}, {x + m * 5, y + 40}}, windowBottom);
        polygon({{x + m * 5, y + 40}, {x + m * 65, y + 40}, {x + m * 65, y + 50}, {x + m * 5, y + 50}}, windowTop);

        /// Third Window
        polygon({{x + m * 5, y + 60}, {x + m * 65, y + 60}, {x + m * 65, y + 80}, {x + m * 5, y + 80}}, windowBottom);
        polygon({{x + m * 5, y + 80}, {x + m * 65, y + 80}, {x + m * 65, y + 90}, {x + m * 5, y + 90}}, windowTop);

        /// Forth Window
        polygon({{x + m * 5, y + 100}, {x + m * 65, y + 100}, {x + m * 65, y + 120}, {x + m * 5, y + 120}}, windowBottom);
        polygon({{x + m * 5, y + 120}, {x + m * 65, y + 120}, {x + m * 65, y + 130}, {x + m * 5, y + 130}}, windowTop);

        /// Fifth Window
        polygon({{x + m * 5, y + 140}, {x + m * 65, y + 140}, {x + m * 65, y + 160}, {x + m * 5, y + 160}}, windowBottom);
        polygon({{x + m * 5, y + 160}, {x + m * 65, y + 160}, {x + m * 65, y + 170}, {x + m * 5, y + 170}}, windowTop);

        /// Sixth Window
        polygon({{x + m * 5, y + 180}, {x + m * 65, y + 180}, {x + m * 65, y + 200}, {x + m * 5, y + 200}}, windowBottom);
        polygon({{x + m * 5, y + 200}, {x + m * 65, y + 200}, {x + m * 65, y + 210}, {x + m * 5, y + 210}}, windowTop);

        /// Seventh Window
        polygon({{x + m * 5, y + 220}, {x + m * 65, y + 220}, {x + m * 65, y + 240}, {x + m * 5, y + 240}}, windowBottom);
        polygon({{x + m * 5, y + 240}, {x + m * 65, y + 240}, {x + m * 65, y + 250}, {x + m * 5, y + 250}}, windowTop);

        /// Eight Window
        polygon({{x + m * 5, y + 260}, {x + m * 65, y + 260}, {x + m * 65, y + 290}, {x + m * 5, y + 290}}, windowTop);

         // Ninth Window
        polygon({{x + m * 5, y + 300}, {x + m * 65, y + 300}, {x + m * 65, y + 320}, {x + m * 5, y + 320}}, windowBottom);
        polygon({{x + m * 5, y + 320}, {x + m * 65, y + 320}, {x + m * 65, y + 330}, {x + m * 5, y + 330}}, windowTop);

        /// Tenth Window
        polygon({{x + m * 5, y + 340}, {x + m * 65, y + 340}, {x + m * 65, y + 360}, {x + m * 5, y + 360}}, windowBottom);
        polygon({{x + m * 5, y + 360}, {x + m * 65, y + 360}, {x + m * 65, y + 370}, {x + m * 5, y + 370}}, windowTop);

        /// Eleventh Window
        polygon({{x + m * 5, y + 380}, {x + m * 65, y + 380}, {x + m * 65, y + 400}, {x + m * 5, y + 400}}, windowBottom);
        polygon({{x + m * 5, y + 400}, {x + m * 65, y + 400}, {x + m * 65, y + 410}, {x + m * 5, y + 410}}, windowTop);

        /// Twelfth Window
        polygon({{x + m * 5, y + 420}, {x + m * 65, y + 420}, {x + m * 65, y + 440}, {x + m * 5, y + 440}}, windowBottom);
        polygon({{x + m * 5, y + 440}, {x + m * 65, y + 440}, {x + m * 65, y + 450}, {x + m * 5, y + 450}}, windowTop);

        /// Thirteenth Window
        polygon({{x + m * 5, y + 460}, {x + m * 65, y + 460}, {x + m * 65, y + 480}, {x + m * 5, y + 480}}, windowBottom);
        polygon({{x + m * 5, y + 480}, {x + m * 65, y + 480}, {x + m * 65, y + 490}, {x + m * 5, y + 490}}, windowTop);
    }
}

void SignBoard_Two(float x, float y, Color shadow = {19, 23, 69}){
    /// Pole's back shadow
    polygon({{x + 20, y + 60}, {x + 100, y + 60}, {x + 100, y + 70}, {x + 20, y + 70}}, {35, 48, 98});

    /// Poles
    polygon({{x + 10, y + 60}, {x + 20, y + 60}, {x + 20, y + 70}, {x + 10, y + 70}}, shadow);
    polygon({{x + 40, y + 60}, {x + 50, y + 60}, {x + 50, y + 70}, {x + 40, y + 70}}, shadow);
    polygon({{x + 70, y + 60}, {x + 80, y + 60}, {x + 80, y + 70}, {x + 70, y + 70}}, shadow);
    polygon({{x + 100, y + 60}, {x + 110, y + 60}, {x + 110, y + 70}, {x + 100, y + 70}}, shadow);

    /// Board
    polygon({{x + 0, y + 70}, {x + 120, y + 70}, {x + 120, y + 110}, {x + 0, y + 110}}, {63, 73, 249});

    /// Board Objects
    polygon({{x + 15, y + 105}, {x + 20, y + 105}, {x + 20, y + 110}, {x + 15, y + 110}}, {125, 187, 255});
    polygon({{x + 25, y + 105}, {x + 80, y + 105}, {x + 80, y + 110}, {x + 25, y + 110}}, {125, 187, 255});

    polygon({{x + 20, y + 80}, {x + 55, y + 80}, {x + 55, y + 105}, {x + 20, y + 105}}, shadow);
    polygon({{x + 55, y + 80}, {x + 75, y + 80}, {x + 75, y + 90}, {x + 55, y + 90}}, shadow);
    polygon({{x + 75, y + 85}, {x + 95, y + 85}, {x + 95, y + 90}, {x + 75, y + 90}}, shadow);
    polygon({{x + 80, y + 85}, {x + 95, y + 85}, {x + 95, y + 100}, {x + 80, y + 100}}, shadow);
    polygon({{x + 90, y + 80}, {x + 100, y + 80}, {x + 100, y + 100}, {x + 90, y + 100}}, shadow);
    polygon({{x + 20, y + 90}, {x + 35, y + 90}, {x + 35, y + 105}, {x + 20, y + 105}}, {239, 46, 65});
}

void Eight_Building(float x, float y, int m = 1, Color buildingWall = {24, 31, 61}, Color buildingWall2  = {42, 33, 186}, Color buildingWall3 = {35, 4, 46}, Color buildingWall4 = {27, 46, 75},
                    Color windowLightBottom = {34, 203, 197}, Color windowLightTop = {19, 122, 117}, Color SecondwindowLightBottom = {7, 79, 178}, Color SecondwindowLightTop = {18, 43, 135},
                    Color windowDarkBottom = {31, 19, 92}, Color windowDarkTop = {41, 0, 70}, Color SecondwindowDarkBottom = {23, 125, 122}, Color SecondwindowDarkTop = {39, 80, 92}
                    ){
    if(night){
        buildingWall = {24, 31, 61};
        buildingWall2  = {42, 33, 186};
        windowLightBottom = {34, 203, 197};
        windowLightTop = {19, 122, 117};
        SecondwindowLightBottom = {7, 79, 178};
        SecondwindowLightTop = {18, 43, 135};
        windowDarkBottom = {31, 19, 92};
        windowDarkTop = {41, 0, 70};
        SecondwindowDarkBottom = {23, 125, 122};
        SecondwindowDarkTop = {39, 80, 92};
    }else if(day){
        buildingWall2 = {153, 77, 28};
        buildingWall  = {228, 143, 69};
        windowLightBottom = {251, 139, 36};
        windowLightTop = {145, 15, 64};
        SecondwindowLightBottom = {153, 77, 28};
        SecondwindowLightTop = {107, 36, 12};
        windowDarkBottom = {107, 36, 12};
        windowDarkTop = {53, 27, 28};
        SecondwindowDarkBottom = {226, 139, 59};
        SecondwindowDarkTop = {125, 12, 55};
    }
    for (int i = 0; i < 200; ++i) {
        Color interpolatedColor = interpolateColor(buildingWall, buildingWall2, static_cast<float>(i) / 200.0f);
        polygon({{x + m * i, y + 0}, {x + m * (i + 1), y + 0}, {x + m * (i + 1), y + 250}, {x + m * i, y + 250}}, interpolatedColor);
    }

    polygon({{x + m * 0, y + 70}, {x + m * 200, y + 70}, {x + m * 200, y + 75}, {x + m * 0, y + 75}}, {0, 0, 0});
    polygon({{x + m * 0, y + 150}, {x + m * 200, y + 150}, {x + m * 200, y + 155}, {x + m * 0, y + 155}}, {0, 0, 0});

    polygon({{x + m * 40, y + 250}, {x + m * 160, y + 250}, {x + m * 160, y + 255}, {x + m * 40, y + 255}}, {0, 0, 0});

    /// Windows Layer One
    polygon({{x + m * 0, y + 10}, {x + m * 40, y + 10}, {x + m * 40, y + 20}, {x + m * 0, y + 20}}, windowLightBottom);
    polygon({{x + m * 0, y + 20}, {x + m * 40, y + 20}, {x + m * 40, y + 30}, {x + m * 0, y + 30}}, windowLightTop);

    polygon({{x + m * 50, y + 10}, {x + m * 100, y + 10}, {x + m * 100, y + 20}, {x + m * 50, y + 20}}, SecondwindowLightBottom);
    polygon({{x + m * 50, y + 20}, {x + m * 100, y + 20}, {x + m * 100, y + 30}, {x + m * 50, y + 30}}, SecondwindowLightTop);


    polygon({{x + m * 100, y + 10}, {x + m * 140, y + 10}, {x + m * 140, y + 20}, {x + m * 100, y + 20}}, windowDarkBottom);
    polygon({{x + m * 100, y + 20}, {x + m * 140, y + 20}, {x + m * 140, y + 30}, {x + m * 100, y + 30}}, windowDarkTop);

    polygon({{x + m * 150, y + 10}, {x + m * 175, y + 10}, {x + m * 175, y + 20}, {x + m * 150, y + 20}}, windowDarkBottom);
    polygon({{x + m * 150, y + 20}, {x + m * 175, y + 20}, {x + m * 175, y + 30}, {x + m * 150, y + 30}}, windowDarkTop);

    polygon({{x + m * 180, y + 10}, {x + m * 200, y + 10}, {x + m * 200, y + 20}, {x + m * 180, y + 20}}, SecondwindowDarkBottom);
    polygon({{x + m * 180, y + 20}, {x + m * 200, y + 20}, {x + m * 200, y + 30}, {x + m * 180, y + 30}}, SecondwindowDarkTop);

    /// Windows Layer Two
    polygon({{x + m * 0, y + 40}, {x + m * 20, y + 40}, {x + m * 20, y + 50}, {x + m * 0, y + 50}}, SecondwindowLightBottom);
    polygon({{x + m * 0, y + 50}, {x + m * 20, y + 50}, {x + m * 20, y + 60}, {x + m * 0, y + 60}}, SecondwindowLightTop);

    polygon({{x + m * 30, y + 40}, {x + m * 50, y + 40}, {x + m * 50, y + 50}, {x + m * 30, y + 50}}, windowLightBottom);
    polygon({{x + m * 30, y + 50}, {x + m * 50, y + 50}, {x + m * 50, y + 60}, {x + m * 30, y + 60}}, windowLightTop);

    polygon({{x + m * 60, y + 40}, {x + m * 100, y + 40}, {x + m * 100, y + 50}, {x + m * 60, y + 50}}, windowLightBottom);
    polygon({{x + m * 60, y + 50}, {x + m * 100, y + 50}, {x + m * 100, y + 60}, {x + m * 60, y + 60}}, windowLightTop);

    polygon({{x + m * 100, y + 40}, {x + m * 130, y + 40}, {x + m * 130, y + 50}, {x + m * 100, y + 50}}, SecondwindowDarkBottom);
    polygon({{x + m * 100, y + 50}, {x + m * 130, y + 50}, {x + m * 130, y + 60}, {x + m * 100, y + 60}}, SecondwindowDarkTop);

    polygon({{x + m * 140, y + 40}, {x + m * 160, y + 40}, {x + m * 160, y + 50}, {x + m * 140, y + 50}}, windowDarkBottom);
    polygon({{x + m * 140, y + 50}, {x + m * 160, y + 50}, {x + m * 160, y + 60}, {x + m * 140, y + 60}}, windowDarkTop);

    polygon({{x + m * 170, y + 40}, {x + m * 200, y + 40}, {x + m * 200, y + 50}, {x + m * 170, y + 50}}, SecondwindowDarkBottom);
    polygon({{x + m * 170, y + 50}, {x + m * 200, y + 50}, {x + m * 200, y + 60}, {x + m * 170, y + 60}}, SecondwindowDarkTop);

    /// Window Layer Three
    polygon({{x + m * 0, y + 80}, {x + m * 20, y + 80}, {x + m * 20, y + 90}, {x + m * 0, y + 90}}, SecondwindowLightBottom);
    polygon({{x + m * 0, y + 90}, {x + m * 20, y + 90}, {x + m * 20, y + 100}, {x + m * 0, y + 100}}, windowLightTop);

    polygon({{x + m * 30, y + 80}, {x + m * 50, y + 80}, {x + m * 50, y + 90}, {x + m * 30, y + 90}}, SecondwindowLightBottom);
    polygon({{x + m * 30, y + 90}, {x + m * 50, y + 90}, {x + m * 50, y + 100}, {x + m * 30, y + 100}}, windowLightTop);

    polygon({{x + m * 60, y + 80}, {x + m * 80, y + 80}, {x + m * 80, y + 90}, {x + m * 60, y + 90}}, SecondwindowLightBottom);
    polygon({{x + m * 60, y + 90}, {x + m * 80, y + 90}, {x + m * 80, y + 100}, {x + m * 60, y + 100}}, windowLightTop);

    polygon({{x + m * 90, y + 80}, {x + m * 100, y + 80}, {x + m * 100, y + 90}, {x + m * 90, y + 90}}, SecondwindowLightBottom);
    polygon({{x + m * 90, y + 90}, {x + m * 100, y + 90}, {x + m * 100, y + 100}, {x + m * 90, y + 100}}, windowLightTop);

    polygon({{x + m * 100, y + 80}, {x + m * 110, y + 80}, {x + m * 110, y + 90}, {x + m * 100, y + 90}}, windowDarkBottom);
    polygon({{x + m * 100, y + 90}, {x + m * 110, y + 90}, {x + m * 110, y + 100}, {x + m * 100, y + 100}}, windowDarkTop);

    polygon({{x + m * 120, y + 80}, {x + m * 140, y + 80}, {x + m * 140, y + 90}, {x + m * 120, y + 90}}, windowDarkBottom);
    polygon({{x + m * 120, y + 90}, {x + m * 140, y + 90}, {x + m * 140, y + 100}, {x + m * 120, y + 100}}, windowDarkTop);

    polygon({{x + m * 150, y + 80}, {x + m * 170, y + 80}, {x + m * 170, y + 90}, {x + m * 150, y + 90}}, windowDarkBottom);
    polygon({{x + m * 150, y + 90}, {x + m * 170, y + 90}, {x + m * 170, y + 100}, {x + m * 150, y + 100}}, windowDarkTop);

    polygon({{x + m * 180, y + 80}, {x + m * 200, y + 80}, {x + m * 200, y + 90}, {x + m * 180, y + 90}}, windowDarkBottom);
    polygon({{x + m * 180, y + 90}, {x + m * 200, y + 90}, {x + m * 200, y + 100}, {x + m * 180, y + 100}}, windowDarkTop);

    /// Window Layer Four
    polygon({{x + m * 0, y + 115}, {x + m * 40, y + 115}, {x + m * 40, y + 125}, {x + m * 0, y + 125}}, windowLightBottom);
    polygon({{x + m * 0, y + 125}, {x + m * 40, y + 125}, {x + m * 40, y + 135}, {x + m * 0, y + 135}}, windowLightTop);

    polygon({{x + m * 50, y + 115}, {x + m * 100, y + 115}, {x + m * 100, y + 125}, {x + m * 50, y + 125}}, SecondwindowLightBottom);
    polygon({{x + m * 50, y + 125}, {x + m * 100, y + 125}, {x + m * 100, y + 135}, {x + m * 50, y + 135}}, windowLightTop);

    polygon({{x + m * 100, y + 115}, {x + m * 150, y + 115}, {x + m * 150, y + 125}, {x + m * 100, y + 125}}, windowDarkBottom);
    polygon({{x + m * 100, y + 125}, {x + m * 150, y + 125}, {x + m * 150, y + 135}, {x + m * 100, y + 135}}, windowDarkTop);

    polygon({{x + m * 160, y + 115}, {x + m * 180, y + 115}, {x + m * 180, y + 125}, {x + m * 160, y + 125}}, SecondwindowDarkBottom);
    polygon({{x + m * 160, y + 125}, {x + m * 180, y + 125}, {x + m * 180, y + 135}, {x + m * 160, y + 135}}, SecondwindowDarkTop);

    polygon({{x + m * 190, y + 115}, {x + m * 200, y + 115}, {x + m * 200, y + 125}, {x + m * 190, y + 125}}, windowDarkBottom);
    polygon({{x + m * 190, y + 125}, {x + m * 200, y + 125}, {x + m * 200, y + 135}, {x + m * 190, y + 135}}, windowDarkTop);

    /// Window Layer Five
    polygon({{x + m * 0, y + 160}, {x + m * 20, y + 160}, {x + m * 20, y + 170}, {x + m * 0, y + 170}}, SecondwindowLightBottom);
    polygon({{x + m * 0, y + 170}, {x + m * 20, y + 170}, {x + m * 20, y + 180}, {x + m * 0, y + 180}}, windowLightTop);

    polygon({{x + m * 30, y + 160}, {x + m * 50, y + 160}, {x + m * 50, y + 170}, {x + m * 30, y + 170}}, windowLightBottom);
    polygon({{x + m * 30, y + 170}, {x + m * 50, y + 170}, {x + m * 50, y + 180}, {x + m * 30, y + 180}}, windowLightTop);

    polygon({{x + m * 60, y + 160}, {x + m * 80, y + 160}, {x + m * 80, y + 170}, {x + m * 60, y + 170}}, SecondwindowLightBottom);
    polygon({{x + m * 60, y + 170}, {x + m * 80, y + 170}, {x + m * 80, y + 180}, {x + m * 60, y + 180}}, windowLightTop);

    polygon({{x + m * 90, y + 160}, {x + m * 100, y + 160}, {x + m * 100, y + 170}, {x + m * 90, y + 170}}, SecondwindowLightBottom);
    polygon({{x + m * 90, y + 170}, {x + m * 100, y + 170}, {x + m * 100, y + 180}, {x + m * 90, y + 180}}, windowLightTop);

    polygon({{x + m * 100, y + 160}, {x + m * 110, y + 160}, {x + m * 110, y + 170}, {x + m * 100, y + 170}}, windowDarkBottom);
    polygon({{x + m * 100, y + 170}, {x + m * 110, y + 170}, {x + m * 110, y + 180}, {x + m * 100, y + 180}}, windowDarkTop);

    polygon({{x + m * 120, y + 160}, {x + m * 140, y + 160}, {x + m * 140, y + 170}, {x + m * 120, y + 170}}, windowDarkBottom);
    polygon({{x + m * 120, y + 170}, {x + m * 140, y + 170}, {x + m * 140, y + 180}, {x + m * 120, y + 180}}, windowDarkTop);

    polygon({{x + m * 150, y + 160}, {x + m * 170, y + 160}, {x + m * 170, y + 170}, {x + m * 150, y + 170}}, windowDarkBottom);
    polygon({{x + m * 150, y + 170}, {x + m * 170, y + 170}, {x + m * 170, y + 180}, {x + m * 150, y + 180}}, windowDarkTop);

    polygon({{x + m * 180, y + 160}, {x + m * 200, y + 160}, {x + m * 200, y + 170}, {x + m * 180, y + 170}}, SecondwindowDarkBottom);
    polygon({{x + m * 180, y + 170}, {x + m * 200, y + 170}, {x + m * 200, y + 180}, {x + m * 180, y + 180}}, SecondwindowDarkTop);

    /// Window Layer Six
    polygon({{x + m * 0, y + 195}, {x + m * 50, y + 195}, {x + m * 50, y + 205}, {x + m * 0, y + 205}}, windowLightBottom);
    polygon({{x + m * 0, y + 205}, {x + m * 50, y + 205}, {x + m * 50, y + 215}, {x + m * 0, y + 215}}, windowLightTop);

    polygon({{x + m * 60, y + 195}, {x + m * 100, y + 195}, {x + m * 100, y + 205}, {x + m * 60, y + 205}}, windowLightBottom);
    polygon({{x + m * 60, y + 205}, {x + m * 100, y + 205}, {x + m * 100, y + 215}, {x + m * 60, y + 215}}, windowLightTop);

    polygon({{x + m * 100, y + 195}, {x + m * 140, y + 195}, {x + m * 140, y + 205}, {x + m * 100, y + 205}}, SecondwindowDarkBottom);
    polygon({{x + m * 100, y + 205}, {x + m * 140, y + 205}, {x + m * 140, y + 215}, {x + m * 100, y + 215}}, SecondwindowDarkTop);

    polygon({{x + m * 150, y + 195}, {x + m * 200, y + 195}, {x + m * 200, y + 205}, {x + m * 150, y + 205}}, windowDarkBottom);
    polygon({{x + m * 150, y + 205}, {x + m * 200, y + 205}, {x + m * 200, y + 215}, {x + m * 150, y + 215}}, windowDarkTop);

    /// Window Layer Seven
    polygon({{x + m * 0, y + 225}, {x + m * 30, y + 225}, {x + m * 30, y + 232}, {x + m * 0, y + 232}}, windowLightBottom);
    polygon({{x + m * 0, y + 232}, {x + m * 30, y + 232}, {x + m * 30, y + 240}, {x + m * 0, y + 240}}, windowLightTop);

    polygon({{x + m * 40, y + 225}, {x + m * 70, y + 225}, {x + m * 70, y + 232}, {x + m * 40, y + 232}}, SecondwindowLightBottom);
    polygon({{x + m * 40, y + 232}, {x + m * 70, y + 232}, {x + m * 70, y + 240}, {x + m * 40, y + 240}}, windowLightTop);

    polygon({{x + m * 80, y + 225}, {x + m * 100, y + 225}, {x + m * 100, y + 232}, {x + m * 80, y + 232}}, windowLightBottom);
    polygon({{x + m * 80, y + 232}, {x + m * 100, y + 232}, {x + m * 100, y + 240}, {x + m * 80, y + 240}}, windowLightTop);

    polygon({{x + m * 100, y + 225}, {x + m * 120, y + 225}, {x + m * 120, y + 232}, {x + m * 100, y + 232}}, SecondwindowDarkBottom);
    polygon({{x + m * 100, y + 232}, {x + m * 120, y + 232}, {x + m * 120, y + 240}, {x + m * 100, y + 240}}, SecondwindowDarkTop);

    polygon({{x + m * 130, y + 225}, {x + m * 160, y + 225}, {x + m * 160, y + 232}, {x + m * 130, y + 232}}, SecondwindowDarkBottom);
    polygon({{x + m * 130, y + 232}, {x + m * 160, y + 232}, {x + m * 160, y + 240}, {x + m * 130, y + 240}}, SecondwindowDarkTop);

    polygon({{x + m * 170, y + 225}, {x + m * 200, y + 225}, {x + m * 200, y + 232}, {x + m * 170, y + 232}}, SecondwindowDarkBottom);
    polygon({{x + m * 170, y + 232}, {x + m * 200, y + 232}, {x + m * 200, y + 240}, {x + m * 170, y + 240}}, SecondwindowDarkTop);
}

void Shadow_Building_One(float x, float y, int m = 1, Color shadow = {19, 23, 69}){
    polygon({{x + m * 0, y + 0}, {x + m * 60, y + 0}, {x + m * 60, y + 40}, {x + m * 0, y + 40}}, shadow);
    polygon({{x + m * 0, y + 40}, {x + m * 45, y + 40}, {x + m * 45, y + 60}, {x + m * 0, y + 60}}, shadow);
    polygon({{x + m * 0, y + 60}, {x + m * 30, y + 60}, {x + m * 30, y + 80}, {x + m * 0, y + 80}}, shadow);
    polygon({{x + m * 15, y + 80}, {x + m * 25, y + 80}, {x + m * 25, y + 95}, {x + m * 15, y + 95}}, shadow);

    polygon({{x + m * 70, y - 85}, {x + m * 80, y - 85}, {x + m * 80, y + 165}, {x + m * 70, y + 165}}, shadow);
}

void Shadow_Building_Three(float x, float y, Color shadow = {19, 23, 69}){
    polygon({{x + 0, y + 0}, {x + 50, y + 0}, {x + 50, y + 130}, {x + 0, y + 130}}, shadow);
    polygon({{x + 10, y + 60}, {x + 50, y + 60}, {x + 50, y + 245}, {x + 10, y + 245}}, shadow);

    polygon({{x + 20, y + 245}, {x + 40, y + 245}, {x + 40, y + 250}, {x + 20, y + 250}}, shadow);
    polygon({{x + 10, y + 250}, {x + 50, y + 250}, {x + 50, y + 255}, {x + 10, y + 255}}, shadow);

    polygon({{x + 20, y + 255}, {x + 40, y + 255}, {x + 40, y + 260}, {x + 20, y + 260}}, shadow);
    polygon({{x + 10, y + 260}, {x + 50, y + 260}, {x + 50, y + 265}, {x + 10, y + 265}}, shadow);

    polygon({{x + 20, y + 265}, {x + 40, y + 265}, {x + 40, y + 270}, {x + 20, y + 270}}, shadow);
    polygon({{x + 10, y + 270}, {x + 50, y + 270}, {x + 50, y + 275}, {x + 10, y + 275}}, shadow);

    polygon({{x + 20, y + 275}, {x + 40, y + 275}, {x + 40, y + 280}, {x + 20, y + 280}}, shadow);
    polygon({{x + 10, y + 280}, {x + 50, y + 280}, {x + 50, y + 285}, {x + 10, y + 285}}, shadow);

    polygon({{x + 20, y + 285}, {x + 40, y + 285}, {x + 40, y + 290}, {x + 20, y + 290}}, shadow);
    polygon({{x + 10, y + 290}, {x + 50, y + 290}, {x + 50, y + 295}, {x + 10, y + 295}}, shadow);

    polygon({{x + 20, y + 295}, {x + 40, y + 295}, {x + 40, y + 300}, {x + 20, y + 300}}, shadow);
    polygon({{x + 10, y + 300}, {x + 50, y + 300}, {x + 50, y + 310}, {x + 10, y + 310}}, shadow);
}

void Building_Tower(float x, float y, Color shadow = {19, 23, 69}, Color light = {130, 86, 199}, Color red = {244, 29, 27}, Color black = {19, 23, 69}, Color pole = {53, 41, 107}){
    if(night){
        light = {130, 86, 199};
        shadow = {19, 23, 69};
        pole = {53, 41, 107};
    } else if (day){
        light = {228, 143, 69};
        shadow = {153, 77, 28};
        pole = {107, 36, 12};
    }
    polygon({{x + -5, y}, {x + 70, y}, {x + 70, y + 15}, {x + -5, y + 15}}, light);
    polygon({{x + 70, y}, {x + 145, y}, {x + 145, y + 15}, {x + 70, y + 15}}, shadow);

    polygon({{x + -5, y + 15}, {x + 65, y + 15}, {x + 65, y + 30}, {x + -5, y + 30}}, light);
    polygon({{x + 65, y + 15}, {x + 140, y + 15}, {x + 140, y + 30}, {x + 65, y + 30}}, shadow);

    polygon({{x + -5, y + 30}, {x + 65, y + 30}, {x + 65, y + 45}, {x + -5, y + 45}}, light);
    polygon({{x + 60, y + 30}, {x + 135, y + 30}, {x + 135, y + 45}, {x + 60, y + 45}}, shadow);

    polygon({{x + -5, y + 45}, {x + 60, y + 45}, {x + 60, y + 60}, {x + -5, y + 60}}, light);
    polygon({{x + 55, y + 45}, {x + 130, y + 45}, {x + 130, y + 60}, {x + 55, y + 60}}, shadow);

    polygon({{x + -5, y + 60}, {x + 55, y + 60}, {x + 55, y + 75}, {x + -5, y + 75}}, light);
    polygon({{x + 50, y + 60}, {x + 125, y + 60}, {x + 125, y + 75}, {x + 50, y + 75}}, shadow);

    polygon({{x + 90, y + 15}, {x + 110, y + 15}, {x + 110, y + 90}, {x + 90, y + 90}}, pole);
    polygon({{x + 85, y + 90}, {x + 110, y + 90}, {x + 100, y + 105}, {x + 75, y + 105}}, {40, 55, 76});
    polygon({{x + 85, y + 15}, {x + 90, y + 15}, {x + 90, y + 95}, {x + 85, y + 95}}, light);
    polygon({{x + 80, y + 30}, {x + 85, y + 30}, {x + 85, y + 105}, {x + 80, y + 105}}, light);
    polygon({{x + 75, y + 45}, {x + 80, y + 45}, {x + 80, y + 110}, {x + 75, y + 110}}, light);

    polygon({{x + 15, y + 75}, {x + 40, y + 75}, {x + 40, y + 125}, {x + 15, y + 125}}, light);
    polygon({{x + 40, y + 75}, {x + 65, y + 75}, {x + 65, y + 110}, {x + 40, y + 110}}, shadow);

    polygon({{x + 27, y + 120}, {x + 40, y + 120}, {x + 40, y + 125}, {x + 27, y + 125}}, black);
    polygon({{x + 35, y + 125}, {x + 40, y + 125}, {x + 40, y + 135}, {x + 35, y + 135}}, black);
    if(night) polygon({{x + 34, y + 135}, {x + 41, y + 135}, {x + 41, y + 140}, {x + 34, y + 140}}, red);

    polygon({{x + 40, y + 90}, {x + 80, y + 90}, {x + 80, y + 110}, {x + 40, y + 110}}, light);
    polygon({{x + 50, y + 105}, {x + 75, y + 105}, {x + 75, y + 110}, {x + 50, y + 110}}, black);
    polygon({{x + 65, y + 80}, {x + 80, y + 80}, {x + 80, y + 90}, {x + 65, y + 90}}, light);
    polygon({{x + 85, y + 95}, {x + 90, y + 95}, {x + 90, y + 100}, {x + 85, y + 100}}, black);

    polygon({{x + 95, y + 95}, {x + 100, y + 95}, {x + 100, y + 120}, {x + 95, y + 120}}, black);
    if(night) polygon({{x + 94, y + 120}, {x + 101, y + 120}, {x + 101, y + 125}, {x + 94, y + 125}}, red);
}

void setGLColorWithAlpha(const Color& color, int alpha) {
    glColor4ub(color.r, color.g, color.b, alpha);
}

void Laser(float x, float y, Color shadow = {97, 252, 254}, int alpha = 100){

    glPushMatrix();
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    setGLColorWithAlpha(shadow, alpha);
    glBegin(GL_POLYGON);
    glVertex2f(x + 0, y + 0);
    glVertex2f(x + 5, y + 0);
    glVertex2f(x + 5, y + 600);
    glVertex2f(x + 0, y + 600);
    glEnd();

    setGLColorWithAlpha({48, 122, 255}, alpha);
    glBegin(GL_POLYGON);
    glVertex2f(x + 5, y + 0);
    glVertex2f(x + 45, y + 0);
    glVertex2f(x + 45, y + 600);
    glVertex2f(x + 5, y + 600);
    glEnd();

    setGLColorWithAlpha(shadow, alpha);
    glBegin(GL_POLYGON);
    glVertex2f(x + 45, y + 0);
    glVertex2f(x + 50, y + 0);
    glVertex2f(x + 50, y + 600);
    glVertex2f(x + 45, y + 600);
    glEnd();

    glDisable(GL_BLEND);
    glPopMatrix();
}

void LaserObjects(float x, float y, Color body = {97, 252, 254}){
    polygon({{x + 20, y + 10 + objectPositionY}, {x + 25, y + 10 + objectPositionY}, {x + 25, y + 15 + objectPositionY}, {x + 20, y + 15 + objectPositionY}}, body);
    polygon({{x + 30, y + 20 - objectPositionY}, {x + 35, y + 20 - objectPositionY}, {x + 35, y + 25 - objectPositionY}, {x + 30, y + 25 - objectPositionY}}, body);
    polygon({{x + 25, y + 40 + objectPositionY}, {x + 30, y + 40 + objectPositionY}, {x + 30, y + 45 + objectPositionY}, {x + 25, y + 45 + objectPositionY}}, body);
    polygon({{x + 20, y + 50 - objectPositionY}, {x + 25, y + 50 - objectPositionY}, {x + 25, y + 55 - objectPositionY}, {x + 20, y + 55 - objectPositionY}}, body);
}

void updateLaserObjects(int value) {
    if(objectPositionX < 2000){
        objectPositionX += 20;
    }
    else{
        objectPositionX = 0;
    }

    if(objectPositionY < 10){
        objectPositionY += 1;
    }
    else{
        objectPositionY = 0;
    }

    glutPostRedisplay();
    glutTimerFunc(50, updateLaserObjects, 0);
}

/// SignBoard Building Two
void SignBoard2(float x, float y, Color body = {211, 0, 253}, Color light = {111, 81, 143}){
    /// Poles
    polygon({{x + 0, y + 0}, {x + 30, y + 0}, {x + 30, y + 5}, {x + 0, y + 5}}, {22, 28, 40});
    polygon({{x + 0, y + 5}, {x + 30, y + 5}, {x + 30, y + 35}, {x + 0, y + 35}}, {53, 59, 107});
    polygon({{x + 0, y + 35}, {x + 30, y + 35}, {x + 30, y + 40}, {x + 0, y + 40}}, {22, 28, 40});

    /// Body
    polygon({{x - 10 , y - 10}, {x + 15, y - 10}, {x + 15, y + 50}, {x - 10, y + 50}}, body);
    polygon({{x - 5 , y + 50}, {x + 10, y + 50}, {x + 10, y + 55}, {x - 5, y + 55}}, body);
    polygon({{x - 5 , y - 15}, {x + 10, y - 15}, {x + 10, y - 10}, {x - 5, y - 10}}, body);

    /// Light
    polygon({{x - 15 , y - 10}, {x - 10, y - 10}, {x - 10, y + 50}, {x - 15, y + 50}}, light);
    polygon({{x + 15 , y - 10}, {x + 20, y - 10}, {x + 20, y + 50}, {x + 15, y + 50}}, light);
    polygon({{x - 10 , y + 50}, {x - 5, y + 50}, {x - 5, y + 55}, {x - 10, y + 55}}, light);
    polygon({{x + 10 , y + 50}, {x + 15, y + 50}, {x + 15, y + 55}, {x + 10, y + 55}}, light);
    polygon({{x - 5 , y + 55}, {x + 10, y + 55}, {x + 10, y + 60}, {x - 5, y + 60}}, light);
    polygon({{x - 5 , y - 20}, {x + 10, y - 20}, {x + 10, y - 15}, {x - 5, y - 15}}, light);
    polygon({{x - 10 , y - 15}, {x - 5, y - 15}, {x - 5, y - 10}, {x - 10, y - 10}}, light);
    polygon({{x + 10 , y - 15}, {x + 15, y - 15}, {x + 15, y - 10}, {x + 10, y - 10}}, light);

    /// Objects
    polygon({{x - 5 , y + 0}, {x + 10, y + 0}, {x + 10, y + 5}, {x - 5, y + 5}}, light);
    polygon({{x - 5 , y + 35}, {x + 10, y + 35}, {x + 10, y + 40}, {x - 5, y + 40}}, light);
    polygon({{x - 5 , y + 15}, {x + 10, y + 15}, {x + 10, y + 20}, {x - 5, y + 20}}, light);
    polygon({{x + 0 , y + 20}, {x + 10, y + 20}, {x + 10, y + 25}, {x + 0, y + 25}}, light);
}

/// SignBoard Building Thirteen
void SignBoard13(float x, float y, Color light = {110, 115, 153}){
    /// Poles
    polygon({{x + 0, y + 0}, {x + 30, y + 0}, {x + 30, y + 5}, {x + 0, y + 5}}, {22, 28, 40});
    polygon({{x + 0, y + 5}, {x + 30, y + 5}, {x + 30, y + 35}, {x + 0, y + 35}}, {53, 59, 107});
    polygon({{x + 0, y + 35}, {x + 30, y + 35}, {x + 30, y + 40}, {x + 0, y + 40}}, {22, 28, 40});

    /// Body
    polygon({{x - 10 , y - 10}, {x + 15, y - 10}, {x + 15, y + 50}, {x - 10, y + 50}}, {0, 252, 161});

    /// Light
    polygon({{x - 15 , y - 5}, {x - 10, y - 5}, {x - 10, y + 50}, {x - 15, y + 50}}, light);
    polygon({{x + 15 , y - 5}, {x + 20, y - 5}, {x + 20, y + 50}, {x + 15, y + 50}}, light);
    polygon({{x - 10 , y + 50}, {x + 15, y + 50}, {x + 15, y + 55}, {x - 10, y + 55}}, light);
    polygon({{x - 10 , y - 10}, {x + 15, y - 10}, {x + 15, y - 5}, {x - 10, y - 5}}, light);

    /// Lower Object
    polygon({{x - 10 , y - 10}, {x + 15, y - 10}, {x + 15, y + 10}, {x + 10, y + 10}, {x + 10, y + 5}, {x - 10, y + 5}}, {46, 0, 251});
    polygon({{x - 10 , y + 30}, {x + 15, y + 30}, {x + 15, y + 40}, {x + 10, y + 40}, {x + 10, y + 25}, {x - 10, y + 25}}, {114, 195, 204});
}

void Rain(float x, float y, Color shadow = {197, 255, 248}, int alpha = 50){

    glPushMatrix();
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    setGLColorWithAlpha(shadow, alpha);
    int numLines = 10; // Number of lines
    int xIncrement = 20; // Distance between lines along the x-axis
        int numPolygons = 10; // Number of polygons per line
    int yIncrement = 180; // Distance between polygons along the y-axis

    for (int line = 0; line < numLines; ++line) {
        for (int i = 0; i < numPolygons; ++i) {
            glBegin(GL_POLYGON);

            int startX = line * (numPolygons * xIncrement); // Adjust this for proper spacing
            int startY = i * yIncrement;

            glVertex2f(x + startX, y + startY);
            glVertex2f(x + 2 + startX, y + startY);
            glVertex2f(x + 2 + startX, y + 70 + startY);
            glVertex2f(x + startX, y + 70 + startY);

            glEnd();
        }
    }
    glDisable(GL_BLEND);
    glPopMatrix();
}

/// Timer Function for updating First Carrier Positions
void updateRain(int value) {
    if(rainPosition > - 80){
        rainPosition -= 2;
    }
    else{
        rainPosition = 0;
    }

    glutPostRedisplay();
    glutTimerFunc(1, updateRain, 0);
}

void moveRain(){
    glPushMatrix();
    Rain(10, rainPosition + 10);
    Rain(100, rainPosition + 40);
    glPopMatrix();
}



/// Initializing all Clouds
void drawClouds(){
    cloud3();
    cloud2();
    cloud1();
}

/// Initializing Buildings
void Building(){
    /// Building One
    Building_One(0, 0);
    polygon({{0, 250}, {0, 255}, {140,255}, {140, 250}}, {0, 0, 0});
    Building_One(0, 255);
    Building_One_Terrace(-5, 510);
    SignBoard(-5, 510);

    /// Building Two
    Building_Two(100, 0);
    NetworkTowerTwo(100, 220);

    /// Shadow Building One
    Shadow_Building_One(335, 85);

    /// Building Three
    Third_Building(200, 0);
    NetworkTowerThree(230, 505);

    /// Building Five
    Fifth_Building(420, 0);
    SignBoard_Two(420, 435);

    /// Building Six
    Building_Two(520, 0);
    NetworkTowerTwo(535, 220);

    /// Shadow Building Three
    Shadow_Building_Three(660, 0);

    /// Building Seven
    Building_One(700, 0);

    /// Shadow Building Four
    polygon({{840, 0}, {840, 120}, {860, 120}, {860, 0}}, {19, 23, 69});
    polygon({{860, 0}, {860, 180}, {900, 180}, {900, 0}}, {19, 23, 69});

    /// Building Eight
    Eight_Building(900, 0);
    Building_Two(930, 255);
    NetworkTowerTwo(935, 475);

    /// Shadow Building Five
    polygon({{1080, 0}, {1080, 280}, {1200,280}, {1200, 0}}, {19, 23, 69});

    /// Building Ten
    glPushMatrix();
    glScalef(1.2,1.4,0.0);
    Building_Two(950, 0);
    Building_Tower(950, 220);
    glPopMatrix();

    /// Building Twelve
    Building_One(1380, 0);
    polygon({{1420, 250}, {1500, 250}, {1500, 260}, {1420, 260}}, {19, 23, 69});
    polygon({{1400, 260}, {1500, 260}, {1500, 290}, {1400, 290}}, {19, 23, 69});

    /// Building Eleven
    Building_Two(1280, 0);
    NetworkTowerTwo(1295, 220);

    /// Building Thirteen
    Building_One(1500, 0);
    polygon({{1505, 250}, {1505, 255}, {1640,255}, {1640, 250}}, {0, 0, 0});
    Building_One_Terrace(1500, 510);
    Building_One(1500, 255);

    /// Shadow Building One
    Shadow_Building_One(1845, 85);

    /// Fourteenth Building
    Building_Two(1600, 0);
    NetworkTowerTwo(1615, 220);
    if(night) {
        if(laser){
            Laser(1775, 505);
            LaserObjects(1775, 515 + objectPositionX);
        }
    }

    /// Building Fifteen
    Third_Building(1720, 0);
    NetworkTowerThree(1750, 505);

    /// Building Four
    Tomb(320, 0);

    /// Building Nine
    Tomb(1070, 0);

    /// Building Sixteen
    Tomb(1850, 0);
}

void display()
{
   /// Sky
    Sky();

    /// Stars
    if(night){
        Stars();
    }

    /// Sun for evening
    if(day){Sun(1000, 900);}

    /// Clouds
    drawClouds();

    // Building
    Building();

    /// Plane and Carrier
    movePlane();
    moveCarrer();
    moveCarrerTwo();

    /// Signboards
    moveGhost();

    /// Buildings Window Light Toggle
    LightOnOff();

    /// Signboard for Building Two and Thirteen
    SignBoard2(185, 335);
    SignBoard13(1705, 315);

    /// Rain
    if(night){
        if(rain) moveRain();
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
    case '1': /// for turning on signboard at night
        signBoard = true;
        break;
    case '2': /// for turning off signboard at night
        signBoard = false;
        break;
    case '3': /// for turning on laser at night
        laser = true;
        break;
    case '4': /// for turning off laser at night
        laser = false;
    break;
    case '5': /// for turning on rain at night
        rain = true;
        rainSound = true;
        sndPlaySound("rain.wav", SND_ASYNC | SND_LOOP);
        break;
    case '6': /// for turning off rain at night
        rain = false;
        rainSound = false;
        sndPlaySound(NULL, SND_ASYNC | SND_LOOP);
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
    glutTimerFunc(100, updateGhost, 0);
    glutTimerFunc(100, updateDonute, 0);
    glutTimerFunc(100, updateLights, 0);
    glutTimerFunc(100, updateLights2, 0);
    glutTimerFunc(100, updateLights3, 0);
    glutTimerFunc(50, updateLaserObjects, 0);
    glutTimerFunc(1, updateRain, 0);
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
