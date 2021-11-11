#include "Spring.h"

#include <iostream>
using namespace std;

void Spring::init(shared_ptr<MassPoint> m1, shared_ptr<MassPoint> m2) {
	m1_ = m1;
	m2_ = m2;
}

double Spring::getLength() { return m1_->distTo(*m2_); }

void Spring::applyForceToMasses() {
	// update current state
	currLength_ = getLength();
	direction_ = m1_->directionTo(*m2_);

	// add forces
	Vec3 force = -stiffness_ * (currLength_ - restLength_) * direction_;
	m1_->addForce(force);
	m2_->addForce(-force);
}

void Spring::draw(DrawingUtilitiesClass* DUC) {
	DUC->beginLine();
	DUC->drawLine(m1_->getPosition(), Vec3(1, 0, 0), m2_->getPosition(), Vec3(1, 0, 0));
	DUC->endLine();
}