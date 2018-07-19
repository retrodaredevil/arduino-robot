//
// Created by josh on 7/9/18.
//

#ifndef ARDUINO_ROBOT_V2_PROCESSLISTENER_H
#define ARDUINO_ROBOT_V2_PROCESSLISTENER_H

class ProcessListener;
#include "RobotProcess.h"

/**
 * A ProcessListener usually belongs to a RobotProcess. The RobotProcess should update all of its "listeners" when it updates
 * <br/>
 * A ProcessListener can only ever belong to one RobotProcess. Once it is added to a RobotProcess it cannot be added to another
 * or changed to another
 */
class ProcessListener{
public:
	/**
	 * Should only be called one time. When this is called it should be used to pass the RobotProcess that this is added
	 * to.
	 */
	virtual void init(RobotProcess *robotProcess);
	/**
	 * This method is called after a RobotProcess is updated. This method may call methods of the RobotProcess that owns
	 * it and is even allowed to call setDone().
	 * <br/>
	 * When this method is called, the RobotProcess that has this ProcessListener should be updated enough to be able to
	 * change things like speed, etc, but may later after this method is called use the state this method changed
	 * (or didn't change) to actually do something like set the speed of motors.
	 */
	virtual void update()=0;
	/**
	 * This is called when the RobotProcess this is attached to ends. This should be used to possibly call setNextProcess()
	 * and change it but still preserve what getNextProcess() returns.
	 */
	virtual void processEnd()=0;

	/**
	 * This is called and checked frequently when the RobotProcess is being updated. This can also be used
	 * to remove this ProcessListener after end is called (this is recommended) although, note that after end is called,
	 * it's likely that this method won't be called for some time.
	 * @return true if this ProcessListener is done and should be removed from the RobotProcess
	 */
	virtual bool isDone()=0;
};
class SimpleProcessListener : public ProcessListener{
private:
	const bool removeAfterEnd;
	bool started = false;
	bool done = false;
	bool hasEndedAtLeastOneTime = false; // only set to true never set back to false
protected:
	RobotProcess *robotProcess = nullptr; // set ONCE in init
	SimpleProcessListener(bool removeAfterEnd = false);
	void setDone(bool done = true);
public:
	void init(RobotProcess *robotProcess) override;
	void update() override;
	virtual void onStart();
	virtual void onUpdate()=0;
	void processEnd() override;
	virtual void onProcessEnd()=0;
	bool isDone() override;
};

#endif //ARDUINO_ROBOT_V2_PROCESSLISTENER_H
