#include <Arduino.h>
#include <Zumo32U4.h>
#include <Wire.h>

#include "Robot.h"
#include "Sensors.h"
#include "RobotProcess.h"
#include "DriveProcesses.h"
#include "DriveListeners.h"

void setup() {
	delay(1000);
	Serial.begin(9600);
	Wire.begin();
	initRobot();
	debug(F("Starting"), F("Soon"));
	delay(1000); // wait for Serial to connect if it's going to at all
	debug(F("Now"), F("Started!"));
	Serial.println("now started");
	RobotProcessBuilder builder;
//	builder.append(new TankDistanceDrive(.2, 10, 10));
//	builder.append(new TurnToHeading(-90, .2));
//	builder.append(new TankDistanceDrive(.2, 10, 10));
//	builder.append(new TurnToHeading(90, 1));
//	builder.append(new TankDistanceDrive(.1, 5, 5));
	const double distance = 25;
	HeadingDrive *up = new HeadingDrive(90, 1, distance);
	builder.append(up);
	builder.append(new HeadingDrive(0, 1, distance));
	builder.append(new HeadingDrive(-90, 1, distance));
	HeadingDrive *left = new HeadingDrive(180, 1, distance);
	left->setNextProcess(up); // loop forever
	builder.append(left);
	setRobotProcess(builder.getFirstProcess());
}

void loop() {
//	throwError(F("Stops"), F("Program"));
	update();
//	Serial.print((int) robot.getDrive()->getLeftDistance());
//	Serial.print(F("\t"));
//	Serial.print((int) robot.getDrive()->getRightDistance());
//	int32_t x = getAccelerometerX(), y = getAccelerometerY(), z = getAccelerometerZ();

//	Serial.print((int) getAccelerometerX());
//	Serial.print(F("\t"));
//	Serial.print((int) getAccelerometerY());
//	Serial.print(F("\t"));
//	Serial.print((int) getAccelerometerZ());
//	Serial.println();
//
//	Serial.print((int) isLeftOnLine());
//	Serial.print(F("\t"));
//	Serial.print((int) isCenterOnLine());
//	Serial.print(F("\t"));
//	Serial.print((int) isRightOnLine());
//	Serial.println();
//	Serial.println(getRobotHeading());

	delay(10);
}
