//
// Created by josh on 7/9/18.
//

#ifndef SUMO_ROBOT_V2_DRIVEPROCESSES_H
#define SUMO_ROBOT_V2_DRIVEPROCESSES_H

#include "RobotProcess.h"

/**
 * A really basic RobotProcess used when driving
 */
struct DriveProcess : virtual public RobotProcess{
	virtual double getLeftSpeed()=0;
	virtual double getRightSpeed()=0;

	/**
	 * Adds a speed multiplier where a value of 1 has no effect.
	 * @param left The value to multiply the left motor speed by
	 * @param right The value to multiply the right motor speed by
	 */
	virtual void addSpeedMultiplier(double left, double right)=0;
};
/**
 * A basic drive process that has a target distance away or an estimated distance away
 */
struct DistanceDriveProcess : virtual public DriveProcess {
	/**
	 * @return The left distance away this is from the desired distance. This could be < 0 if it has gone past it
	 */
	virtual double getLeftDistanceAway()=0;
	/**
	 * @return The right distance away this is from the desired distance. This could be < 0 if it has gone past it
	 */
	virtual double getRightDistanceAway()=0;
};

class TankDistanceDrive : public SimpleRobotProcess, public DistanceDriveProcess{
private:
	const double speed;
	const double leftDistance, rightDistance;
	const bool normalizeLeftAndRight;
	double leftStart, rightStart; // set once when onStart() is called
	double leftMotorSpeed, rightMotorSpeed; // set every call to onUpdate(), used in onLateUpdate()

	double getLeftDistanceGone();
	double getRightDistanceGone();
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
	void addSpeedMultiplier(double left, double right) override;

	double getLeftDistanceAway() override;
	double getRightDistanceAway() override;
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
class HeadingDrive : public SimpleRobotProcess {
private:
	const double heading;
	const double speed;
	const double distance;
	double leftStart, rightStart;
protected:
	void onStart() override;
	void onUpdate() override;
	void onLateUpdate() override;
	void onEnd(bool wasPeaceful) override;
public:
	HeadingDrive(double heading, double speed, double distance);
	~HeadingDrive();
};

#endif //SUMO_ROBOT_V2_DRIVEPROCESSES_H
