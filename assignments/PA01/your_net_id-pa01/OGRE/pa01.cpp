#include "pa01.h"
#include "Agent.h"

//stl
#include <iostream>

using namespace std;

class CS425PA01 : public OgreBasic
{
public:
    CS425PA01();
    virtual ~CS425PA01() {
		    if (m_agent != NULL) delete m_agent;
	  }

    void setup() override;

	  Agent * m_agent;

    //handling rendering events
  	virtual bool frameRenderingQueued(const Ogre::FrameEvent& evt)
  	{
      OgreBasic::frameRenderingQueued(evt);

  		if (m_agent != NULL) m_agent->update(evt.timeSinceLastFrame);

  		return true;
  	}

};


CS425PA01::CS425PA01() : OgreBasic("CS425 PA01")
{
	  m_agent = NULL;
}


void CS425PA01::setup()
{
	OgreBasic::setup();
	assert(m_Scene_Mgr);

	//task 1 (10 pts):
	//create floor of size 1000x1000 textured with Examples/Rockwall
	//the floor is located at (0,0,0)
	
 	//create moving agent
	m_agent = new Agent(m_Scene_Mgr, "Sinbad", "Sinbad.mesh");
}

int main(int argc, char **argv)
{
    try
    {
    	CS425PA01 app;
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
