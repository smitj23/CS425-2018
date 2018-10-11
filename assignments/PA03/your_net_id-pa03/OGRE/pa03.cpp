#include "LL.h"
#include "MyPhysics.h"
#include "SinbadCharacterController.h"

using namespace std;

class PA03 : public LevelLoading
{
public:

	PA03(const std::string & filename) : LevelLoading(filename)
	{
		mBallPhy = new BallPhysicsEngine(0.01);
		mMassSpringPhy = new MassSpringPhysicsEngine(0.01);
	}

	virtual ~PA03()
	{
		delete mChara;
	}

	virtual void setup() override
	{
		LevelLoading::setup();
		m_Camera_Man->setStyle(CS_MANUAL); //disable camera
		mChara = new SinbadCharacterController(this->m_Camera, this->m_grid);


		//get all necessary geometries from OGRE
		mBallPhy->addSceneManager(this->m_Scene_Mgr);
		mBallPhy->SetupEnv();

		mMassSpringPhy->addSceneManager(this->m_Scene_Mgr);
		mMassSpringPhy->SetupEnv();
	}

	//handling rendering events
	virtual bool frameRenderingQueued(const Ogre::FrameEvent& evt) override
	{
		LevelLoading::frameRenderingQueued(evt);

		mChara->addTime(evt.timeSinceLastFrame);
		mBallPhy->Step(evt.timeSinceLastFrame);
		mMassSpringPhy->Step(evt.timeSinceLastFrame);

		return true;
	}

	bool keyPressed(const KeyboardEvent& evt) override
	{
		if (!m_Tray_Mgr->isDialogVisible()) mChara->injectKeyDown(evt);

		if (evt.keysym.sym == 'b')
		{
			//throw a ball into space by adding it to mBallPhy,
			//using mChara's current location and facing direction
			cout << "throw a ball" << endl;
		}
		else if(evt.keysym.sym == 't')
		{
			//throw a tetrahedron into space by adding it to mMassSpringPhy,
			//add a ball to deformable tetrahedron, using mChara's current location and facing direction
			cout << "throw a tetra" << endl;
		}


		return LevelLoading::keyPressed(evt);
	}

	bool keyReleased(const KeyboardEvent& evt)
	{
		if (!m_Tray_Mgr->isDialogVisible()) mChara->injectKeyUp(evt);
		return LevelLoading::keyReleased(evt);
	}

	bool mouseMoved(const MouseMotionEvent& evt)
	{
		if (!m_Tray_Mgr->isDialogVisible()) mChara->injectMouseMove(evt);
		return LevelLoading::mouseMoved(evt);
	}

	virtual bool mouseWheelRolled(const MouseWheelEvent& evt)
	{
		if (!m_Tray_Mgr->isDialogVisible()) mChara->injectMouseWheel(evt);
		return LevelLoading::mouseWheelRolled(evt);
	}

	bool mousePressed(const MouseButtonEvent& evt)
	{
		if (!m_Tray_Mgr->isDialogVisible()) mChara->injectMouseDown(evt);
		return LevelLoading::mousePressed(evt);
	}


protected:

	SinbadCharacterController * mChara;
	BallPhysicsEngine * mBallPhy;
	MassSpringPhysicsEngine * mMassSpringPhy;
};


int main(int argc, char **argv)
{
	if (argc != 2) cerr << "Usage: " << argv[0] << " level_file" << endl;

	try
	{
		PA03 app(argv[1]);
		app.initApp();
		app.getRoot()->startRendering();
		app.closeApp();
	}
	catch (const std::exception& e)
	{
		std::cerr << "Error occurred during execution: " << e.what() << '\n';
		return 1;
	}

	return 0;
}
