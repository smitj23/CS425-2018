#include "MyPhysics.h"

//
//part #1:
//create a ball physics system
//

	//add a ball to the system 
	//input: initial position and velocity and radius and its life span
	void BallPhysicsEngine::addBall(Ogre::Vector3& pos, Ogre::Vector3& vec, double radius, int lifespan)
	{
		//create MyBall and add to m_MyParticles
	}

	//create your partciles here and add the MyParticles in
	//OGRE for visualization 
	void BallPhysicsEngine::SetupEnv()
	{

	}

	//step the simulation forward 
	bool BallPhysicsEngine::Step(double time)
	{
		return false;
	}


	//compute force for each MyParticle
	void BallPhysicsEngine::ApplyForce(std::vector<Ogre::Vector3> & forces)
	{

	}

	//compute the derivative for each MyParticle from the given force
	//input: forces
	//output: dx, dv
	void BallPhysicsEngine::ComputeDerivative(const std::vector<Ogre::Vector3> & forces,
		std::vector<Ogre::Vector3> & dx,
		std::vector<Ogre::Vector3> & dv)
	{

	}

	//compute the new states of the MyParticle
	//output: new position and new velocity nx & nv
	void BallPhysicsEngine::BallPhysicsEngine::SolveODE(std::vector<Ogre::Vector3> & nx, std::vector<Ogre::Vector3> & nv)
	{

	}

	//check for collision
	bool BallPhysicsEngine::BallPhysicsEngine::isCollision(const Ogre::Vector3& x, const Ogre::Vector3& nx)
	{
		return false;
	}

	//
	//compute collision response of a single MyParticle
	//input: a partcile p and simulation time
	//
	//return: remaining time
	//
	double BallPhysicsEngine::CollisionResponse(const MyParticle& p, double step, Ogre::Vector3& nx, Ogre::Vector3& nv)
	{
		return step;
	}


	//
	//part #2:
	//create a mass-spring physics system
	//

	//add a deformable tetrahedron to the system 
	//input: initial position and velocity and radius and its life span
	void MassSpringPhysicsEngine::addTetra(Ogre::Vector3 pos[4], Ogre::Vector3 vec[4], int lifespan)
	{

	}

	//create your partciles here and add the MyParticles in
	//OGRE for visualization 
	void MassSpringPhysicsEngine::SetupEnv() 
	{

	}

	//step the simulation forward 
	bool MassSpringPhysicsEngine::Step(double time)
	{
		return false;
	}


	//compute force for each MyParticle
	void MassSpringPhysicsEngine::ApplyForce(std::vector<Ogre::Vector3> & forces)
	{

	}

	//compute the derivative for each MyParticle from the given force
	//input: forces
	//output: dx, dv
	void MassSpringPhysicsEngine::ComputeDerivative(const std::vector<Ogre::Vector3> & forces,
		std::vector<Ogre::Vector3> & dx,
		std::vector<Ogre::Vector3> & dv)
	{

	}

	//compute the new states of the MyParticle
	//output: new position and new velocity nx & nv
	void MassSpringPhysicsEngine::SolveODE(std::vector<Ogre::Vector3> & nx, std::vector<Ogre::Vector3> & nv)
	{

	}

	//check for collision
	bool MassSpringPhysicsEngine::isCollision(const Ogre::Vector3& x, const Ogre::Vector3& nx)
	{
		return false;
	}

	//
	//compute collision response of a single MyParticle
	//input: a partcile p and simulation time
	//
	//return: remaining time
	//
	double MassSpringPhysicsEngine::CollisionResponse(const MyParticle& p, double step, Ogre::Vector3& nx, Ogre::Vector3& nv)
	{
		return step;
	}


