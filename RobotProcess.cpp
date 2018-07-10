//
// Created by josh on 7/9/18.
//

#include "RobotProcess.h"

// RobotProcessBuilder
RobotProcessBuilder::RobotProcessBuilder() {
}

RobotProcessBuilder* RobotProcessBuilder::append(RobotProcess *process) {
	if(firstProcess == nullptr){
		firstProcess = process;
		lastProcess = process;
		return;
	}
	lastProcess->setNextProcess(process);
	lastProcess = process;
	return this;
}

RobotProcess* RobotProcessBuilder::getFirstProcess() {
	return firstProcess;
}

// SimpleRobotProcess
SimpleRobotProcess::SimpleRobotProcess(bool canRecycle, RobotProcess *nextProcess):
		canRecycle(canRecycle),
		nextProcess(nextProcess){

}
SimpleRobotProcess::~SimpleRobotProcess() {

}
void SimpleRobotProcess::update() {
	if(!started){
		startMillis = millis();
		started = true;
		onStart();
	}
	onUpdate();
	for(ProcessMod *mod : mods){
		mod->update(this);
	}
	onLateUpdate();
}
void SimpleRobotProcess::end() {
	onEnd(done); // we ended peacefully if done is true
}
bool SimpleRobotProcess::isDone() {
	return done;
}
void SimpleRobotProcess::setDone(bool done) {
	this->done = done;
}
void SimpleRobotProcess::addProcessMod(ProcessMod *processMod) {
	mods.push_back(processMod);
}
long SimpleRobotProcess::getProcessTime() {
	return millis() - startMillis;
}
RobotProcess* SimpleRobotProcess::setNextProcess(RobotProcess *nextProcess) {
	this->nextProcess = nextProcess;
	return nextProcess;
}
RobotProcess* SimpleRobotProcess::getNextProcess() {
	return nextProcess;
}
