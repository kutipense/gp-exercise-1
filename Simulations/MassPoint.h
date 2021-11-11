#pragma once

#include "util/vectorbase.h"
#include "DrawingUtilitiesClass.h"


using GamePhysics::Vec3;

struct MassPoint
{
	MassPoint() : mass_(0), fixed_(false) {}
	MassPoint(float mass) : mass_(mass), fixed_(false) {}

	void setPosition(double x, double y, double z) { position_ = Vec3(x, y, z); }
	void setVelocity(double x, double y, double z) { velocity_ = Vec3(x, y, z); }
	void setPosition(Vec3 p) { position_ = p; }
	void setVelocity(Vec3 v) { velocity_ = v; }
	void setFixed(bool fixed) { fixed_ = fixed; }

	Vec3& getPosition() { return position_; }
	Vec3& getVelocity() { return velocity_; }
	float getMass() { return mass_; }

	double distTo(MassPoint& other);
	Vec3 directionTo(MassPoint& other);

	void addForce(Vec3& force);
	void addGravity(float factor = 10.0f);

	void update(float timeStep, int integrator, bool _addGravity = false);

	void draw(DrawingUtilitiesClass* DUC);

private:
	void eulerUpdate(float timeStep);
	void fullStepUpdate(float timeStep);

	Vec3 position_, oldPosition_;
	Vec3 velocity_, oldVelocity_;
	Vec3 forces_;
	
	float mass_;
	bool fixed_;
};

