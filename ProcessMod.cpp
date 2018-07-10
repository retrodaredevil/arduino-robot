//
// Created by josh on 7/9/18.
//

#include "ProcessMod.h"

SimpleProcessMod::SimpleProcessMod(bool removeAfterEnd) : removeAfterEnd(removeAfterEnd){
}

void SimpleProcessMod::update(RobotProcess *robotProcess) {
	if(!started){
		done = false;
		started = true;
		onStart(robotProcess);
	}
	onUpdate(robotProcess);
}
void SimpleProcessMod::end(RobotProcess *robotProcess) {
	onEnd(robotProcess);
	started = false;
	done = false;
	hasEndedAtLeastOneTime = true;
}
bool SimpleProcessMod::isDone() {
	return done || (hasEndedAtLeastOneTime && removeAfterEnd);
}
