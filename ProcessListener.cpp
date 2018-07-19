//
// Created by josh on 7/9/18.
//

#include "ProcessListener.h"
#include "Robot.h"

SimpleProcessListener::SimpleProcessListener(bool removeAfterEnd) : removeAfterEnd(removeAfterEnd){
}
void SimpleProcessListener::setDone(bool done) {
	this->done = done;
}

void SimpleProcessListener::init(RobotProcess *robotProcess) {
	if(this->robotProcess != nullptr){
		throwError("process", "is null");
	}
	this->robotProcess = robotProcess;
}

void SimpleProcessListener::update() {
	if(!started){
		done = false;
		started = true;
		onStart();
	}
	onUpdate();
}
void SimpleProcessListener::processEnd() {
	onProcessEnd();
	started = false;
	hasEndedAtLeastOneTime = true;
}
bool SimpleProcessListener::isDone() {
	return done || (hasEndedAtLeastOneTime && removeAfterEnd);
}
