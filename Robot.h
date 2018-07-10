//
// Created by josh on 7/7/18.
//

#ifndef SUMO_ROBOT_V2_ROBOT_H
#define SUMO_ROBOT_V2_ROBOT_H

#include <Arduino.h>
#include <Zumo32U4.h>

void initRobot();
/**
 * Updates this Robot and its necessary subsystems
 */
void update();
void debug(String line1, String line2 = "");
#endif //SUMO_ROBOT_V2_ROBOT_H
