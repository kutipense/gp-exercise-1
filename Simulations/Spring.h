#pragma once

#include "MassPoint.h"
#include "util/vectorbase.h"
#include "DrawingUtilitiesClass.h"
#include <memory>

using GamePhysics::Vec3;
using namespace std;

class Spring
{
public:
	Spring(float stiffness, float restLength) :
		stiffness_(stiffness),
		restLength_(restLength),
		currLength_(0) {}

	void setStiffness(float stiffness) { stiffness_ = stiffness; }
	void setRestLength(float restLength) { restLength_ = restLength; }

	double getLength();

	void init(shared_ptr<MassPoint> m1, shared_ptr<MassPoint> m2);

	void applyForceToMasses();

	void draw(DrawingUtilitiesClass* DUC);

private:
	shared_ptr<MassPoint> m1_;
	shared_ptr<MassPoint> m2_;

	Vec3 direction_; // m1 to m2

	float stiffness_;
	float restLength_;
	float currLength_;
};

