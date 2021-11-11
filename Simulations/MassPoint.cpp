#include "MassPoint.h"
#include <cmath>
#include <iostream>
using namespace std;

#define EULER 0
#define LEAPFROG 1
#define MIDPOINT 2

double MassPoint::distTo(MassPoint& other)
{
	return std::sqrt(position_.squaredDistanceTo(other.getPosition()));
}

GamePhysics::Vec3 MassPoint::directionTo(MassPoint& other)
{
	return GamePhysics::getNormalized(position_ - other.getPosition());
}

void MassPoint::addForce(Vec3& force) {
	if (!fixed_)
		forces_ += force;
}

void MassPoint::addGravity(float factor) {
	addForce(Vec3(0, -factor * mass_, 0));
}

void MassPoint::eulerUpdate(float timeStep) {
	oldPosition_ = position_;
	oldVelocity_ = velocity_;

	position_ += velocity_ * timeStep;
	velocity_ += forces_ / mass_ * timeStep; // acceleration * timeStep
}

void MassPoint::fullStepUpdate(float timeStep) { // for midpoint
	position_ = oldPosition_ + velocity_ * timeStep;
	velocity_ = oldVelocity_ + forces_ / mass_ * timeStep; // acceleration * timeStep
}

void MassPoint::update(float timeStep, int integrator, bool _addGravity) {
	if (_addGravity) addGravity();

	switch (integrator)
	{
	case EULER: eulerUpdate(timeStep); break;
	case MIDPOINT: fullStepUpdate(timeStep); break;
	default: eulerUpdate(timeStep); break;
	}

	forces_ = Vec3();
}

void MassPoint::draw(DrawingUtilitiesClass* DUC)
{
	DUC->setUpLighting(Vec3(), 0.4 * Vec3(1, 1, 1), 100, 0.6 * Vec3(0, 1, 0));
	DUC->drawSphere(position_, Vec3(1, 1, 1) * 0.05f);
}
