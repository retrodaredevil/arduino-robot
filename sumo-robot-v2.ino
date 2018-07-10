#include <Arduino.h>
#include <Zumo32U4.h>
#include <Wire.h>

#include "Robot.h"
#include "Sensors.h"

void setup() {
	delay(1000);
	Serial.begin(9600);
	Wire.begin();
	initRobot();
	debug(F("Starting"), F("Soon"));
	delay(1000); // wait for Serial to connect if it's going to at all
	debug(F("Now"), F("Started!"));
}

void loop() {
	debug(F("Looping"), String(millis() / 1000));
	update();
//	Serial.print((int) robot.getDrive()->getLeftDistance());
//	Serial.print(F("\t"));
//	Serial.print((int) robot.getDrive()->getRightDistance());

	Serial.print((int) getAccelerometerX());
	Serial.print(F("\t"));
	Serial.print((int) getAccelerometerY());
	Serial.print(F("\t"));
	Serial.print((int) getAccelerometerZ());
	Serial.println();

	Serial.print((int) isLeftOnLine());
	Serial.print(F("\t"));
	Serial.print((int) isCenterOnLine());
	Serial.print(F("\t"));
	Serial.print((int) isRightOnLine());
	Serial.println();
	Serial.println(getRobotHeading());

	delay(20);
}
