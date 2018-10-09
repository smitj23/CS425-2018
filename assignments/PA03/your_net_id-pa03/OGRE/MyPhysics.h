#pragma once

#include <Ogre.h>
#include <OgreVector3.h>
#include <vector> //stl vector
#include <list>   //stl list


struct MyParticle
{
    Ogre::Vector3 x; //position
    Ogre::Vector3 v; //linear velocity
    double m;    //mass
};

//the base class
class MyParticlePhysicsEngineBase
{
    public:

	  	//h is the simulation step size
	  	MyParticlePhysicsEngineBase(double h):m_h(h){ }

       //create your partciles here and add the MyParticles in
       //OGRE for visualization
       virtual void SetupEnv()=0;

       //step the simulation forward
       virtual bool Step(double time)=0;

       //access functions
       void addSceneManager(Ogre::SceneManager * sm) { m_Scene_Mgr=sm; }

    protected:

       //compute force for each MyParticle
       virtual void ApplyForce(std::vector<Ogre::Vector3> & forces)=0;

       //compute the derivative for each MyParticle from the given force
       //input: forces
       //output: dx, dv
       virtual void ComputeDerivative(const std::vector<Ogre::Vector3> & forces,
                                            std::vector<Ogre::Vector3> & dx,
                                            std::vector<Ogre::Vector3> & dv)=0;

       //compute the new states of the MyParticle
       //output: new position and new velocity nx & nv
       virtual void SolveODE(std::vector<Ogre::Vector3> & nx, std::vector<Ogre::Vector3> & nv)=0;

       //check for collision
	     virtual bool isCollision(const Ogre::Vector3& x, const Ogre::Vector3& nx)=0;

       //
       //compute collision response of a single MyParticle
       //input: a partcile p and simulation time
       //
       //return: remaining time
       //
       virtual double CollisionResponse(const MyParticle& p, double step, Ogre::Vector3& nx, Ogre::Vector3& nv) = 0 ;

       //MyParticles
       std::list<MyParticle*> m_MyParticles;

	     //step size
	     double m_h;

       //
	   Ogre::SceneManager * m_Scene_Mgr;
};


//a ball physics system
class BallPhysicsEngine : public MyParticlePhysicsEngineBase
{
public:

	//h is the simulation step size
	BallPhysicsEngine(double h): MyParticlePhysicsEngineBase(h) { }

	//add a ball to the system
	//input: initial position and velocity and radius and its life span
	void addBall(Ogre::Vector3& pos, Ogre::Vector3& vec, double radius, int lifespan);

	//create your partciles here and add the MyParticles in
	//OGRE for visualization
	virtual void SetupEnv() override;

	//step the simulation forward
	virtual bool Step(double time) override;

protected:

	//compute force for each MyParticle
	virtual void ApplyForce(std::vector<Ogre::Vector3> & forces) override;

	//compute the derivative for each MyParticle from the given force
	//input: forces
	//output: dx, dv
	virtual void ComputeDerivative(const std::vector<Ogre::Vector3> & forces,
		std::vector<Ogre::Vector3> & dx,
		std::vector<Ogre::Vector3> & dv) override;

	//compute the new states of the MyParticle
	//output: new position and new velocity nx & nv
	virtual void SolveODE(std::vector<Ogre::Vector3> & nx, std::vector<Ogre::Vector3> & nv) override;

	//check for collision
	virtual bool isCollision(const Ogre::Vector3& x, const Ogre::Vector3& nx) override;

	//
	//compute collision response of a single MyParticle
	//input: a partcile p and simulation time
	//
	//return: remaining time
	//
	virtual double CollisionResponse(const MyParticle& p, double step, Ogre::Vector3& nx, Ogre::Vector3& nv) override;

private:

	struct MyBall : public MyParticle
	{
		int lifespan;
		double radius;
	};
};


//a mass-spring physics system
class MassSpringPhysicsEngine : public MyParticlePhysicsEngineBase
{
public:

	//h is the simulation step size
	MassSpringPhysicsEngine(double h) : MyParticlePhysicsEngineBase(h) { }

	//add a deformable tetrahedron to the system
	//input: initial position and velocity and radius and its life span
	void addTetra(Ogre::Vector3 pos[4], Ogre::Vector3 vec[4], int lifespan);

	//create your partciles here and add the MyParticles in
	//OGRE for visualization
	virtual void SetupEnv() override;

	//step the simulation forward
	virtual bool Step(double time) override;

protected:

	//compute force for each MyParticle
	virtual void ApplyForce(std::vector<Ogre::Vector3> & forces) override;

	//compute the derivative for each MyParticle from the given force
	//input: forces
	//output: dx, dv
	virtual void ComputeDerivative(const std::vector<Ogre::Vector3> & forces,
		std::vector<Ogre::Vector3> & dx,
		std::vector<Ogre::Vector3> & dv) override;

	//compute the new states of the MyParticle
	//output: new position and new velocity nx & nv
	virtual void SolveODE(std::vector<Ogre::Vector3> & nx, std::vector<Ogre::Vector3> & nv) override;

	//check for collision
	virtual bool isCollision(const Ogre::Vector3& x, const Ogre::Vector3& nx) override;

	//
	//compute collision response of a single MyParticle
	//input: a partcile p and simulation time
	//
	//return: remaining time
	//
	virtual double CollisionResponse(const MyParticle& p, double step, Ogre::Vector3& nx, Ogre::Vector3& nv) override;

private:

	struct Spring
	{
		MyParticle *a, *b; //end points of a spring
		double k_s; //spring coefficient
		double k_d; //spring damping
		double rest_length;
	};

	struct MyBall : public MyParticle
	{
		int lifespan;
		double radius;
		std::list<Spring*> springs; //list of attached springs
	};

	std::list<Spring*> m_springs; //a list of all springs
};
