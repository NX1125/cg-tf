/* 
 * File:   shapes.h
 * Author: guilh
 *
 * Created on 3 de outubro de 2019, 16:22
 */

#ifndef SHAPES_H
#define SHAPES_H

#include <cmath>

#include <vector>

#include <GL/gl.h>

// Deprecated
struct vector2d {
    double x;
    double y;

    vector2d() {
    }

    vector2d(double x, double y) :
    x(x), y(y) {
    }

    double normalize() {
        double length = sqrt(lengthSqr());
        x /= length;
        y /= length;
        return length;
    }

    double lengthSqr() {
        return x * x + y * y;
    }

    vector2d operator/(const double& right) const {
        vector2d result(*this); // Make a copy of myself.
        result /= right; // Reuse compound assignment
        return result;
    }

    vector2d& operator/=(const double& right) {
        x /= right;
        y /= right;
        return *this;
    }

    double operator*(const vector2d& right) const {
        return x * right.x + y * right.y;
    }

    vector2d operator*(const double& right) const {
        vector2d result(*this); // Make a copy of myself.
        result *= right; // Reuse compound assignment
        return result;
    }

    vector2d& operator*=(double right) {
        x *= right;
        y *= right;
        return *this;
    }

    vector2d operator+(const vector2d& right) const {
        vector2d result(*this); // Make a copy of myself.
        result += right; // Reuse compound assignment
        return result;
    }

    vector2d& operator+=(const vector2d& right) {
        x += right.x;
        y += right.y;
        return *this;
    }

    vector2d operator-(const vector2d& right) const {
        vector2d result(*this); // Make a copy of myself.
        result -= right; // Reuse compound assignment
        return result;
    }

    vector2d& operator-=(const vector2d& right) {
        x -= right.x;
        y -= right.y;
        return *this;
    }

    vector2d operator-() const {
        vector2d result = {x, y}; // Make a copy of myself.
        return result;
    }

    double angle() const {
        return atan2(y, x);
    }

    void swap() {
        double z = x;
        x = y;
        y = z;
    }
};

struct vector3f {
    float x;
    float y;
    float z;

    vector3f() {
    }

    vector3f(float x, float y, float z) :
    x(x), y(y), z(z) {
    }

    float normalize() {
        float l = length();
        x /= l;
        y /= l;
        z /= l;
        return l;
    }

    float lengthSqr() const {
        return x * x + y * y + z * z;
    }

    vector3f operator/(const float& right) const {
        vector3f result(*this); // Make a copy of myself.
        result /= right; // Reuse compound assignment
        return result;
    }

    vector3f& operator/=(const float& right) {
        x /= right;
        y /= right;
        z /= right;
        return *this;
    }

    float operator*(const vector3f& right) const {
        return x * right.x + y * right.y + z * right.z;
    }

    vector3f operator*(const float& right) const {
        vector3f result(*this); // Make a copy of myself.
        result *= right; // Reuse compound assignment
        return result;
    }

    vector3f& operator*=(float right) {
        x *= right;
        y *= right;
        z *= right;
        return *this;
    }

    vector3f operator+(const vector3f& right) const {
        vector3f result(*this); // Make a copy of myself.
        result += right; // Reuse compound assignment
        return result;
    }

    vector3f& operator+=(const vector3f& right) {
        x += right.x;
        y += right.y;
        z += right.z;
        return *this;
    }

    vector3f operator-(const vector3f& right) const {
        vector3f result(*this); // Make a copy of myself.
        result -= right; // Reuse compound assignment
        return result;
    }

    vector3f& operator-=(const vector3f& right) {
        x -= right.x;
        y -= right.y;
        z -= right.z;
        return *this;
    }

    vector3f operator-() const {
        vector3f result = {x, y, z}; // Make a copy of myself.
        return result;
    }
    
    static float angle(const vector3f& a, const vector3f& b) {
        return acosf((a * b) / (a.length() * b.length()));
    }
    
    float length() const {
        return sqrt(lengthSqr());
    }
};

vector2d make_unitVector(double a);

struct point2d {
    double x;
    double y;

    point2d() {
    }

    point2d(double x, double y) :
    x(x), y(y) {
    }

    vector2d operator-(const point2d& right) const {
        vector2d v = {x - right.x, y - right.y};
        return v;
    }

    point2d operator-(const vector2d& right) const {
        point2d v = {x - right.x, y - right.y};
        return v;
    }

    point2d operator+(const vector2d& right) const {
        point2d result(*this); // Make a copy of myself.
        result += right; // Reuse compound assignment
        return result;
    }

    point2d& operator+=(const vector2d& right) {
        x += right.x;
        y += right.y;
        return *this;
    }

    point2d& operator-=(const vector2d& right) {
        x -= right.x;
        y -= right.y;
        return *this;
    }

    void add(vector2d& v, double s);
};

struct point3f {
    
    float x;
    float y;
    float z;

    point3f() {
    }

    point3f(float x, float y, float z) :
    x(x), y(y), z(z) {
    }

    vector3f operator-(const point3f& right) const {
        vector3f v = {x - right.x, y - right.y, z - right.z};
        return v;
    }

    point3f operator-(const vector3f& right) const {
        point3f v = {x - right.x, y - right.y, z - right.z};
        return v;
    }

    point3f operator+(const vector3f& right) const {
        point3f result(*this); // Make a copy of myself.
        result += right; // Reuse compound assignment
        return result;
    }

    point3f& operator+=(const vector3f& right) {
        x += right.x;
        y += right.y;
        z += right.z;
        return *this;
    }

    point3f& operator-=(const vector3f& right) {
        x -= right.x;
        y -= right.y;
        z -= right.z;
        return *this;
    }

    void add(vector3f& v, float s);
    
    static float distanceSqr(const point3f& p1, const point3f& p2);
};

double distanceSqr(const point2d& p1, const point2d& p2);

typedef struct line* Line;

struct line {
    double x1;
    double y1;

    double x2;
    double y2;

    virtual ~line() {
    }

    void draw();

    vector2d createVector() const {
        return vector2d{ x2 - x1, y2 - y1};
    }

    point2d getStart() const {
        return point2d{x1, y1};
    }

    point2d getEnd() const {
        return point2d{x2, y2};
    }
};

typedef struct circle_blueprint_t* CircleBlueprint;

/**
 * A circumference sampled by a fixed angle.
 */
struct circle_blueprint_t {
    /**
     * The number of points this circumference has.
     */
    int numOfPoints;

    /**
     * All sampled points of the circumference.
     */
    struct vector2d* points;

    circle_blueprint_t(double radius, double maxArcLength);

    circle_blueprint_t(unsigned int n);

    virtual ~circle_blueprint_t();

    void draw(bool opaque);

    void draw(bool opaque, double r);
};

typedef struct circle* Circle;

struct circle {
    double cx;
    double cy;

    double radius;

    CircleBlueprint blueprint = NULL;

    virtual ~circle();

    void draw(bool opaque);

    void translate();

    bool intersects(const point2d& center, double radius) const {
        return containsDot(center, this->radius + radius);
    }

    bool contains(const point2d& center, double radius) const {
        return containsDot(center, this->radius - radius);
    }

    bool contains(const point2d& center) const {
        return containsDot(center, this->radius);
    }

    point2d getCenter() {
        return point2d{
            cx, cy};
    }

private:

    bool containsDot(const point2d& center, double radius) const {
        vector2d r = center - point2d{cx, cy};
        return r * r < radius * radius;
    }
};


struct point2d closestPointFromLineToPoint(
        const point2d& start, const vector2d& ray, const point2d& p);

void drawAxis(float size);

#endif /* SHAPES_H */

