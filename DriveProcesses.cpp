//
// Created by josh on 7/9/18.
//

#include "DriveProcesses.h"
#include "TankDrive.h"

TankDistanceDrive::TankDistanceDrive(double speed, double leftDistance, double rightDistance)
		: speed(speed), leftDistance(leftDistance), rightDistance(rightDistance) {
}
TankDistanceDrive::~TankDistanceDrive() {

}
void TankDistanceDrive::onStart() {
	leftStart = getLeftDriveDistance();
	rightStart = getRightDriveDistance();
}
void TankDistanceDrive::onUpdate() {
	bool leftDone = getLeftDistanceAway() < 0;
	bool rightDone = getRightDistanceAway() < 0;
	if (!leftDone) {
		leftSpeed = (leftDistance < 0 ? -1 : 1) * speed;
	} else {
		leftSpeed = 0;
	}
	if(!rightDone) {
		rightSpeed = (rightDistance < 0 ? -1 : 1) * speed;
	} else {
		rightStart = 0;
	}
	if(leftDone && rightDone){
		setDone();
	}
}
void TankDistanceDrive::onLateUpdate() {
	tankDrive(leftSpeed, rightSpeed);
}
void TankDistanceDrive::onEnd(bool wasPeaceful) {
	tankDrive(0, 0); // TODO Make an optional ProcessMod to do this
}

double TankDistanceDrive::getLeftSpeed() {
	return leftSpeed;
}
double TankDistanceDrive::getRightSpeed() {
	return rightSpeed;
}
void TankDistanceDrive::addSpeedMultiplier(double left, double right) {
	leftSpeed *= left;
	rightSpeed *= right;
}
double TankDistanceDrive::getLeftDistanceAway() {
	return (leftDistance < 0 ? -1 : 1)
	       * ((leftStart + leftDistance) - getLeftDriveDistance());
}
double TankDistanceDrive::getRightDistanceAway() {
	return (rightDistance < 0 ? -1 : 1)
	       * ((rightStart + rightDistance) - getRightDriveDistance());
}