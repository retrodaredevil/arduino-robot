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
	const double leftDistance, rightDistance;
	const double speed;
	double leftStart, rightStart;
	double leftSpeed, rightSpeed;
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
	 */
	TankDistanceDrive(double speed, double leftDistance, double rightDistance);
	~TankDistanceDrive();
	double getLeftSpeed() override;
	double getRightSpeed() override;
	void addSpeedMultiplier(double left, double right) override;

	double getLeftDistanceAway() override;
	double getRightDistanceAway() override;
};

#endif //SUMO_ROBOT_V2_DRIVEPROCESSES_H
