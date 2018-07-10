//
// Created by josh on 7/10/18.
//

#include "Util.h"

double mod(double a, double b){
	double quotient = a / b; // x.1234
	int rounded = (int) quotient;
	return (quotient - rounded) * b;
}

double minChange(double a, double b, double wrap){
	a = mod(a, wrap);
	b = mod(b, wrap);
	double change = a - b;
	if(abs(change) > wrap / 2.0f){
		if(change < 0){
			change += wrap;
		} else {
			change -= wrap;
		}
	}
	return change;
}
