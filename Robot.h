//
// Created by josh on 7/7/18.
//

#ifndef ARDUINO_ROBOT_V2_ROBOT_H
#define ARDUINO_ROBOT_V2_ROBOT_H

#include <Arduino.h>
#include <Zumo32U4.h>
#include "RobotProcess.h"

void initRobot();
void setRobotProcess(RobotProcess *robotProcess);
/**
 * Updates this Robot and its necessary subsystems
 */
void update();
void debug(String line1, String line2 = "");
void throwError(String line1, String line2);
#endif //ARDUINO_ROBOT_V2_ROBOT_H
