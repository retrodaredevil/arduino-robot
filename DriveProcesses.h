//
// Created by josh on 7/9/18.
//

#ifndef ARDUINO_ROBOT_V2_DRIVEPROCESSES_H
#define ARDUINO_ROBOT_V2_DRIVEPROCESSES_H

#include "RobotProcess.h"
#include "TankDrive.h"

/**
 * A really basic RobotProcess used when driving
 */
struct DriveProcess : virtual public RobotProcess{ // interface with default methods
	// abstract methods
	virtual double getLeftSpeed()=0;
	virtual double getRightSpeed()=0;


	virtual void setLeftSpeed(double left)=0;
	virtual void setRightSpeed(double right)=0;

	/**
	 * Adds a speed multiplier where a value of 1 has no effect.
	 * @param left The value to multiply the left motor speed by
	 * @param right The value to multiply the right motor speed by
	 */
	virtual void addSpeedMultiplier(double left, double right)=0;
	virtual void addSpeedMultiplier(double multiplier)=0;

	/**
	 * This method sets each speed. This is not usually the recommended way of doing it.
	 * @param left The left speed
	 * @param right The right speed
	 */
	virtual void setSpeeds(double left, double right)=0;

	virtual double getTurnAmount()=0;
	virtual double getHighSpeed()=0;
	virtual void setTurnAmount(double turnAmount)=0;
	/**
	 * Sets the the motor (left or right) with the highest speed to the passed speed value and scales the other motor
	 * value conserving the rate of turn
	 * <br/>
	 * By default this calls the setSpeeds method
	 * @param speed The speed to set
	 */
	virtual void setHighSpeed(double speed)=0;
};

/**
 * A basic drive process that has a target distance away or an estimated distance away
 */
struct DistanceDriveProcess : virtual public DriveProcess { // interface
	/**
	 * @return The left distance away this is from the desired distance. This could be < 0 if it has gone past it
	 */
	virtual double getLeftDistanceAway()=0;
	/**
	 * @return The right distance away this is from the desired distance. This could be < 0 if it has gone past it
	 */
	virtual double getRightDistanceAway()=0;
	virtual double getDistanceAway(){ return (getLeftDistanceAway() + getRightDistanceAway()) / 2.0; }

	virtual double getLeftDistanceGone()=0;
	virtual double getRightDistanceGone()=0;
	virtual double getDistanceGone(){ return (getLeftDistanceGone() + getRightDistanceGone()) / 2.0; }
};

class BasicTankDrive : virtual public DriveProcess { // interface inheriting interface defining default methods
public:
	double getLeftSpeed()override=0;
	double getRightSpeed()override=0;
	void setLeftSpeed(double left)override=0;
	void setRightSpeed(double right)override=0;

	void addSpeedMultiplier(double left, double right) override;
	void addSpeedMultiplier(double multiplier) override;
	void setSpeeds(double left, double right) override;

	double getTurnAmount() override;
	double getHighSpeed() override;
	void setTurnAmount(double turnAmount) override;
	void setHighSpeed(double speed) override;
};
class BasicSpeedDrive : virtual public DriveProcess {
	double getTurnAmount()override=0;
	double getHighSpeed()override=0;
	void setTurnAmount(double turnAmount)override=0;
	void setHighSpeed(double speed)override=0;

	double getLeftSpeed()override;
	double getRightSpeed()override;
	void setLeftSpeed(double left)override;
	void setRightSpeed(double right)override;

	void addSpeedMultiplier(double left, double right) override;
	void addSpeedMultiplier(double multiplier) override;
	void setSpeeds(double left, double right) override;

};

class TankDistanceDrive : public SimpleRobotProcess, public BasicTankDrive, public DistanceDriveProcess{
private:
	const double speed;
	const double leftDistance, rightDistance;
	const bool normalizeLeftAndRight;
	double leftStart, rightStart; // set once when onStart() is called
	double leftMotorSpeed, rightMotorSpeed; // set every call to onUpdate(), used in onLateUpdate()
protected:
	void onStart() override;
	void onUpdate() override;
	void onLateUpdate() override;
	void onEnd(bool wasPeaceful) override;
public:
	/**
	 * @param speed The magnitude of the speeds for each left and right. This should NEVER be < 0
	 * @param leftDistance The distance for the left motor. This can be < 0
	 * @param rightDistance The distance for the right motor. This can be < 0
	 * @param normalizeLeftAndRight By default true. When true, either the left or right speeds will be decreased to make
	 * 								sure they each end at the same time. (One will go slower than the other)
	 */
	TankDistanceDrive(double speed, double leftDistance, double rightDistance, bool normalizeLeftAndRight = true);
	~TankDistanceDrive();
	double getLeftSpeed() override;
	double getRightSpeed() override;
	void setLeftSpeed(double left) override;
	void setRightSpeed(double right) override;

	double getLeftDistanceAway() override;
	double getRightDistanceAway() override;

	double getLeftDistanceGone() override;
	double getRightDistanceGone() override;
};

class TurnToHeading : public SimpleRobotProcess {
private:
	static const double DONE_DEADBAND_DEGREES = 5; // withing * 2 of this

	const double heading; // value from constructor
	const double speed;

protected:
	void onStart() override;
	void onUpdate() override;
	void onLateUpdate() override;
	void onEnd(bool wasPeaceful) override;
public:
	/**
	 * @param heading The heading relative to the gyro
	 * @param speed The speed; A number from 0 to 1
	 */
	TurnToHeading(double heading, double speed);
	~TurnToHeading();
};
class HeadingDrive : public SimpleRobotProcess, public BasicSpeedDrive, public DistanceDriveProcess {
private:
	const double heading;
	const double speed;
	const double distance;
	double highSpeed, turnAmount; // highSpeed will be == to speed unless changed
	double leftStart, rightStart;
protected:
	void onStart() override;
	void onUpdate() override;
	void onLateUpdate() override;
	void onEnd(bool wasPeaceful) override;
public:
	HeadingDrive(double heading, double speed, double distance);
	~HeadingDrive();

	// BasicSpeedDrive
	double getHighSpeed() override;
	double getTurnAmount() override;
	void setHighSpeed(double speed) override;
	void setTurnAmount(double turnAmount) override;

	// DistanceDriveProcess
	double getLeftDistanceAway() override;
	double getRightDistanceAway() override;
	double getDistanceAway() override;

	double getLeftDistanceGone() override;
	double getRightDistanceGone() override;
	double getDistanceGone() override;

};

#endif //ARDUINO_ROBOT_V2_DRIVEPROCESSES_H
