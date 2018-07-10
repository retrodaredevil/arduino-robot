//
// Created by josh on 7/9/18.
//

#include "RobotProcess.h"
#include "Robot.h"

#include <Arduino.h>

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
	delete modsLinkedList;
}
void SimpleRobotProcess::update() {
	if(!started){
		if(hasEndedAtLeastOnce && !canRecycle){
			throwError(F("starting"), F("again")); // should put program in infinite loop
		}
		startMillis = millis();
		started = true;
		onStart();
	}
	onUpdate();
	for(Node<ProcessMod*> *mod = modsLinkedList; mod != nullptr; mod = mod->next){
		mod->element->update(this);
	}
	onLateUpdate();
}
void SimpleRobotProcess::end() {
	onEnd(done); // we ended peacefully if done is true
	for(Node<ProcessMod*> *mod = modsLinkedList; mod != nullptr; mod = mod->next){
		mod->element->end(this);
	}
	started = false;
	hasEndedAtLeastOnce = true;
}
bool SimpleRobotProcess::isDone() {
	return done;
}
void SimpleRobotProcess::setDone(bool done) {
	this->done = done;
}
void SimpleRobotProcess::addProcessMod(ProcessMod *processMod) {
	modsLinkedList->add(processMod);
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

SimpleTimedRobotProcess::SimpleTimedRobotProcess(long timeToLast) : timeToLast(timeToLast) {}
long SimpleTimedRobotProcess::getTimeLeft() {
	return timeToLast - getProcessTime();
}