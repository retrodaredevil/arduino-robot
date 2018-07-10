//
// Created by josh on 7/9/18.
//

#include "ProcessListener.h"

SimpleProcessListener::SimpleProcessListener(bool removeAfterEnd) : removeAfterEnd(removeAfterEnd){
}

void SimpleProcessListener::update(RobotProcess *robotProcess) {
	if(!started){
		done = false;
		started = true;
		onStart(robotProcess);
	}
	onUpdate(robotProcess);
}
void SimpleProcessListener::end(RobotProcess *robotProcess) {
	onEnd(robotProcess);
	started = false;
	done = false;
	hasEndedAtLeastOneTime = true;
}
bool SimpleProcessListener::isDone() {
	return done || (hasEndedAtLeastOneTime && removeAfterEnd);
}
