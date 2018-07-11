//
// Created by josh on 7/7/18.
//
#include "Robot.h"
#include "Sensors.h"
#include "RobotProcess.h"
#include "DriveProcesses.h"

String currentLine1 = "";
String currentLine2 = "";
RobotProcess *currentProcess;
Zumo32U4LCD lcd;

void initRobot(){
	initSensors();
	setRobotHeading(90);
	RobotProcessBuilder builder;
//	builder.append(new TankDistanceDrive(.2, 10, 10));
//	builder.append(new TurnToHeading(-90, .2));
//	builder.append(new TankDistanceDrive(.2, 10, 10));
//	builder.append(new TurnToHeading(90, 1));
//	builder.append(new TankDistanceDrive(.1, 5, 5));
	builder.append(new HeadingDrive(180, .3, 40));
	currentProcess = builder.getFirstProcess();
}
void update() {
	updateSensors();
	if(currentProcess != nullptr){
		currentProcess->update();
		if(currentProcess->isDone()){
			currentProcess->end();
			RobotProcess *nextProcess = currentProcess->getNextProcess();
			delete currentProcess;
			currentProcess = nextProcess;
		}
	}
}
void debug(String line1, String line2) {
	if(currentLine1 != line1 || currentLine2 != line2) {
		lcd.clear();
		lcd.gotoXY(0, 0);
		lcd.print(line1);
		currentLine1 = line1;

		lcd.gotoXY(0, 1);
		lcd.print(line2);
		currentLine2 = line2;

//		Serial.println("updating lines");
	}
}
void throwError(String line1, String line2){
	debug(line1, line2);
	while(1){
		Serial.println(line1);
		Serial.println(line2);
		Serial.println();
		delay(1000);
	}
}
