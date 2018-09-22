//
//1. Use Grid class to detect if the user-controlled ogre overlaps with an object or a moving agent. You need to make sure that the cells occupied by the moving agents are correctly marked. (30 pts)
//
//2. Modify the code so that, the user-controlled ogre will not move into a cell that would cause collision. (20 pts)
//
#include "LL.h"
#include "SinbadCharacterController.h"

using namespace std;

class PA02 : public LevelLoading
{
public:

	PA02(const std::string & filename) : LevelLoading(filename)
	{

	}

	virtual ~PA02()
	{
		delete mChara;
	}

	virtual void setup() override
	{
		LevelLoading::setup();
		m_Camera_Man->setStyle(CS_MANUAL); //disable camera
		//m_Scene_Mgr->setFog(Ogre::FOG_LINEAR, ColourValue(1.0f, 1.0f, 0.8f), 0, 15, 100);

		mChara = new SinbadCharacterController(this->m_Camera);
	}

	//handling rendering events
	virtual bool frameRenderingQueued(const Ogre::FrameEvent& evt) override
	{
		LevelLoading::frameRenderingQueued(evt);

		mChara->addTime(evt.timeSinceLastFrame);
		return true;
	}

	bool keyPressed(const KeyboardEvent& evt) override
	{
		if (!m_Tray_Mgr->isDialogVisible()) mChara->injectKeyDown(evt);
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
};


int main(int argc, char **argv)
{
	if (argc != 2) cerr << "Usage: " << argv[0] << " level_file" << endl;

	try
	{
		PA02 app(argv[1]);
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
