#include "MyPhysics.h"
#include "OgreMesh.h"
#include "OgreMeshManager.h"

#include <iostream>

void MyParticlePhysicsEngineBase::SetupEnv()
{
	//
	//cout << "-------------------------------------" << endl;
	std::list<Ogre::SceneNode *> open;
	open.push_back(this->m_Scene_Mgr->getRootSceneNode());
	while (open.empty() == false)
	{
		Ogre::SceneNode * node = open.front();
		open.pop_front();

		auto objects = node->getAttachedObjects();
		for (auto obj : objects)
		{
			if (dynamic_cast<Ogre::Entity *>(obj) == NULL) continue;
			m_all_obstacles.push_back(std::make_pair(node,(Ogre::Entity *)obj));

		}//end for obj

		 //get all the kids
		for (auto kid : node->getChildren())
		{
			if (dynamic_cast<Ogre::SceneNode *>(kid) != NULL)
				open.push_back(dynamic_cast<Ogre::SceneNode *>(kid));
		}
	}
	//cout << "-------------------------------------" << endl;
}

//
//part #1:
//create a ball physics system
//

	//add a ball to the system 
	//input: initial position and velocity and radius and its life span
	//also add the ball in OGRE for visualization 
	void BallPhysicsEngine::addBall(Ogre::Vector3& pos, Ogre::Vector3& vec, double radius, int lifespan)
	{
		//create MyBall and add to m_MyParticles

		//create an entity and a node in Ogre (also find a way to asscoiate this node with MyBall)
	}


	void BallPhysicsEngine::SetupEnv()
	{
		//call base SetupEnv
		MyParticlePhysicsEngineBase::SetupEnv();

		//TODO: compute the bounding box of the entire world so that the balls 
		//won't go out of bound

	}

	//step the simulation forward 
	bool BallPhysicsEngine::Step(double time)
	{
		//remove dead particles (whose timer is <0)

		//simulate (time/m_h) times

		//reduce the timer of the particles

		return false;
	}


	//compute force for each MyParticle
	void BallPhysicsEngine::ApplyForce(std::vector<Ogre::Vector3> & forces)
	{
		//add gravity and viscos drag
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
		//implement Euler's method here
	}

	//check for collision
	bool BallPhysicsEngine::BallPhysicsEngine::isCollision(const MyParticle& p, const Ogre::Vector3& x, const Ogre::Vector3& nx)
	{
		//check if the ball p located at x moving toward nx is in collision with any object in the world.
		//check against all bounding boxes of all entities in m_all_obstacles

		//to get the bounding box:

		//Ogre::AxisAlignedBox box = ((Ogre::Entity*)obj)->getBoundingBox();
		//box.transform(node->_getFullTransform());
		//std::cout << "name=" << obj->getName() << " bbox=" << box << std::endl;

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
		return step; //return the remaining step
	}


	//
	//part #2:
	//create a mass-spring physics system
	//

	//add a deformable tetrahedron to the system 
	//input: initial position and velocity and radius and its life span
	//also add the tetrahedron in OGRE for visualization 
	void MassSpringPhysicsEngine::addTetra(Ogre::Vector3 pos[4], Ogre::Vector3 vec[4], int lifespan)
	{
		//create 4 balls and 6 springs

		//create an entity and a node in Ogre for each ball
		//also create an tetrahedron mesh and we will be deforming it as we Step forward in time
	}


	void MassSpringPhysicsEngine::SetupEnv() 
	{
		//call base SetupEnv
		MyParticlePhysicsEngineBase::SetupEnv();

		//TODO: compute the bounding box of the entire world so that the balls 
		//won't go out of bound
	}

	//step the simulation forward 
	bool MassSpringPhysicsEngine::Step(double time)
	{
		//remove dead tetrahedra (whose timer is <0)

		//simulate (time/m_h) times

		//reduce the timer of all tetrahedra

		return false;
	}


	//compute force for each MyParticle
	void MassSpringPhysicsEngine::ApplyForce(std::vector<Ogre::Vector3> & forces)
	{
		//add gravity, drag and string force
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
		//implement midpoint methods or RK3 here
	}

	//check for collision
	bool MassSpringPhysicsEngine::isCollision(const MyParticle& p, const Ogre::Vector3& x, const Ogre::Vector3& nx)
	{
		//check if the ball p located at x moving toward nx is in collision with any object in the world.
		//check against all bounding boxes of all entities in m_all_obstacles

		//to get the bounding box:

		//Ogre::AxisAlignedBox box = ((Ogre::Entity*)obj)->getBoundingBox();
		//box.transform(node->_getFullTransform());
		//std::cout << "name=" << obj->getName() << " bbox=" << box << std::endl;

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


