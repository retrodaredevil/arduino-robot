//
// Created by josh on 7/7/18.
//

#ifndef SUMO_ROBOT_V2_SENSORS_H
#define SUMO_ROBOT_V2_SENSORS_H

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

double getRobotHeading();

#endif //SUMO_ROBOT_V2_SENSORS_H
