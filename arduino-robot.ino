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
	HeadingDrive *drive = new HeadingDrive(-90, 1, 40);
//	drive->addProcessListener(new SideSensorGetaway(drive))
	drive->addProcessListener(new InstantStop());
	builder.append(drive);
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
