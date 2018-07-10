//
// Created by josh on 7/10/18.
//

#ifndef SUMO_ROBOT_V2_UTIL_H
#define SUMO_ROBOT_V2_UTIL_H

template <class T>
struct Node{
	T element;
	Node *next;
	Node(T element) : element(element){
	}
	~Node(){
		delete next;
	}
	void add(T element){
		Node *last = this;
		while(last->next != nullptr){
			last = last->next;
		}
		last->next = new Node(element);
	}
};
#endif //SUMO_ROBOT_V2_UTIL_H
