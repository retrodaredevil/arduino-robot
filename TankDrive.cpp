//
// Created by josh on 7/7/18.
//
#include "TankDrive.h"

#include <Zumo32U4.h>
#include <Arduino.h>

const long double ENCODER_COUNTS_TO_CM = 40.0 / 2900.0;
Zumo32U4Motors motors;
Zumo32U4Encoders encoders;

void tankDrive(double left, double right) {
	double max = abs(max(left, right));
	if(max > 1){
		left /= max;
		right /= max;
	}
	motors.setSpeeds(left * 400, right * 400);
}
void speedDrive(double speed, double turnAmount){
	double leftSpeed, rightSpeed;
	getTankDriveValues(speed, turnAmount, &leftSpeed, &rightSpeed);
	tankDrive(leftSpeed, rightSpeed);
}
void getTankDriveValues(double speed, double turnAmount, double *leftSpeed, double *rightSpeed){
//	*leftSpeed = speed * (1 - (turnAmount < 0 ? -turnAmount : 0));
//	*rightSpeed = speed * (1 - (turnAmount > 0 ?  turnAmount : 0));
	if(leftSpeed != nullptr) *leftSpeed = speed * (1 + min(turnAmount, 0));
	if(rightSpeed != nullptr) *rightSpeed = speed * (1 - max(turnAmount, 0));
}
void getSpeedDriveValues(double left, double right, double *speed, double *turnAmount){
	double speedResult = abs(left) > abs(right) ? left : right;
	if(speed != nullptr) *speed = speedResult;

	if(speedResult == 0){
		if(turnAmount != nullptr) {
			*turnAmount = 0;
		}
		return;
	}
	if(turnAmount != nullptr) {
		if (abs(left) > abs(right)) {
			double a = right / left;
			a += 1;
			*turnAmount = a;
		} else {
			double a = left / right; // 0 / 1 -> -1 // -.5 / 1 -> -1.5 /// -1 / 1 -> -2
			a -= 1;
			*turnAmount = a;
		}
	}
}
int16_t getCountsLeft() {
	return encoders.getCountsLeft();
}
int16_t getCountsRight() {
	return encoders.getCountsRight();
}
double getLeftDriveDistance() {
	return getCountsLeft() * ENCODER_COUNTS_TO_CM;
}
double getRightDriveDistance() {
	return getCountsRight() * ENCODER_COUNTS_TO_CM;
}
