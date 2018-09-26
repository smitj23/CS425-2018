
//modified 9/26/2018 add GUI

#pragma once

//ogre
#include <Ogre.h>
#include <OgreApplicationContext.h>
#include <OgreInput.h>
#include <OgreRTShaderSystem.h>
#include <OgreCameraMan.h>
#include <OgreTrays.h>
#include <OgreAdvancedRenderControls.h>

#include <iostream>
using namespace std;

using namespace Ogre;
using namespace OgreBites;

class OgreBasic : public ApplicationContext, public InputListener, public TrayListener
{
public:

	OgreBasic(const String & name) : ApplicationContext(name)
	{
		m_Camera_Man = NULL;
		m_Controls = NULL;
		m_CursorWasVisible = false;
		m_DragLook = false;
		m_Camera = NULL;
	}

	virtual ~OgreBasic()
	{
		if (m_Camera_Man != NULL) delete m_Camera_Man;
		if (m_Controls != NULL) delete m_Controls;
		if (m_Camera != NULL) delete m_Camera;
	}

	virtual void OgreBasic::setup()
	{
		// do not forget to call the base first
		ApplicationContext::setup();
		addInputListener(this);

		// get a pointer to the already created root
		Root* root = getRoot();
		

		m_Scene_Mgr = root->createSceneManager();

		//this is necessary to view OGRE tray GUI...
		m_Scene_Mgr->addRenderQueueListener(mOverlaySystem);

		// register our scene with the RTSS
		RTShader::ShaderGenerator* shadergen = RTShader::ShaderGenerator::getSingletonPtr();
		shadergen->addSceneManager(m_Scene_Mgr);

		//create ambient light
		m_Scene_Mgr->setAmbientLight(ColourValue(0.5, 0.5, 0.5));

		//create point light
		Light* light = m_Scene_Mgr->createLight("MainLight");
		light->setType(Light::LT_POINT);
		SceneNode* lightNode = m_Scene_Mgr->getRootSceneNode()->createChildSceneNode();
		lightNode->attachObject(light);
		lightNode->setPosition(20, 180, 50);


		//! [camera]
		this->m_Camera_Node = m_Scene_Mgr->getRootSceneNode()->createChildSceneNode();

		// create the camera
		this->m_Camera = m_Scene_Mgr->createCamera("myCam");
		m_Camera->setNearClipDistance(5); // specific to this sample
		m_Camera->setAutoAspectRatio(true);
		m_Camera->lookAt(0, 0, 0);
		this->m_Camera_Node->attachObject(m_Camera);
		this->m_Camera_Node->setPosition(0, 150, 222);

		// and tell it to render into the main window
		getRenderWindow()->addViewport(m_Camera);	

		//create camera man
		this->m_Camera_Man = new OgreBites::CameraMan(this->m_Camera_Node);

		// create tray manager and show stats and logo and (hide the cursor)
		m_Tray_Mgr = new OgreBites::TrayManager("Controls", getRenderWindow(), this);  // create a tray interface
		m_Tray_Mgr->showFrameStats(TL_BOTTOMLEFT);
		m_Tray_Mgr->showLogo(TL_BOTTOMRIGHT);
		//m_Tray_Mgr->hideCursor();
		m_Tray_Mgr->setListener(this);

		m_Controls = new AdvancedRenderControls(m_Tray_Mgr, m_Camera);
	}

	virtual void buttonHit(Button* button) override
	{
        //do nothing
	}

	void loadResources()
	{
		Ogre::ResourceGroupManager::getSingleton().initialiseResourceGroup("Essential");
		m_Tray_Mgr = new TrayManager("BrowserControls", getRenderWindow(), this);
		m_Tray_Mgr->showBackdrop("SdkTrays/Bands");
		m_Tray_Mgr->getTrayContainer(TL_NONE)->hide();
		createDummyScene();
		m_Tray_Mgr->showLoadingBar(1, 0);
		ApplicationContext::loadResources();
		m_Tray_Mgr->hideLoadingBar();
		destroyDummyScene();
		delete m_Tray_Mgr;
	}

	bool keyPressed(const KeyboardEvent& evt) override
	{
		if (evt.keysym.sym == SDLK_ESCAPE)
		{
			getRoot()->queueEndRendering();
		}

		m_Controls->keyPressed(evt);
		m_Camera_Man->keyPressed(evt);

		return true;
	}

	virtual bool keyReleased(const KeyboardEvent& evt)
	{
		m_Camera_Man->keyReleased(evt);

		return true;
	}

	virtual void unpaused()
	{
		m_Tray_Mgr->refreshCursor();
	}

	/*-----------------------------------------------------------------------------
	| Automatically saves position and orientation for free-look cameras.
	-----------------------------------------------------------------------------*/
	virtual void saveState(Ogre::NameValuePairList& state)
	{
		if (m_Camera_Man->getStyle() == CS_FREELOOK)
		{
			state["CameraPosition"] = Ogre::StringConverter::toString(m_Camera_Node->getPosition());
			state["CameraOrientation"] = Ogre::StringConverter::toString(m_Camera_Node->getOrientation());
		}
	}

	/*-----------------------------------------------------------------------------
	| Automatically restores position and orientation for free-look cameras.
	-----------------------------------------------------------------------------*/
	virtual void restoreState(Ogre::NameValuePairList& state)
	{
		if (state.find("CameraPosition") != state.end() && state.find("CameraOrientation") != state.end())
		{
			m_Camera_Man->setStyle(CS_FREELOOK);
			m_Camera_Node->setPosition(Ogre::StringConverter::parseVector3(state["CameraPosition"]));
			m_Camera_Node->setOrientation(Ogre::StringConverter::parseQuaternion(state["CameraOrientation"]));
		}
	}

	virtual bool frameRenderingQueued(const Ogre::FrameEvent& evt)
	{

		m_Tray_Mgr->frameRendered(evt);
		m_Controls->frameRendered(evt);

		if (!m_Tray_Mgr->isDialogVisible())
		{
			m_Camera_Man->frameRendered(evt);   // if dialog isn't up, then update the camera
		}

		return true;
	}

	virtual bool mouseMoved(const MouseMotionEvent& evt)
	{
		if (m_Tray_Mgr->mouseMoved(evt)) return true;

		m_Camera_Man->mouseMoved(evt);
		return true;
	}

	virtual bool mousePressed(const MouseButtonEvent& evt)
	{
		if (m_Tray_Mgr->mousePressed(evt)) {
			cout << "m_Tray_Mgr handled event " <<evt.type << endl;
			return true;
		}

		if (m_DragLook && evt.button == BUTTON_LEFT)
		{
			m_Camera_Man->setStyle(CS_FREELOOK);
			m_Tray_Mgr->hideCursor();
		}

		m_Camera_Man->mousePressed(evt);
		return true;
	}

	virtual bool mouseReleased(const MouseButtonEvent& evt)
	{
		if (m_Tray_Mgr->mouseReleased(evt)) return true;

		if (m_DragLook && evt.button == BUTTON_LEFT)
		{
			m_Camera_Man->setStyle(CS_MANUAL);
			m_Tray_Mgr->showCursor();
		}

		m_Camera_Man->mouseReleased(evt);
		return true;
	}

	// convert and redirect
	virtual bool touchReleased(const TouchFingerEvent& evt) {
		MouseButtonEvent e;
		e.button = BUTTON_LEFT;
		return mouseReleased(e);
	}

	virtual bool mouseWheelRolled(const MouseWheelEvent& evt) {
		m_Camera_Man->mouseWheelRolled(evt);
		return true;
	}

	virtual void windowResized(Ogre::RenderWindow* rw) {
		cout << "window resized w="<< rw->getWidth()<<" h="<< rw->getHeight() << endl;
	}

protected:

	SceneManager * m_Scene_Mgr;                  // scene manager
	Camera* m_Camera;
	OgreBites::CameraMan *  m_Camera_Man;          // camera controller
	Ogre::SceneNode* m_Camera_Node;                // camera itself
	OgreBites::TrayManager * m_Tray_Mgr;           // tray interface manager
	AdvancedRenderControls* m_Controls;           // sample details panel
	bool m_CursorWasVisible;                     // was cursor visible before dialog appeared
	bool m_DragLook;                              // click and drag to free-look
};
