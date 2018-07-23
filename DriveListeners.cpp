//
// Created by josh on 7/17/18.
//

#include "DriveListeners.h"

#include "Robot.h"
#include "TankDrive.h"

SideSensorGetaway::SideSensorGetaway(DriveProcess *driveProcess) : driveProcess(driveProcess) {}
SideSensorGetaway::~SideSensorGetaway() {}
void SideSensorGetaway::init(RobotProcess *robotProcess) {
	SimpleProcessListener::init(robotProcess);
	if(driveProcess != robotProcess) {
		throwError("not same", "dprocess");
	}
}
void SideSensorGetaway::onStart() {}
void SideSensorGetaway::onUpdate() {
}
void SideSensorGetaway::onProcessEnd() {}


InstantStop::InstantStop() {}
InstantStop::~InstantStop() {}
void InstantStop::onStart() {}
void InstantStop::onUpdate() {}
void InstantStop::onProcessEnd() { tankDrive(0, 0); }