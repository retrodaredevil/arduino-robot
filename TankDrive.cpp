//
// Created by josh on 7/7/18.
//
#include "TankDrive.h"

#include <Zumo32U4.h>

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
//	if(abs(turnAmount) > 1){ don't need this because already accounts for it below
//		if(turnAmount < 0){ // turning left - right full motor
//			tankDrive(speed * (1 + turnAmount), speed);
//		} else {
//			tankDrive(speed, speed * (1 - turnAmount));
//		}
//		return;
//	}
	tankDrive(
			speed * (1 - (turnAmount < 0 ? -turnAmount : 0)),
			speed * (1 - (turnAmount > 0 ?  turnAmount : 0))
	);
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
