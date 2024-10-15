#ifndef KINEMATICSBASE_H
#define KINEMATICSBASE_H

#include <cstring>

// Kinematics Base is Equipment-Wide (6DoF)

typedef class kinematics_params_t
{
public:
    float OD = 0.0f;
    float od = 0.0f;
    float ID = 0.0f;
    float id = 0.0f;
    float length = 0.0f;
    float max_abs_pushpull = 0.0f; // mm
    float max_abs_linear = 0.0f;   // mm
}kinematics_params_t;

typedef class actuation_t
{
public:
    float translation = 0.0f;      // Actua_P/D(1)is the translation dof displacement, unit mm;
    float pull = 0.0f;             // Actua_P/D(2)is the pull dispalcement, unit mm;
    float rotation_angle = 0.0f;   // Actua_P/D(3)is the rotation angle, unit rad;
    void reset()
    {
        translation = 0.0f;
        pull = 0.0f;
        rotation_angle = 0.0f;
    }
}actuation_t;

typedef class Point3D
{
public:
    Point3D() {};
    Point3D(float _x, float _y, float _z) : x(_x), y(_y), z(_z) {};
    float x = 0.0f;
    float y = 0.0f;
    float z = 0.0f;
    void reset()
    {
        x = 0.0f;
        y = 0.0f;
        z = 0.0f;
    }
}Point3D, Vector3D;

typedef class Point2D
{
public:
    float x = 0.0f;
    float y = 0.0f;
    void reset()
    {
        x = 0.0f;
        y = 0.0f;
    }
}Point2D, Vector2D;

class KinematicsBase
{
public:
    // KinematicsBase(kinematics_params_t& prox, kinematics_params_t& dist) : proximal_params(prox), distal_params(dist) {};
    virtual void calculate(Point3D& prox, Point3D& dist) = 0;
    virtual void proximalReset() = 0;
    virtual void distalReset() = 0;
    actuation_t proximal_act;
    actuation_t distal_act;
    kinematics_params_t *proximal_params = nullptr;
    kinematics_params_t *distal_params = nullptr;
private:

};

#endif // KINEMATICSBASE_H
