
#include "shapes.h"

double distanceSqr(const point2d& p1, const point2d& p2) {
    double dx = p1.x - p2.x;
    double dy = p1.y - p2.y;

    return dx * dx + dy * dy;
}

void point2d::add(vector2d& v, double s) {
    x += v.x * s;
    y += v.y * s;
}

vector2d make_unitVector(double a) {
    float x, y;
    sincosf(a, &y, &x);
    return vector2d(x, y);
}

struct point2d closestPointFromLineToPoint(
        const point2d& start, const vector2d& ray, const point2d& p) {
    struct point2d closest;

    double c1 = -ray.x * p.x - ray.y * p.y;
    double c2 = start.y * ray.x - start.x * ray.y;

    closest.x = -(ray.y * c2 + ray.x * c1);
    closest.y = (-ray.y * c1 + ray.x * c2);

    return closest;
}

float point3f::distanceSqr(const point3f& p1, const point3f& p2) {
    vector3f v = p1 - p2;
    return v.lengthSqr();
}

void drawAxisPart(float x, float y, float z) {
    glColor3f(x, y, z);
    glBegin(GL_LINES);
    glVertex3f(0, 0, 0);
    glVertex3f(x, y, z);
    glEnd();
}

void drawAxis(float s) {
    GLboolean b = glIsEnabled(GL_LIGHTING);
    if (b) {
        glDisable(GL_LIGHTING);
    }
    glPushMatrix();
    {
        drawAxisPart(s, 0, 0);
        drawAxisPart(0, s, 0);
        drawAxisPart(0, 0, s);
    }
    glPopMatrix();
    if (b) {
        glEnable(GL_LIGHTING);
    }
}

float clampAngle(float angle) {
    float t = fmodf(angle, M_PI * 2);
    if (angle > 0) {
        return t;
    } else {
        return M_PI * 2 + t;
    }
}
