//
// Created by josh on 7/7/18.
//
#include "Robot.h"
#include "Sensors.h"
#include "DriveProcesses.h"

String currentLine1 = "";
String currentLine2 = "";
RobotProcess *currentProcess;
Zumo32U4LCD lcd;

void initRobot(){
	initSensors();
	setRobotHeading(90);
}
void setRobotProcess(RobotProcess *robotProcess){
	delete currentProcess;
	currentProcess = robotProcess;
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
