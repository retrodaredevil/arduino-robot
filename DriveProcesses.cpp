//
// Created by josh on 7/9/18.
//

#include "DriveProcesses.h"
#include <Arduino.h>
#include "TankDrive.h"
#include "Util.h"
#include "Sensors.h"

TankDistanceDrive::TankDistanceDrive(double speed, double leftDistance, double rightDistance, bool normalizeLeftAndRight)
		: speed(speed), leftDistance(leftDistance), rightDistance(rightDistance), normalizeLeftAndRight(normalizeLeftAndRight) {
}
TankDistanceDrive::~TankDistanceDrive() {

}
void TankDistanceDrive::onStart() {
	leftStart = getLeftDriveDistance();
	rightStart = getRightDriveDistance();
//	tankDrive(speed * (leftDistance < 0 ? -1 : 1), speed * (rightDistance < 0 ? -1 : 1)); // start them off at the same speed
}
void TankDistanceDrive::onUpdate() {
	const int leftDirection = (leftDistance < 0 ? -1 : 1); // -1 or 1
	const int rightDirection = (rightDistance < 0 ? -1 : 1); // -1 or 1

	bool leftDone = getLeftDistanceAway() < 0;
	bool rightDone = getRightDistanceAway() < 0;

	if(normalizeLeftAndRight){
		double leftDistanceGone = getLeftDistanceGone();
		double rightDistanceGone = getRightDistanceGone();
		double estimatedDistanceAhead = 1.7 * speed;
		if(abs(leftDistance) > abs(rightDistance)){ // base right speed off of how far left motor has gone (turn right)
			leftMotorSpeed = speed * leftDirection;
			if(rightDistanceGone <= 0){ // if we are at position 0 or behind where we want to go, full speed in correct direction
				rightMotorSpeed = speed * rightDirection;
			} else {
				double rightToLeft = abs(rightDistance / leftDistance);
				double desiredRightDistance = leftDistanceGone * rightToLeft;
				rightMotorSpeed = speed * (desiredRightDistance / (rightDistanceGone + estimatedDistanceAhead)) * rightDirection;
			}
		} else { // base left speed off of how far right motor has gone (turn left or go straight)
			if(leftDistanceGone <= 0){ // if we are at position 0 or behind where we want to go, full speed in correct direction
				leftMotorSpeed = speed * leftDirection;
			} else {
				double leftToRight = abs(leftDistance / rightDistance);
				double desiredLeftDistance = rightDistanceGone * leftToRight;
				leftMotorSpeed = speed * (desiredLeftDistance / (leftDistanceGone + estimatedDistanceAhead)) * leftDirection;
			}
			rightMotorSpeed = speed * rightDirection;
		}
	} else {
		if (!leftDone) {
			leftMotorSpeed = leftDirection * speed;
		} else {
			leftMotorSpeed = 0;
		}
		if (!rightDone) {
			rightMotorSpeed = rightDirection * speed;
		} else {
			rightStart = 0;
		}
	}

	if(leftDone && rightDone){
		setDone();
	}
}
void TankDistanceDrive::onLateUpdate() {
	tankDrive(leftMotorSpeed, rightMotorSpeed);
}
void TankDistanceDrive::onEnd(bool wasPeaceful) {
	tankDrive(0, 0); // TODO Make an optional ProcessListener to do this
}

double TankDistanceDrive::getLeftSpeed() {
	return leftMotorSpeed;
}
double TankDistanceDrive::getRightSpeed() {
	return rightMotorSpeed;
}
void TankDistanceDrive::addSpeedMultiplier(double left, double right) {
	leftMotorSpeed *= left;
	rightMotorSpeed *= right;
}
double TankDistanceDrive::getLeftDistanceGone() {
		return (leftDistance < 0 ? -1 : 1)
	       * (getLeftDriveDistance() - leftStart);
}
double TankDistanceDrive::getRightDistanceGone() {
	return (rightDistance < 0 ? -1 : 1)
	       * (getRightDriveDistance() - rightStart);
}
double TankDistanceDrive::getLeftDistanceAway() {
	return (leftDistance < 0 ? -1 : 1)
	       * ((leftStart + leftDistance) - getLeftDriveDistance());
}
double TankDistanceDrive::getRightDistanceAway() {
	return (rightDistance < 0 ? -1 : 1)
	       * ((rightStart + rightDistance) - getRightDriveDistance());
}

// Turn to Heading
TurnToHeading::TurnToHeading(double heading, double speed, bool relativeToCurrentHeading)
		: headingValue(heading),
		  speed(speed),
		  relativeToCurrentHeading(relativeToCurrentHeading) {}
TurnToHeading::~TurnToHeading() {}

void TurnToHeading::onStart() {
	if(relativeToCurrentHeading){
		gyroHeading = getRobotHeading() + headingValue;
	} else {
		gyroHeading = headingValue;
	}
}
void TurnToHeading::onUpdate() {
	double change = minChange(gyroHeading, getRobotHeading(), 360);
	if(abs(change) < DONE_DEADBAND_DEGREES){
		setDone();
		return;
	}
	speedDrive(speed, 2 * (change < 0 ? 1 : -1));
}
void TurnToHeading::onLateUpdate() {}
void TurnToHeading::onEnd(bool wasPeaceful) {
	tankDrive(0, 0);
}
