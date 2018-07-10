//
// Created by josh on 7/7/18.
//

#ifndef SUMO_ROBOT_V2_ROBOTPROCESS_H
#define SUMO_ROBOT_V2_ROBOTPROCESS_H
class RobotProcess;
#include "ProcessMod.h"

class RobotProcess{
protected:
	RobotProcess(){}
public:
	virtual ~RobotProcess()=0;
	virtual void update()=0;
	/**
	 * This method is called when this process is forcefully ended and when it ended because isDone() returned true
	 */
	virtual void end()=0;
	virtual bool isDone()=0;
	virtual void setDone(bool done = true)=0;
	virtual void addProcessMod(ProcessMod *processMod);
	/**
	 *
	 * @param nextProcess The next process.
	 * @return The exact value that was passed: nextProcess (for chaining)
	 */
	virtual RobotProcess* setNextProcess(RobotProcess *nextProcess)=0;
	/**
	 * Can be called whenever you desire to see the next RobotProcess or when isDone() is true, calling this returns
	 * the next RobotProcess that should be set to the current RobotProcess
	 * @return
	 */
	virtual RobotProcess* getNextProcess()=0;
};

class RobotProcessBuilder{
private:
	RobotProcess *firstProcess = nullptr;
	RobotProcess *lastProcess;
public:
	RobotProcessBuilder();
	RobotProcessBuilder* append(RobotProcess *process);
	RobotProcess* getFirstProcess();
};

class SimpleRobotProcess : public RobotProcess {
private:
	const bool canRecycle;
	const long startMillis;
	bool done = false;
	bool hasBeenDoneAtLeastOnce = false; // never set back to false
	bool started = false;
	RobotProcess *nextProcess = nullptr;
	std::list<ProcessMod*> mods;
protected:
	SimpleRobotProcess(bool canRecycle = false, RobotProcess *nextProcess = nullptr);
	virtual void onStart()=0;
	/**
	 * Called before all ProcessMods are updated
	 */
	virtual void onUpdate()=0;
	/**
	 * Called after all ProcessMods are updated
	 */
	virtual void onLateUpdate()=0;
	/**
	 * Called before all ProcessMods are ended
	 * @param wasPeaceful true if the reason this ending was because isDone() is true. (Normally true)
	 */
	virtual void onEnd(bool wasPeaceful)=0;

	long getProcessTime();
public:
	~SimpleRobotProcess() override;
	void update() override;
	void end() override;
	bool isDone() override;
	void setDone(bool done) override;
	void addProcessMod(ProcessMod *processMod) override;
	RobotProcess* setNextProcess(RobotProcess *nextProcess) override;
	RobotProcess* getNextProcess() override;
};
#endif //SUMO_ROBOT_V2_ROBOTPROCESS_H