#include "KinematicsAtan2.h"
#include <cmath>
#include "../iFOC/Motor.h"

#define PUSHPULL_DEADZONE_MM 0.5

// float KinematicsAtan2::getXYAngle(Point3D& a, Point3D& b)
// {
//     return acosf((a.x * b.x + a.y * b.y) / (sqrtf(a.x * a.x + b.x * b.x) * sqrtf(a.y * a.y + b.y * b.y)));
// }

uint8_t KinematicsAtan2::getXYDimension(Point3D &a)
{
    if(a.x >= 0)
    {
        if(a.y >= 0) return 1;
        return 4;
    }
    if(a.y >= 0) return 2;
    return 3;
}

void KinematicsAtan2::proximalReset()
{
    proximal_act.reset();
    lastProxAngle = 0.0f;
}

void KinematicsAtan2::distalReset()
{
    distal_act.reset();
    lastDistAngle = 0.0f;
}

void KinematicsAtan2::calculate(Point3D& prox, Point3D& dist)
{
    // if(prox.x * lastProx.x < 0)
    // {

    // }
    // float sign = 1.0f;
    // float at = 0.0f;
    // if(prox.x != 0.0f) at = atanf(prox.y / prox.x);
    // float deg = RAD2DEG(at);
    // if(deg >= 0.0f && deg <= 90.0f) sign = 1.0f;
    // else if(deg > 90.0f && deg <= 180.0f) sign = 1.0f;
    // else if(deg > 180.0f && deg <= 270.0f)
    // {
    //     at -= PI;
    //     sign = -1.0f;
    // }
    // else if(deg > 270.0f && deg <= 360.0f)
    // {
    //     at -= PI;
    //     sign = -1.0f;
    // }
    float at = normalizeRad(atan2f(prox.y, prox.x));
    if(proximal_params && std::abs(proximal_act.pull) > proximal_params->max_abs_pushpull)
    {
        prox.x = cosf(at) * proximal_params->max_abs_pushpull;
        prox.y = sinf(at) * proximal_params->max_abs_pushpull;
    }
    proximal_act.pull = sqrtf((prox.x * prox.x) + (prox.y * prox.y));
    if(std::abs(proximal_act.pull) <= PUSHPULL_DEADZONE_MM) proximal_act.pull = 0.0f;

    float diff = at - lastProxAngle;
    if(diff > 0.0f) while(diff >= PI) diff -= PI2;
    else while(diff <= -PI) diff += PI2;
    proximal_act.rotation_angle += diff;
    lastProxAngle = at;

    if(proximal_params)
    {
        proximal_act.translation = _constrain(prox.z, -proximal_params->max_abs_linear, proximal_params->max_abs_linear);
    }

    // uint8_t proxDimension = getXYDimension(prox);
    // if(proxDimension != lastProxDimension)
    // {
    //     qDebugMessage(QString::asprintf("Dim changed, last: %d, now: %d", lastProxDimension, proxDimension));
    //     lastProxDimension = proxDimension;
    // }


    at = normalizeRad(atan2f(dist.y, dist.x));
    if(distal_params && std::abs(distal_act.pull) > distal_params->max_abs_pushpull)
    {
        dist.x = cosf(at) * distal_params->max_abs_pushpull;
        dist.y = sinf(at) * distal_params->max_abs_pushpull;
    }
    distal_act.pull = sqrtf((dist.x * dist.x) + (dist.y * dist.y));
    if(std::abs(distal_act.pull) <= PUSHPULL_DEADZONE_MM) distal_act.pull = 0.0f;

    diff = at - lastDistAngle;
    if(diff > 0.0f) while(diff >= PI) diff -= PI2;
    else while(diff <= -PI) diff += PI2;
    distal_act.rotation_angle += diff;
    lastDistAngle = at;

    if(distal_params)
    {
        distal_act.translation = _constrain(dist.z, -distal_params->max_abs_linear, distal_params->max_abs_linear);
    }
}
