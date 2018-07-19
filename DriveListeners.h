//
// Created by josh on 7/17/18.
//

#ifndef ARDUINO_ROBOT_DRIVELISTENERS_H
#define ARDUINO_ROBOT_DRIVELISTENERS_H

#include "ProcessListener.h"
#include "RobotProcess.h"
#include "DriveProcesses.h"

class SideSensorGetaway : public SimpleProcessListener {
private:
	const DriveProcess *driveProcess;
public:
	SideSensorGetaway(DriveProcess *driveProcess);
	~SideSensorGetaway();
	void init(RobotProcess *robotProcess) override; // optional override
	void onStart() override;
	void onUpdate() override;
	void onProcessEnd() override;
};

/**
 * Stops the motor after a process has ended
 */
class InstantStop : public SimpleProcessListener {
public:
	InstantStop();
	~InstantStop();
	void onStart() override;
	void onUpdate() override;
	void onProcessEnd() override;
};

#endif //ARDUINO_ROBOT_DRIVELISTENERS_H
