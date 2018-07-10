//
// Created by josh on 7/7/18.
//
#ifndef SUMO_ROBOT_V2_TANKDRIVE_H
#define SUMO_ROBOT_V2_TANKDRIVE_H

/**
 *
 * @param left A number -1 to 1. 1 being full forward, -1 being full back on left motor
 * @param right A number -1 to 1. 1 being full forward, -1 being full back on right motor
 */
void tankDrive(double left, double right);
/**
 * @param speed a number from -1 to 1 which is the speed of the robot
 * @param turnAmount The amount to turn -1 is full right motor(turn left), 1 is full left motor(turn right). 0 is straight.
 * 					-2 is full right motor and full left motor in opposite direction and vice versa for +2
 */
void speedDrive(double speed, double turnAmount);
double getLeftDriveDistance();
double getRightDriveDistance();

#endif //SUMO_ROBOT_V2_TANKDRIVE_H
