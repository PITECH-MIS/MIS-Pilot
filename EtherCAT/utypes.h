#ifndef __UTYPES_H__
#define __UTYPES_H__

#include <inttypes.h>

/* Object dictionary storage */

#pragma pack(push, 1)
typedef struct slave_inputs_t
{
    struct
    {
        uint8_t MotorCount;
        uint8_t MCUTemp;
        uint16_t Vbus;
        uint16_t FramesPerSec;
        uint32_t Uptime;
    } Interface_State;
    struct
    {
        uint8_t IsAlive;
        uint8_t Enable;
        uint8_t NodeID;
        uint8_t Mode;
        uint8_t MotorTemp;
        uint8_t MCUTemp;
        uint8_t LimiterState;
        uint16_t ErrorCode;
        uint32_t SN;
        float IqReal;
        float IdReal;
        float Vbus;
        float Ibus;
        float EstimateSpeed;
        float EstimateRawAngle;
        float CurrentLimit;
    } Motor1_State;
    struct
    {
        uint8_t IsAlive;
        uint8_t Enable;
        uint8_t NodeID;
        uint8_t Mode;
        uint8_t MotorTemp;
        uint8_t MCUTemp;
        uint8_t LimiterState;
        uint16_t ErrorCode;
        uint32_t SN;
        float IqReal;
        float IdReal;
        float Vbus;
        float Ibus;
        float EstimateSpeed;
        float EstimateRawAngle;
        float CurrentLimit;
    } Motor2_State;
    struct
    {
        uint8_t IsAlive;
        uint8_t Enable;
        uint8_t NodeID;
        uint8_t Mode;
        uint8_t MotorTemp;
        uint8_t MCUTemp;
        uint8_t LimiterState;
        uint16_t ErrorCode;
        uint32_t SN;
        float IqReal;
        float IdReal;
        float Vbus;
        float Ibus;
        float EstimateSpeed;
        float EstimateRawAngle;
        float CurrentLimit;
    } Motor3_State;
    struct
    {
        uint8_t IsAlive;
        uint8_t Enable;
        uint8_t NodeID;
        uint8_t Mode;
        uint8_t MotorTemp;
        uint8_t MCUTemp;
        uint8_t LimiterState;
        uint16_t ErrorCode;
        uint32_t SN;
        float IqReal;
        float IdReal;
        float Vbus;
        float Ibus;
        float EstimateSpeed;
        float EstimateRawAngle;
        float CurrentLimit;
    } Motor4_State;
    struct
    {
        uint8_t IsAlive;
        uint8_t Enable;
        uint8_t NodeID;
        uint8_t Mode;
        uint8_t MotorTemp;
        uint8_t MCUTemp;
        uint8_t LimiterState;
        uint16_t ErrorCode;
        uint32_t SN;
        float IqReal;
        float IdReal;
        float Vbus;
        float Ibus;
        float EstimateSpeed;
        float EstimateRawAngle;
        float CurrentLimit;
    } Motor5_State;
    struct
    {
        uint8_t IsAlive;
        uint8_t Enable;
        uint8_t NodeID;
        uint8_t Mode;
        uint8_t MotorTemp;
        uint8_t MCUTemp;
        uint8_t LimiterState;
        uint16_t ErrorCode;
        uint32_t SN;
        float IqReal;
        float IdReal;
        float Vbus;
        float Ibus;
        float EstimateSpeed;
        float EstimateRawAngle;
        float CurrentLimit;
    } Motor6_State;
    struct
    {
        uint8_t IsAlive;
        uint8_t Enable;
        uint8_t NodeID;
        uint8_t Mode;
        uint8_t MotorTemp;
        uint8_t MCUTemp;
        uint8_t LimiterState;
        uint16_t ErrorCode;
        uint32_t SN;
        float IqReal;
        float IdReal;
        float Vbus;
        float Ibus;
        float EstimateSpeed;
        float EstimateRawAngle;
        float CurrentLimit;
    } Motor7_State;
    struct
    {
        uint8_t IsAlive;
        uint8_t Enable;
        uint8_t NodeID;
        uint8_t Mode;
        uint8_t MotorTemp;
        uint8_t MCUTemp;
        uint8_t LimiterState;
        uint16_t ErrorCode;
        uint32_t SN;
        float IqReal;
        float IdReal;
        float Vbus;
        float Ibus;
        float EstimateSpeed;
        float EstimateRawAngle;
        float CurrentLimit;
    } Motor8_State;
}slave_inputs_t;

typedef struct slave_outputs_t
{
    struct
    {
        uint8_t Enable;
        uint8_t Mode;
        float IqSet;
        float SetSpeed;
        float SetRawAngle;
        float SetTrajectoryAngle;
        float SetCurrentLimit;
    } Motor1_Set;
    struct
    {
        uint8_t Enable;
        uint8_t Mode;
        float IqSet;
        float SetSpeed;
        float SetRawAngle;
        float SetTrajectoryAngle;
        float SetCurrentLimit;
    } Motor2_Set;
    struct
    {
        uint8_t Enable;
        uint8_t Mode;
        float IqSet;
        float SetSpeed;
        float SetRawAngle;
        float SetTrajectoryAngle;
        float SetCurrentLimit;
    } Motor3_Set;
    struct
    {
        uint8_t Enable;
        uint8_t Mode;
        float IqSet;
        float SetSpeed;
        float SetRawAngle;
        float SetTrajectoryAngle;
        float SetCurrentLimit;
    } Motor4_Set;
    struct
    {
        uint8_t Enable;
        uint8_t Mode;
        float IqSet;
        float SetSpeed;
        float SetRawAngle;
        float SetTrajectoryAngle;
        float SetCurrentLimit;
    } Motor5_Set;
    struct
    {
        uint8_t Enable;
        uint8_t Mode;
        float IqSet;
        float SetSpeed;
        float SetRawAngle;
        float SetTrajectoryAngle;
        float SetCurrentLimit;
    } Motor6_Set;
    struct
    {
        uint8_t Enable;
        uint8_t Mode;
        float IqSet;
        float SetSpeed;
        float SetRawAngle;
        float SetTrajectoryAngle;
        float SetCurrentLimit;
    } Motor7_Set;
    struct
    {
        uint8_t Enable;
        uint8_t Mode;
        float IqSet;
        float SetSpeed;
        float SetRawAngle;
        float SetTrajectoryAngle;
        float SetCurrentLimit;
    } Motor8_Set;

}slave_outputs_t;

#pragma pack(pop)

#endif /* __UTYPES_H__ */
