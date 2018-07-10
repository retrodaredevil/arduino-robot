//
// Created by josh on 7/7/18.
//

#include "Sensors.h"

const double POW2_16 = pow(2, 16);

Zumo32U4LineSensors lineSensors;
Zumo32U4ProximitySensors proximitySensors;
LSM303 compass;
L3G gyro;

// for line sensor
unsigned int *lineSensorValues = nullptr; // left, center, right
// variables for proximity sensors
uint8_t frontLeftProximity;
uint8_t frontRightProximity;
uint8_t leftProximity;
uint8_t rightProximity;
// variables for accelerometer
double accelX, accelY, accelZ;

// Variables for the heading
uint32_t turnAngle = 0;
int16_t gyroOffset;
uint32_t gyroLastUpdate = 0;

bool initGyro();
void updateGyro();
void resetGyro();

bool initSensors() {
	bool r = true;
	lineSensors.initThreeSensors();
	proximitySensors.initThreeSensors();

	r = r && compass.init();
	compass.enableDefault();

	r = r && initGyro();
	return r;
}
void updateSensors() {
	unsigned int tempSensorValues[3];
	lineSensors.read(tempSensorValues);
	lineSensorValues = tempSensorValues;

	proximitySensors.read();
	frontLeftProximity = proximitySensors.countsFrontWithLeftLeds();
	frontRightProximity = proximitySensors.countsFrontWithRightLeds();
	leftProximity = proximitySensors.countsLeftWithLeftLeds();
	rightProximity = proximitySensors.countsRightWithRightLeds();

	compass.read();
	accelX = compass.a.x; // forward and backwards
	accelY = compass.a.y; // side to side
	accelZ = compass.a.z; // up and down

	updateGyro();

}
bool initGyro(){
	const uint32_t CALIBRATION_ITERATION_TIMES = 1024;
	// all credit goes to: https://github.com/pololu/zumo-32u4-arduino-library/blob/master/examples/MazeSolver/TurnSensor.cpp
	if(!gyro.init()){
		return false;
	}

	// 800 Hz output data rate,
	// low-pass filter cutoff 100 Hz
	gyro.writeReg(L3G::CTRL1, 0b11111111);

	// 2000 dps full scale
	gyro.writeReg(L3G::CTRL4, 0b00100000);

	// High-pass filter disabled
	gyro.writeReg(L3G::CTRL5, 0b00000000);

	// Calibrate the gyro.
	int32_t total = 0;
	for (uint16_t i = 0; i < CALIBRATION_ITERATION_TIMES; i++) {
		// Wait for new data to be available, then read it.
		while(!gyro.readReg(L3G::STATUS_REG) & 0x08);
		gyro.read();

		// Add the Z axis reading to the total.
		total += gyro.g.z;
	}
	gyroOffset = total / CALIBRATION_ITERATION_TIMES;

	resetGyro();
}
void resetGyro(){
	gyroLastUpdate = micros();
	turnAngle = 0;
}
void updateGyro(){
	gyro.read();
	int16_t turnRate = gyro.g.z - gyroOffset;

	// Figure out how much time has passed since the last update (dt)
	uint32_t m = micros();
	uint16_t dt = m - gyroLastUpdate;
	gyroLastUpdate = m;

	// Multiply dt by turnRate in order to get an estimation of how
	// much the robot has turned since the last update.
	// (angular change = angular velocity * time)
	int32_t d = (int32_t)turnRate * dt;

	// The units of d are gyro digits times microseconds.	We need
	// to convert those to the units of turnAngle, where 2^29 units
	// represents 45 degrees.	The conversion from gyro digits to
	// degrees per second (dps) is determined by the sensitivity of
	// the gyro: 0.07 degrees per second per digit.
	//
	// (0.07 dps/digit) * (1/1000000 s/us) * (2^29/45 unit/degree)
	// = 14680064/17578125 unit/(digit*us)
	turnAngle += (int64_t)d * 14680064 / 17578125;
}
double getRobotHeading(){
//	return (((int32_t)turnAngle >> 16) * 360) >> 16;
	return (((int32_t)turnAngle / POW2_16) * 360.0) / POW2_16;
}

uint8_t getFrontLeftProximity() { return frontLeftProximity; }
uint8_t getFrontRightProximity() { return frontRightProximity; }
uint8_t getLeftProximity() { return leftProximity; }
uint8_t getRightProximity() { return rightProximity; }

int32_t getAccelerometerX() { return accelX; }
int32_t getAccelerometerY() { return accelY; }
int32_t getAccelerometerZ() { return accelZ; }

// NOTE: Should work for a duck tape board with masking tape around outside and on perfect black and white boards as well
bool isLeftOnLine()   { return lineSensorValues[0] < 400; }
bool isCenterOnLine() { return lineSensorValues[1] < 300; }
bool isRightOnLine()  { return lineSensorValues[2] < 400; }
