#include "MassSpringSystemSimulator.h"


MassSpringSystemSimulator::MassSpringSystemSimulator() {
	m_fMass = 0;
	m_fStiffness = 0;
	m_fDamping = 0;
	m_iIntegrator = 0;

	m_iTestCase = -1;

	integrationMethod_ = 0;
	gravity_ = false;
}


const char* MassSpringSystemSimulator::getTestCasesStr() {
	return "Demo 1, Demo 2, Demo 3, Demo 4";
}

void MassSpringSystemSimulator::reset() {
}

void MassSpringSystemSimulator::initUI(DrawingUtilitiesClass* DUC)
{
	this->DUC = DUC;
	TwType TW_TYPE_INTEGRATION = TwDefineEnumFromString("Integration Method", "EULER, MIDPOINT");
	switch (m_iTestCase)
	{
	case 3:
		TwAddVarRW(DUC->g_pTweakBar, "Integration Method", TW_TYPE_INTEGRATION, &integrationMethod_, "");
		TwAddVarRW(DUC->g_pTweakBar, "Gravity", TW_TYPE_BOOLCPP, &gravity_, "");
		break;
	default:break;
	}
}

void MassSpringSystemSimulator::notifyCaseChanged(int testCase)
{
	demo123_ = Demo123(); // reset the simulation
	m_iTestCase = testCase;
	switch (m_iTestCase)
	{
	case -1:
		cout << "--Test--\n";
	case 0:
		cout << "--Demo1--\n";
		demo123_.test();
		break;
	case 1:
		cout << "--Demo2--\n";
		break;
	case 2:
		cout << "--Demo3--\n";
		break;
	case 3:
		cout << "--Demo4--\n";
		demo4_ = Demo4();
		break;
	default: break;
	}
}

void MassSpringSystemSimulator::externalForcesCalculations(float timeElapsed)
{
	//// Apply the mouse deltas to g_vfMovableObjectPos (move along cameras view plane)
	//Point2D mouseDiff;
	//mouseDiff.x = m_trackmouse.x - m_oldtrackmouse.x;
	//mouseDiff.y = m_trackmouse.y - m_oldtrackmouse.y;
	//if (mouseDiff.x != 0 || mouseDiff.y != 0)
	//{
	//	Mat4 worldViewInv = Mat4(DUC->g_camera.GetWorldMatrix() * DUC->g_camera.GetViewMatrix());
	//	worldViewInv = worldViewInv.inverse();
	//	Vec3 inputView = Vec3((float)mouseDiff.x, (float)-mouseDiff.y, 0);
	//	Vec3 inputWorld = worldViewInv.transformVectorNormal(inputView);
	//	// find a proper scale!
	//	float inputScale = 0.001f;
	//	inputWorld = inputWorld * inputScale;
	//	m_vfMovableObjectPos = m_vfMovableObjectFinalPos + inputWorld;
	//}
	//else {
	//	m_vfMovableObjectFinalPos = m_vfMovableObjectPos;
	//}
}

void MassSpringSystemSimulator::simulateTimestep(float timeStep)
{
	// update current setup for each frame
	switch (m_iTestCase)
	{// handling different cases
	case -1:
		if (m_iIntegrator == EULER) testEuler(timeStep);
		else if (m_iIntegrator == MIDPOINT) testMidpoint(timeStep);
		break;
	case 1: demo123_.update(0.005, EULER); break;
	case 2: demo123_.update(0.005, MIDPOINT); break;
	case 3: demo4_.update(timeStep, integrationMethod_ ? MIDPOINT : EULER, gravity_); break;
	default:
		break;
	}
}

void MassSpringSystemSimulator::drawFrame(ID3D11DeviceContext* pd3dImmediateContext)
{
	switch (m_iTestCase)
	{
	case 0:
	case 1:
	case 2: demo123_.draw(DUC); break;
	case 3: demo4_.draw(DUC); break;
	}
}

void MassSpringSystemSimulator::onClick(int x, int y)
{
	//m_trackmouse.x = x;
	//m_trackmouse.y = y;
}

void MassSpringSystemSimulator::onMouse(int x, int y)
{
	//m_oldtrackmouse.x = x;
	//m_oldtrackmouse.y = y;
	//m_trackmouse.x = x;
	//m_trackmouse.y = y;
}

int MassSpringSystemSimulator::addMassPoint(Vec3 position, Vec3 Velocity, bool isFixed)
{
	t_massPoints_.push_back(make_shared<MassPoint>(m_fMass));
	t_massPoints_.back()->setPosition(position);
	t_massPoints_.back()->setVelocity(Velocity);
	t_massPoints_.back()->setFixed(isFixed);

	return t_massPoints_.size() - 1;
}

void MassSpringSystemSimulator::addSpring(int masspoint1, int masspoint2, float initialLength)
{
	t_springs_.emplace_back(m_fStiffness, initialLength);
	t_springs_.back().init(t_massPoints_.at(masspoint1), t_massPoints_.at(masspoint2));
}

int MassSpringSystemSimulator::getNumberOfMassPoints() { return t_massPoints_.size(); }

int MassSpringSystemSimulator::getNumberOfSprings() { return t_springs_.size(); }

Vec3 MassSpringSystemSimulator::getPositionOfMassPoint(int index) { return t_massPoints_.at(index)->getPosition(); }

Vec3 MassSpringSystemSimulator::getVelocityOfMassPoint(int index) { return t_massPoints_.at(index)->getVelocity(); }

void MassSpringSystemSimulator::applyExternalForce(Vec3 force)
{
	for (size_t i = 0; i < t_massPoints_.size(); i++) t_massPoints_.at(i)->addForce(force);
}

void MassSpringSystemSimulator::testUpdate(float timeStep, int integrator) {
	for (size_t i = 0; i < t_springs_.size(); i++) t_springs_.at(i).applyForceToMasses();
	for (size_t i = 0; i < t_massPoints_.size(); i++) t_massPoints_.at(i)->update(timeStep, integrator);
}

void MassSpringSystemSimulator::testEuler(float timeStep) { testUpdate(timeStep, EULER); }

void MassSpringSystemSimulator::testMidpoint(float timeStep) {
	testUpdate(timeStep / 2, EULER);
	testUpdate(timeStep, MIDPOINT);
}

