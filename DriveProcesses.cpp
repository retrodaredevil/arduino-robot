//
// Created by josh on 7/9/18.
//

#include "DriveProcesses.h"
#include <Arduino.h>
#include "TankDrive.h"
#include "Util.h"
#include "Sensors.h"
#include "Robot.h"

//region BasicTankDrive

void BasicTankDrive::setSpeeds(double left, double right) {
	setLeftSpeed(left);
	setRightSpeed(right);
}
void BasicTankDrive::addSpeedMultiplier(double left, double right) {
	setLeftSpeed(getLeftSpeed() * left);
	setRightSpeed(getRightSpeed() * right);
}
void BasicTankDrive::addSpeedMultiplier(double multiplier) {
	addSpeedMultiplier(multiplier, multiplier);
}
double BasicTankDrive::getTurnAmount() {
	double turnAmount;
	getSpeedDriveValues(getLeftSpeed(), getRightSpeed(), &turnAmount, nullptr);
	return turnAmount;
}
double BasicTankDrive::getHighSpeed() {
	double left = getLeftSpeed();
	double right = getRightSpeed();
	return abs(left) > abs(right) ? left : right;
}
void BasicTankDrive::setTurnAmount(double turnAmount) {
	double left, right;
	getTankDriveValues(getHighSpeed(), turnAmount, &left, &right);
	setSpeeds(left, right);
}
void BasicTankDrive::setHighSpeed(double speed) {
	double left = getLeftSpeed();
	double right = getRightSpeed();
	if(left > right){
		right = (right / left) * speed;
		left = speed;
	} else {
		left = (left / right) * speed;
		right = speed;
	}
	setSpeeds(left, right);
}
//endregion

//region BasicSpeedDrive

double BasicSpeedDrive::getLeftSpeed() {
	double speed = getHighSpeed();
	double turnAmount = getTurnAmount();
	double left;
	getTankDriveValues(speed, turnAmount, &left, nullptr);
	return left;
}
double BasicSpeedDrive::getRightSpeed() {
	double speed = getHighSpeed();
	double turnAmount = getTurnAmount();
	double right;
	getTankDriveValues(speed, turnAmount, nullptr, &right);
	return right;
}
void BasicSpeedDrive::setLeftSpeed(double left) {
	double right = getRightSpeed();
	double speed, turnAmount;
	getSpeedDriveValues(left, right, &speed, &turnAmount);
	setHighSpeed(speed);
	setTurnAmount(turnAmount);
}
void BasicSpeedDrive::setRightSpeed(double right) {
	double left = getLeftSpeed();
	double speed, turnAmount;
	getSpeedDriveValues(left, right, &speed, &turnAmount);
	setHighSpeed(speed);
	setTurnAmount(turnAmount);
}
void BasicSpeedDrive::setSpeeds(double left, double right) {
	double speed, turnAmount;
	getSpeedDriveValues(left, right, &speed, &turnAmount);
	setHighSpeed(speed);
	setTurnAmount(turnAmount);
}
void BasicSpeedDrive::addSpeedMultiplier(double multiplier) {
	setHighSpeed(getHighSpeed() * multiplier);
}
void BasicSpeedDrive::addSpeedMultiplier(double left, double right) {
	double leftSpeed, rightSpeed;
	getTankDriveValues(getHighSpeed(), getTurnAmount(), &leftSpeed, &rightSpeed);
	setSpeeds(leftSpeed * left, rightSpeed * right);
}
//endregion

//region TankDistanceDrive

TankDistanceDrive::TankDistanceDrive(double speed, double leftDistance, double rightDistance, bool normalizeLeftAndRight)
		: speed(speed), leftDistance(leftDistance), rightDistance(rightDistance), normalizeLeftAndRight(normalizeLeftAndRight),
		  SimpleRobotProcess(true) {
}
TankDistanceDrive::~TankDistanceDrive() {}
void TankDistanceDrive::onStart() {
	leftStart = getLeftDriveDistance();
	rightStart = getRightDriveDistance();
}
void TankDistanceDrive::onUpdate() {
	const int leftDirection = (leftDistance < 0 ? -1 : 1); // -1 or 1
	const int rightDirection = (rightDistance < 0 ? -1 : 1); // -1 or 1

	const bool leftDone = getLeftDistanceAway() < 0;
	const bool rightDone = getRightDistanceAway() < 0;

	if(normalizeLeftAndRight){
		const double leftDistanceGone = getLeftDistanceGone();
		const double rightDistanceGone = getRightDistanceGone();
		const double estimatedDistanceAhead = 1.7 * speed;
		if(abs(leftDistance) > abs(rightDistance)){ // base right speed off of how far left motor has gone (turn right)
			leftMotorSpeed = speed * leftDirection;
			if(rightDistanceGone <= 0){ // if we are at position 0 or behind where we want to go, full speed in correct direction
				rightMotorSpeed = speed * rightDirection;
			} else {
				const double rightToLeft = abs(rightDistance / leftDistance);
				const double desiredRightDistance = leftDistanceGone * rightToLeft;
				rightMotorSpeed = speed * (desiredRightDistance / (rightDistanceGone + estimatedDistanceAhead)) * rightDirection;
			}
		} else { // base left speed off of how far right motor has gone (turn left or go straight)
			if(leftDistanceGone <= 0){ // if we are at position 0 or behind where we want to go, full speed in correct direction
				leftMotorSpeed = speed * leftDirection;
			} else {
				const double leftToRight = abs(leftDistance / rightDistance);
				const double desiredLeftDistance = rightDistanceGone * leftToRight;
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
}

double TankDistanceDrive::getLeftSpeed() {
	return leftMotorSpeed;
}
double TankDistanceDrive::getRightSpeed() {
	return rightMotorSpeed;
}
void TankDistanceDrive::setLeftSpeed(double left) {
	leftMotorSpeed = left;
}
void TankDistanceDrive::setRightSpeed(double right) {
	rightMotorSpeed = right;
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
//endregion

//region TurnToHeading

TurnToHeading::TurnToHeading(double heading, double speed) : heading(heading), speed(speed),
                                                             SimpleRobotProcess(true) {}
TurnToHeading::~TurnToHeading() {}

void TurnToHeading::onStart() {}
void TurnToHeading::onUpdate() {
	double change = minChange(heading, getRobotHeading(), 360);
	if(abs(change) < DONE_DEADBAND_DEGREES){
		setDone();
		return;
	}
	speedDrive(speed, 2 * (change < 0 ? 1 : -1));
}
void TurnToHeading::onLateUpdate() {}
void TurnToHeading::onEnd(bool wasPeaceful) {
}
//endregion

//region HeadingDrive

HeadingDrive::HeadingDrive(double heading, double speed, double distance): heading(heading), speed(speed), distance(distance), SimpleRobotProcess(true) {}
HeadingDrive::~HeadingDrive() {}
void HeadingDrive::onStart() {
	leftStart = getLeftDriveDistance();
	rightStart = getRightDriveDistance();
}
void HeadingDrive::onUpdate() {
	debug(String(heading), F("degrees"));
	double change = minChange(heading, getRobotHeading(), 360);
	turnAmount = change / -10.0;
	turnAmount = max(min(turnAmount, 2), -2);
	highSpeed = speed;
	if(getDistanceAway() <= 0){
		setDone();
	}
}
void HeadingDrive::onLateUpdate() {
	speedDrive(highSpeed, turnAmount);
}
void HeadingDrive::onEnd(bool wasPeaceful) {
}
double HeadingDrive::getHighSpeed() {
	return highSpeed;
}
double HeadingDrive::getTurnAmount() {
	return turnAmount;
}
void HeadingDrive::setHighSpeed(double speed) {
	this->highSpeed = speed;
}
void HeadingDrive::setTurnAmount(double turnAmount) {
	this->turnAmount = turnAmount;
}

double HeadingDrive::getDistanceAway() {
	return distance - getDistanceGone();
}
double HeadingDrive::getDistanceGone() {
	return (getLeftDriveDistance() - leftStart + getRightDriveDistance() - rightStart) / 2.0 ;
}
double HeadingDrive::getLeftDistanceAway() { return getDistanceAway(); } // TODO provide actual estimates for left and right distances
double HeadingDrive::getRightDistanceAway() { return getDistanceAway(); }

double HeadingDrive::getLeftDistanceGone() { return getDistanceGone(); }
double HeadingDrive::getRightDistanceGone() { return getDistanceGone(); }

//endregion