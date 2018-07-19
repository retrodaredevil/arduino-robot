//
// Created by josh on 7/7/18.
//

#ifndef ARDUINO_ROBOT_V2_SENSORS_H
#define ARDUINO_ROBOT_V2_SENSORS_H

#include <Arduino.h>
#include <Zumo32U4.h>

bool initSensors();
void updateSensors();

bool isLeftOnLine();
bool isCenterOnLine();
bool isRightOnLine();

uint8_t getFrontLeftProximity();
uint8_t getFrontRightProximity();
/**
 * @return a value from 1 to 6: 6 being the highest
 */
uint8_t getLeftProximity();
/**
 * @return a value from 1 to 6: 6 being the highest
 */
uint8_t getRightProximity();

int32_t getAccelerometerX();
int32_t getAccelerometerY();
int32_t getAccelerometerZ();

/**
 *
 * @return A positive number if the robot is accelerating forwards, a negative number is robot is being pushed backwards
 */
int32_t getAccelerometerForward();
int32_t getAccelerometerLeft();
int32_t getAccelerometerUp();
int32_t getAccelerometerUpWithGravity();

double getRobotHeading();
void setRobotHeading(double heading);

#endif //ARDUINO_ROBOT_V2_SENSORS_H
