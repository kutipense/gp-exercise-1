#pragma once

#include "Spring.h"
#include "MassPoint.h"
#include "DrawingUtilitiesClass.h"

class Demo4
{
public:
	Demo4()
	{
		reset();
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

	void draw(DrawingUtilitiesClass* DUC) {
		for (size_t i = 0; i < t_springs_.size(); i++) t_springs_.at(i).draw(DUC);
		for (size_t i = 0; i < t_massPoints_.size(); i++) t_massPoints_.at(i)->draw(DUC);
	}

private:

	void reset() {
		float dir = -0.5;
		for (size_t i = 0; i < 4; i++) {
			for (size_t j = 0; j < 4; j++) {
				t_massPoints_.push_back(make_shared<MassPoint>(1));
				t_massPoints_.back()->setPosition(Vec3(-1.5 + i, j, dir));
				dir *= -1;
			}
			dir *= -1;
		}

		for (size_t i = 0; i < 4; i++) {
			for (size_t j = 0; j < 3; j++) {
				t_springs_.emplace_back(10, 1);
				t_springs_.back().init(t_massPoints_.at(i * 4 + j), t_massPoints_.at(i * 4 + j + 1));
				t_springs_.back().setRestLength(t_springs_.back().getLength() * 0.9);

				t_springs_.emplace_back(10, 1);
				t_springs_.back().init(t_massPoints_.at(j * 4 + i), t_massPoints_.at((j + 1) * 4 + i));
				t_springs_.back().setRestLength(t_springs_.back().getLength()*0.9);

			}
		}
	}

	void _update(float timeStep, int integrator, bool addGravity = false) {
		for (size_t i = 0; i < t_springs_.size(); i++) t_springs_.at(i).applyForceToMasses();
		for (size_t i = 0; i < t_massPoints_.size(); i++) t_massPoints_.at(i)->update(timeStep, integrator, addGravity);
		for (size_t i = 0; i < t_massPoints_.size(); i++) {
			Vec3& position = t_massPoints_.at(i)->getPosition();
			if (position.Y < -2) {
				if (t_massPoints_.at(i)->getVelocity().Y < 0)
					t_massPoints_.at(i)->getVelocity().Y = -0.9 * t_massPoints_.at(i)->getVelocity().Y;
				/*Vec3 bounce(0, 300 * t_massPoints_.at(i)->getMass(), 0);
				t_massPoints_.at(i)->addForce(bounce);*/
			}
		}
	}

	vector<shared_ptr<MassPoint>> t_massPoints_;
	vector<Spring> t_springs_;
};

