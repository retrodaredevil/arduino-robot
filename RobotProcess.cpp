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
	delete firstListenerNode;
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
	Node<ProcessListener*> *lastNode = nullptr;
	for(Node<ProcessListener*> *listenerNode = firstListenerNode; listenerNode != nullptr; listenerNode = listenerNode->next){
		ProcessListener *listener = listenerNode->element;
		listener->update();
		Serial.println("updated listener");
		if(listener->isDone()){
			// remove the node
			Serial.println("removing listener");
			if(lastNode == nullptr){
				this->firstListenerNode = listenerNode->next;
			} else {
				lastNode->next = listenerNode->next;
			}
		}
		lastNode = listenerNode;
	}
	onLateUpdate();
}
void SimpleRobotProcess::end() {
	onEnd(done); // we ended peacefully if done is true
	for(Node<ProcessListener*> *listenerNode = firstListenerNode; listenerNode != nullptr; listenerNode = listenerNode->next){
		listenerNode->element->processEnd();
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
RobotProcess* SimpleRobotProcess::addProcessListener(ProcessListener *processListener) {
//	Serial.println("adding process listener");
	if(processListener == nullptr){
		Serial.println("processListener is null");
		throwError("listener", "null");
	}
	if(firstListenerNode == nullptr){
		firstListenerNode = new Node<ProcessListener*>(processListener);
		if(firstListenerNode->element == nullptr){
			Serial.println("null for some reason idk.");
			throwError("null 4 a", "reason");
		}
	} else {
		firstListenerNode->add(processListener);
	}
//	Serial.println("initializing");
	processListener->init(this);
//	Serial.println("done about to return. (good)");
	return this;
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