//Level loading 

#pragma once

#include "ogrebasic.h"
#include "Agent.h"
#include "Grid.h"

//stl
#include <iostream>
#include <string>

using namespace std;

class LevelLoading : public OgreBasic
{
public:

	Grid * m_grid;

	LevelLoading(const std::string & filename) : OgreBasic("CS425 PA03")
	{
		m_level_filename = filename;
		m_grid = NULL;
	}

	virtual ~LevelLoading()
	{
		for (auto agent : agentList) if (agent != NULL) delete agent;
		delete m_grid;
	}

	void setup() override
	{
		OgreBasic::setup();
		assert(m_Scene_Mgr);
		loadEnv();
		setupEnv();
	}

	//handling rendering events
	virtual bool frameRenderingQueued(const Ogre::FrameEvent& evt)
	{
		OgreBasic::frameRenderingQueued(evt);
		for (auto agent : agentList)
			if (agent != NULL)
				agent->update(evt.timeSinceLastFrame);

		return true;
	}


	// Lecture 5: Load level from file!
	// Load the buildings or ground plane, etc
	inline void loadEnv();

	// Set up lights, shadows, etc
	void setupEnv()
	{
		using namespace Ogre;

		// set shadow properties
		m_Scene_Mgr->setShadowTechnique(SHADOWTYPE_TEXTURE_MODULATIVE);
		m_Scene_Mgr->setShadowColour(ColourValue(0.5, 0.5, 0.5));
		m_Scene_Mgr->setShadowTextureSize(1024);
		m_Scene_Mgr->setShadowTextureCount(1);

		// disable default camera control so the character can do its own
		// CS_FREELOOK, CS_ORBIT, CS_MANUAL
		m_Camera_Man->setStyle(OgreBites::CS_FREELOOK);

		// use small amount of ambient lighting
		m_Scene_Mgr->setAmbientLight(ColourValue(0.3f, 0.3f, 0.3f));

		// add a bright light above the scene
		Light* light = m_Scene_Mgr->createLight();
		light->setType(Light::LT_POINT);
		light->setPosition(-10, 40, 20);
		light->setSpecularColour(ColourValue::White);

		//m_Scene_Mgr->setSkyDome(true, "Examples/CloudySky", 5, 8);
	}


protected:

	// Lecture 5: Returns a unique name for loaded objects and agents
	std::string getNewName() // return a unique name
	{
		static int count = 0;	// keep counting the number of objects
		std::stringstream out;	// a stream for outputing to a string
		out << count++;			// make the current count into a string
		return "object_" + out.str();	// append the current count onto the string
	}

	std::list<Agent*> agentList; // Lecture 5: now a list of agents
	std::string m_level_filename;
};


// Lecture 5: Load level from file!
// Load the buildings or ground plane, etc
inline void LevelLoading::loadEnv()
{
	using namespace Ogre;	// use both namespaces
	using namespace std;

	class readEntity // need a structure for holding entities
	{
	public:
		string filename;
		float y;
		float scale;
		float orient;
		bool agent;
	};

	ifstream inputfile;		// Holds a pointer into the file

	string path = __FILE__; //gets the current cpp file's path with the cpp file
	path = path.substr(0, 1 + path.find_last_of('\\')); //removes filename to leave path
	path += this->m_level_filename; //if txt file is in the same directory as cpp file
	inputfile.open(path);
	std::cout << "path=" << path << std::endl;

	if (!inputfile.is_open()) // oops. there was a problem opening the file
	{
		cout << "ERROR, FILE COULD NOT BE OPENED" << std::endl;	// Hmm. No output?
		return;
	}

	// the file is open
	int x, z;
	inputfile >> x >> z;	// read in the dimensions of the grid
	string matName;
	inputfile >> matName;	// read in the material name

	// create floor mesh using the dimension read
	MeshManager::getSingleton().createPlane("floor", ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME,
		Plane(Vector3::UNIT_Y, 0), x*NODESIZE, z*NODESIZE, x, z, true, 1, x, z, Vector3::UNIT_Z);

	//create a floor entity, give it material, and place it at the origin
	Entity* floor = m_Scene_Mgr->createEntity("Floor", "floor");
	floor->setMaterialName(matName);
	floor->setCastShadows(false);
	m_Scene_Mgr->getRootSceneNode()->attachObject(floor);

	Grid * grid=new Grid(m_Scene_Mgr, z, x); // Set up the grid. z is rows, x is columns
	m_grid = grid;

	string buf;
	inputfile >> buf;	// Start looking for the Objects section
	while (buf != "Objects")
		inputfile >> buf;

	if (buf != "Objects")	// Oops, the file must not be formated correctly
	{
		cout << "ERROR: Level file error" << endl;
		return;
	}

	// read in the objects
	readEntity *rent = NULL;	// hold info for one object
	std::map<string, readEntity*> objs;		// hold all object and agent types;

	// read through all objects until you find the Characters section
	while (!inputfile.eof() && buf != "Characters")
	{
		inputfile >> buf;			// read in the char
		if (buf != "Characters")
		{
			rent = new readEntity();	// create a new instance to store the next object

			// read the rest of the line
			inputfile >> rent->filename >> rent->y >> rent->orient >> rent->scale;
			rent->agent = false;		// these are objects
			objs[buf] = rent;		  	// store this object in the map
		}
	}

	while (buf != "Characters")	// get through any junk
		inputfile >> buf;

	// Read in the characters
	while (!inputfile.eof() && buf != "World") // Read through until the world section
	{
		inputfile >> buf;		// read in the char
		if (buf != "World")
		{
			rent = new readEntity();	// create a new instance to store the next object
			inputfile >> rent->filename >> rent->y >> rent->scale; // read the rest of the line
			rent->agent = true;			// this is an agent
			objs[buf] = rent;			// store the agent in the map
		}
	}
	//delete rent; // we didn't need the last one

	// read through the placement map
	char c;
	for (int i = 0; i < z; i++)			// down (row)
	{
		for (int j = 0; j < x; j++)		// across (column)
		{
			inputfile >> c;			// read one char at a time
			buf = c + '\0';			// convert char to string
			rent = objs[buf];		// find cooresponding object or agent
			if (rent != NULL)		// it might not be an agent or object
			{
				if (rent->agent)	// if it is an agent...
				{
					// Use subclasses instead!
					Agent * agent = new Agent(this->m_Scene_Mgr, getNewName(), rent->filename, rent->y, rent->scale, this->m_grid);
					agent->setPosition(grid->getPosition(i, j).x, 0, grid->getPosition(i, j).z);
					agentList.push_back(agent);
					// If we were using different characters, we'd have to deal with
					// different animation clips.
				}
				else	// Load objects
				{
					grid->loadObject(getNewName(), rent->filename, i, rent->y, j, rent->scale);
				}
			}
			else // rent==null, not an object or agent
			{
				if (c == 'w') // create a wall
				{
					Entity* ent = m_Scene_Mgr->createEntity(getNewName(), Ogre::SceneManager::PT_CUBE);
					ent->setMaterialName("Examples/RustySteel");
					Ogre::SceneNode* mNode = m_Scene_Mgr->getRootSceneNode()->createChildSceneNode();
					mNode->attachObject(ent);
					mNode->scale(0.1f, 0.2f, 0.1f); // cube is 100 x 100
					grid->getNode(i, j)->setOccupied();  // indicate that agents can't pass through
					mNode->setPosition(grid->getPosition(i, j).x, 10.0f, grid->getPosition(i, j).z);
				}
				else if (c == 'e') //particle emitter
				{
					ParticleSystem::setDefaultNonVisibleUpdateTimeout(5);  // set nonvisible timeout
					ParticleSystem* ps = m_Scene_Mgr->createParticleSystem(getNewName(), "Examples/PurpleFountain");
					Ogre::SceneNode* mNode = m_Scene_Mgr->getRootSceneNode()->createChildSceneNode();
					mNode->attachObject(ps);
					mNode->setPosition(grid->getPosition(i, j).x, 0.0f, grid->getPosition(i, j).z);
				}
			}
		} //end for j (col)
	}//end for i (row)

	// delete all of the readEntities in the objs map
	rent = objs["s"]; // just so we can see what is going on in memory (delete this later)

	std::map<string, readEntity*>::iterator it;
	for (it = objs.begin(); it != objs.end(); it++) // iterate through the map
	{
		delete (*it).second; // delete each readEntity
	}
	objs.clear(); // calls their destructors if there are any. (not good enough)

	inputfile.close();
	grid->printToFile(); // see what the initial grid looks like.
}
