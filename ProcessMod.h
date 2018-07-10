//
// Created by josh on 7/9/18.
//

#ifndef SUMO_ROBOT_V2_PROCESSMOD_H
#define SUMO_ROBOT_V2_PROCESSMOD_H

class ProcessMod;
#include "RobotProcess.h"

/**
 * A ProcessMod usually belongs to a RobotProcess. The RobotProcess should update all of its "mods" when it updates
 */
class ProcessMod{
public:
	/**
	 * This method is called after a RobotProcess is updated. This method may call methods of the RobotProcess that owns
	 * it and is even allowed to call setDone().
	 * <br/>
	 * When this method is called, the RobotProcess that has this ProcessMod should be updated enough to be able to
	 * change things like speed, etc, but may later after this method is called use the state this method changed
	 * (or didn't change) to actually do something like set the speed of motors.
	 */
	virtual void update(RobotProcess *robotProcess)=0;
	/**
	 * This is called when the RobotProcess this is attached to ends. This should be used to possibly call setNextProcess()
	 * and change it but still preserve what getNextProcess() returns.
	 */
	virtual void end(RobotProcess *robotProcess)=0;

	/**
	 * This is called and checked frequently when the RobotProcess is being updated. This can also be used
	 * to remove this ProcessMod after end is called (this is recommended) although, note that after end is called,
	 * it's likely that this method won't be called for some time.
	 * @return true if this ProcessMod is done and should be removed from the RobotProcess
	 */
	virtual bool isDone()=0;
};
class SimpleProcessMod : public ProcessMod{
private:
	const bool removeAfterEnd;
	bool started = false;
	bool done = false;
	bool hasEndedAtLeastOneTime = false; // only set to true never set back to false
protected:
	SimpleProcessMod(bool removeAfterEnd = true);
public:
	void update(RobotProcess *robotProcess) override;
	virtual void onStart(RobotProcess *robotProcess);
	virtual void onUpdate(RobotProcess *robotProcess)=0;
	void end(RobotProcess *robotProcess) override;
	virtual void onEnd(RobotProcess *robotProcess)=0;
	bool isDone() override;
};

#endif //SUMO_ROBOT_V2_PROCESSMOD_H
