//
// Created by josh on 7/10/18.
//

#ifndef ARDUINO_ROBOT_V2_UTIL_H
#define ARDUINO_ROBOT_V2_UTIL_H

#include <Arduino.h>

template <class T>
struct Node{
	const T element;
	Node<T> *next = nullptr;

	Node<T>(T element);
	~Node<T>();
	void add(T element);
};
template <class T>
Node<T>::Node(T element): element(element) {}
template <class T>
Node<T>::~Node(){

}
template <class T>
void Node<T>::add(T element){
	Node<T> *last = this;
	while(last->next != nullptr){
		last = last->next;
	}
	last->next = new Node<T>(element);
}

/**
 *
 * @param a Number on the left of the MOD
 * @param b Number on the right of the MOD
 * @return The modulus of a MOD b where the return value is not negative
 */
double mod(double a, double b);

/**
 * returns a - b or, when |a - b| > wrap / 2, it finds a quicker way
 * <br/> <br/>
 * minChange(1, 5, 4) == 0 <br/>
 * minChange(1, 5, 5) == 1 <br/>
 * minChange(5, 1, 5) == -1 <br/>
 * <br/>
 * minChange(30, 300, 360) == 90 <br/>
 * minChange(180, 0, 360) == 180 <br/>
 * minChange(181, 0, 360) == -179 <br/>
 * @param a Usually the desired variable to get to
 * @param b Usually the current variable to change
 * @param wrap The number that it "wraps" at. Ex: if wrap is 10, 2 is the same as 12
 * @return A positive or negative number that if added to b is the smallest change to get to a.
 */
double minChange(double a, double b, double wrap);
#endif //ARDUINO_ROBOT_V2_UTIL_H
