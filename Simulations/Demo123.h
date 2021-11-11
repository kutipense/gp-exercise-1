#pragma once

#include "Spring.h"
#include "MassPoint.h"
#include "DrawingUtilitiesClass.h"

class Demo123
{
public:
	Demo123() : 
		spring_(40, 1),
		m1_(make_shared<MassPoint>(10)),
		m2_(make_shared<MassPoint>(10))
	{
		reset();
		spring_.init(m1_, m2_);
	}

	void update(float timeStep, int integrator, bool addGravity = false) {
		if (integrator == EULER) {
			_update(timeStep, EULER, addGravity);
		}
		else if (integrator == MIDPOINT) {
			_update(timeStep / 2, EULER, addGravity);
			_update(timeStep, MIDPOINT, addGravity);
		}
	}

	void test() {
		cout << "EULER METHOD (h=0.1)" << endl;
		reset();
		update(0.1, EULER, false);
		print();

		cout << "MIDPOINT METHOD (h=0.1)" << endl;
		reset();
		update(0.1, MIDPOINT, false);
		print();

	}

	void draw(DrawingUtilitiesClass* DUC) { 
		spring_.draw(DUC); 
		m1_->draw(DUC);
		m2_->draw(DUC);
	}

private:
	void reset() {
		m1_->setPosition(0, 0, 0);
		m2_->setPosition(0, 2, 0);

		m1_->setVelocity(-1, 0, 0);
		m2_->setVelocity(1, 0, 0);
	}

	void print() {
		cout << "----------------------" << endl;
		cout << "m1 position: " << m1_->getPosition() << endl;
		cout << "m1 velocity: " << m1_->getVelocity() << endl;
		cout << "m2 position: " << m2_->getPosition() << endl;
		cout << "m2 velocity: " << m2_->getVelocity() << endl;
		cout << "----------------------" << endl << endl;
	}

	void _update(float timeStep, int integrator, bool addGravity = false) {
		spring_.applyForceToMasses();

		m1_->update(timeStep, integrator, addGravity);
		m2_->update(timeStep, integrator, addGravity);
	}

	shared_ptr<MassPoint> m1_;
	shared_ptr<MassPoint> m2_;
	Spring spring_;
};

