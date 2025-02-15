#ifndef __Sinbad_H__
#define __Sinbad_H__

#include "Ogre.h"
#include "OgreInput.h"
#include "Grid.h"

using namespace Ogre;
using namespace OgreBites;

#define CHAR_HEIGHT 5          // height of character's center of mass above ground
#define CAM_HEIGHT 2           // height of camera above character's center of mass
#define RUN_SPEED 17           // character running speed in units per second
#define TURN_SPEED 500.0f      // character turning in degrees per second
#define ANIM_FADE_SPEED 7.5f   // animation crossfade speed in % of full weight per second
#define JUMP_ACCEL 30.0f       // character jump acceleration in upward units per squared second
#define GRAVITY 90.0f          // gravity in downward units per squared second


class SinbadCharacterController
{
private:

    // all the animations our character has, and a null ID
    // some of these affect separate body parts and will be blended together
    enum AnimID
    {
        ANIM_IDLE_BASE,
        ANIM_IDLE_TOP,
        ANIM_RUN_BASE,
        ANIM_RUN_TOP,
        ANIM_HANDS_CLOSED,
        ANIM_HANDS_RELAXED,
        ANIM_DRAW_SWORDS,
        ANIM_SLICE_VERTICAL,
        ANIM_SLICE_HORIZONTAL,
        ANIM_DANCE,
        ANIM_JUMP_START,
        ANIM_JUMP_LOOP,
        ANIM_JUMP_END,
        NUM_ANIMS,
        ANIM_NONE = NUM_ANIMS
    };

public:
    
    SinbadCharacterController(Camera* cam, Grid * grid) : mBaseAnimID(ANIM_NONE), mTopAnimID(ANIM_NONE)
    {
		m_grid = grid;
        setupBody(cam->getSceneManager());
        setupCamera(cam);
        setupAnimations();
    }

    void addTime(Real deltaTime)
    {
        updateBody(deltaTime);
        updateAnimations(deltaTime);
        updateCamera(deltaTime);
    }

    void injectKeyDown(const KeyboardEvent& evt)
    {
        Keycode key = evt.keysym.sym;
        if (key == 'q' && (mTopAnimID == ANIM_IDLE_TOP || mTopAnimID == ANIM_RUN_TOP))
        {
            // take swords out (or put them back, since it's the same animation but reversed)
            setTopAnimation(ANIM_DRAW_SWORDS, true);
            mTimer = 0;
        }
        else if (key == 'e' && !mSwordsDrawn)
        {
            if (mTopAnimID == ANIM_IDLE_TOP || mTopAnimID == ANIM_RUN_TOP)
            {
                // start dancing
                setBaseAnimation(ANIM_DANCE, true);
                setTopAnimation(ANIM_NONE);
                // disable hand animation because the dance controls hands
                mAnims[ANIM_HANDS_RELAXED]->setEnabled(false);
            }
            else if (mBaseAnimID == ANIM_DANCE)
            {
                // stop dancing
                setBaseAnimation(ANIM_IDLE_BASE);
                setTopAnimation(ANIM_IDLE_TOP);
                // re-enable hand animation
                mAnims[ANIM_HANDS_RELAXED]->setEnabled(true);
            }
        }

        // keep track of the player's intended direction
        else if (key == 'w') mKeyDirection.z = -1;
        else if (key == 'a') mKeyDirection.x = -1;
        else if (key == 's') mKeyDirection.z = 1;
        else if (key == 'd') mKeyDirection.x = 1;

        else if (key == SDLK_SPACE && (mTopAnimID == ANIM_IDLE_TOP || mTopAnimID == ANIM_RUN_TOP))
        {
            // jump if on ground
            setBaseAnimation(ANIM_JUMP_START, true);
            setTopAnimation(ANIM_NONE);
            mTimer = 0;
        }

        if (!mKeyDirection.isZeroLength() && mBaseAnimID == ANIM_IDLE_BASE)
        {
            // start running if not already moving and the player wants to move
            setBaseAnimation(ANIM_RUN_BASE, true);
            if (mTopAnimID == ANIM_IDLE_TOP) setTopAnimation(ANIM_RUN_TOP, true);
        }
    }

    void injectKeyUp(const KeyboardEvent& evt)
    {
        Keycode key = evt.keysym.sym;
        // keep track of the player's intended direction
        if (key == 'w' && mKeyDirection.z == -1) mKeyDirection.z = 0;
        else if (key == 'a' && mKeyDirection.x == -1) mKeyDirection.x = 0;
        else if (key == 's' && mKeyDirection.z == 1) mKeyDirection.z = 0;
        else if (key == 'd' && mKeyDirection.x == 1) mKeyDirection.x = 0;

        if (mKeyDirection.isZeroLength() && mBaseAnimID == ANIM_RUN_BASE)
        {
            // stop running if already moving and the player doesn't want to move
            setBaseAnimation(ANIM_IDLE_BASE);
            if (mTopAnimID == ANIM_RUN_TOP) setTopAnimation(ANIM_IDLE_TOP);
        }
    }

    void injectMouseMove(const MouseMotionEvent& evt)
    {
        // update camera goal based on mouse movement
        updateCameraGoal(-0.05f * evt.xrel, -0.05f * evt.yrel, 0);
    }

    void injectMouseWheel(const MouseWheelEvent& evt)
    {
        // update camera goal based on mouse movement
        updateCameraGoal(0, 0, -0.05f * evt.y);
    }

    void injectMouseDown(const MouseButtonEvent& evt)
    {
        if (mSwordsDrawn && (mTopAnimID == ANIM_IDLE_TOP || mTopAnimID == ANIM_RUN_TOP))
        {
            // if swords are out, and character's not doing something weird, then SLICE!
            if (evt.button == BUTTON_LEFT) setTopAnimation(ANIM_SLICE_VERTICAL, true);
            else if (evt.button == BUTTON_RIGHT) setTopAnimation(ANIM_SLICE_HORIZONTAL, true);
            mTimer = 0;
        }
    }

private:

    void setupBody(SceneManager* sceneMgr)
    {
        // create main model
        mBodyNode = sceneMgr->getRootSceneNode()->createChildSceneNode(Vector3::UNIT_Y * CHAR_HEIGHT);

		//mBodyNode->showBoundingBox(true);

        mBodyEnt = sceneMgr->createEntity("SinbadBody", "Sinbad.mesh");
        mBodyNode->attachObject(mBodyEnt);

        // create swords and attach to sheath
        LogManager::getSingleton().logMessage("Creating swords");
        mSword1 = sceneMgr->createEntity("SinbadSword1", "Sword.mesh");
        mSword2 = sceneMgr->createEntity("SinbadSword2", "Sword.mesh");
        mBodyEnt->attachObjectToBone("Sheath.L", mSword1);
        mBodyEnt->attachObjectToBone("Sheath.R", mSword2);

        LogManager::getSingleton().logMessage("Creating the chains");
        // create a couple of ribbon trails for the swords, just for fun
        NameValuePairList params;
        params["numberOfChains"] = "2";
        params["maxElements"] = "80";
        mSwordTrail = (RibbonTrail*)sceneMgr->createMovableObject("RibbonTrail", &params);
        mSwordTrail->setMaterialName("Examples/LightRibbonTrail");
        mSwordTrail->setTrailLength(20);
        mSwordTrail->setVisible(false);
        sceneMgr->getRootSceneNode()->attachObject(mSwordTrail);


        for (int i = 0; i < 2; i++)
        {
            mSwordTrail->setInitialColour(i, 1, 0.8, 0);
            mSwordTrail->setColourChange(i, 0.75, 1.25, 1.25, 1.25);
            mSwordTrail->setWidthChange(i, 1);
            mSwordTrail->setInitialWidth(i, 0.5);
        }

        mKeyDirection = Vector3::ZERO;
        mVerticalVelocity = 0;
    }

    void setupAnimations()
    {
        // this is very important due to the nature of the exported animations
        mBodyEnt->getSkeleton()->setBlendMode(ANIMBLEND_CUMULATIVE);

        String animNames[NUM_ANIMS] =
        {"IdleBase", "IdleTop", "RunBase", "RunTop", "HandsClosed", "HandsRelaxed", "DrawSwords",
        "SliceVertical", "SliceHorizontal", "Dance", "JumpStart", "JumpLoop", "JumpEnd"};

        // populate our animation list
        for (int i = 0; i < NUM_ANIMS; i++)
        {
            mAnims[i] = mBodyEnt->getAnimationState(animNames[i]);
            mAnims[i]->setLoop(true);
            mFadingIn[i] = false;
            mFadingOut[i] = false;
        }

        // start off in the idle state (top and bottom together)
        setBaseAnimation(ANIM_IDLE_BASE);
        setTopAnimation(ANIM_IDLE_TOP);

        // relax the hands since we're not holding anything
        mAnims[ANIM_HANDS_RELAXED]->setEnabled(true);

        mSwordsDrawn = false;
    }

    void setupCamera(Camera* cam)
    {
        // create a pivot at roughly the character's shoulder
        mCameraPivot = cam->getSceneManager()->getRootSceneNode()->createChildSceneNode();
        // this is where the camera should be soon, and it spins around the pivot
        mCameraGoal = mCameraPivot->createChildSceneNode(Vector3(0, 0, 15));
        // this is where the camera actually is
        mCameraNode = cam->getParentSceneNode();
        mCameraNode->setPosition(mCameraPivot->getPosition() + mCameraGoal->getPosition());

        mCameraPivot->setFixedYawAxis(true);
        mCameraGoal->setFixedYawAxis(true);
        mCameraNode->setFixedYawAxis(true);

        // our model is quite small, so reduce the clipping planes
        cam->setNearClipDistance(0.1);
        cam->setFarClipDistance(100);

        mPivotPitch = 0;
    }

    void updateBody(Real deltaTime)
    {
        mGoalDirection = Vector3::ZERO;   // we will calculate this

        if (mKeyDirection != Vector3::ZERO && mBaseAnimID != ANIM_DANCE)
        {
            // calculate actually goal direction in world based on player's key directions
            mGoalDirection += mKeyDirection.z * mCameraNode->getOrientation().zAxis();
            mGoalDirection += mKeyDirection.x * mCameraNode->getOrientation().xAxis();
            mGoalDirection.y = 0;
            mGoalDirection.normalise();

			//get the position of the next step
			auto cell=m_grid->getNode(mBodyNode->getPosition());
			if(cell!=NULL && cell->isClear())
			{
				auto next_pos = mBodyNode->getPosition() + mGoalDirection*deltaTime * RUN_SPEED * 1.5 * mAnims[mBaseAnimID]->getWeight();
			//	cout << "next_pos=" << next_pos << endl;
				auto cell = m_grid->getNode(next_pos);
				if (cell == NULL) {
					//cout << "null" << endl; 
					return;
				}
				if (cell->isClear() == false) {
			//		cout << "trapped" << endl; return;
					return;
				}
			}
			//

            Quaternion toGoal = mBodyNode->getOrientation().zAxis().getRotationTo(mGoalDirection);

            // calculate how much the character has to turn to face goal direction
            Real yawToGoal = toGoal.getYaw().valueDegrees();
            // this is how much the character CAN turn this frame
            Real yawAtSpeed = yawToGoal / Math::Abs(yawToGoal) * deltaTime * TURN_SPEED;
            // reduce "turnability" if we're in midair
            if (mBaseAnimID == ANIM_JUMP_LOOP) yawAtSpeed *= 0.2f;

            // turn as much as we can, but not more than we need to
            if (yawToGoal < 0) yawToGoal = std::min<Real>(0, std::max<Real>(yawToGoal, yawAtSpeed)); //yawToGoal = Math::Clamp<Real>(yawToGoal, yawAtSpeed, 0);
            else if (yawToGoal > 0) yawToGoal = std::max<Real>(0, std::min<Real>(yawToGoal, yawAtSpeed)); //yawToGoal = Math::Clamp<Real>(yawToGoal, 0, yawAtSpeed);
            
            mBodyNode->yaw(Degree(yawToGoal));

            // move in current body direction (not the goal direction)
            mBodyNode->translate(0, 0, deltaTime * RUN_SPEED * mAnims[mBaseAnimID]->getWeight(),
                Node::TS_LOCAL);

			//cout << "updated poosition  = " << mBodyNode->getPosition() << endl;
        }

        if (mBaseAnimID == ANIM_JUMP_LOOP)
        {
            // if we're jumping, add a vertical offset too, and apply gravity
            mBodyNode->translate(0, mVerticalVelocity * deltaTime, 0, Node::TS_LOCAL);
            mVerticalVelocity -= GRAVITY * deltaTime;
            
            Vector3 pos = mBodyNode->getPosition();
            if (pos.y <= CHAR_HEIGHT)
            {
                // if we've hit the ground, change to landing state
                pos.y = CHAR_HEIGHT;
                mBodyNode->setPosition(pos);
                setBaseAnimation(ANIM_JUMP_END, true);
                mTimer = 0;
            }
        }
    }

    void updateAnimations(Real deltaTime)
    {
        Real baseAnimSpeed = 1;
        Real topAnimSpeed = 1;

        mTimer += deltaTime;

        if (mTopAnimID == ANIM_DRAW_SWORDS)
        {
            // flip the draw swords animation if we need to put it back
            topAnimSpeed = mSwordsDrawn ? -1 : 1;

            // half-way through the animation is when the hand grasps the handles...
            if (mTimer >= mAnims[mTopAnimID]->getLength() / 2 &&
                mTimer - deltaTime < mAnims[mTopAnimID]->getLength() / 2)
            {
                // so transfer the swords from the sheaths to the hands
                mBodyEnt->detachAllObjectsFromBone();
                mBodyEnt->attachObjectToBone(mSwordsDrawn ? "Sheath.L" : "Handle.L", mSword1);
                mBodyEnt->attachObjectToBone(mSwordsDrawn ? "Sheath.R" : "Handle.R", mSword2);
                // change the hand state to grab or let go
                mAnims[ANIM_HANDS_CLOSED]->setEnabled(!mSwordsDrawn);
                mAnims[ANIM_HANDS_RELAXED]->setEnabled(mSwordsDrawn);

                // toggle sword trails
                if (mSwordsDrawn)
                {
                    mSwordTrail->setVisible(false);
                    mSwordTrail->removeNode(mSword1->getParentNode());
                    mSwordTrail->removeNode(mSword2->getParentNode());
                }
                else
                {
                    mSwordTrail->setVisible(true);
                    mSwordTrail->addNode(mSword1->getParentNode());
                    mSwordTrail->addNode(mSword2->getParentNode());
                }
            }

            if (mTimer >= mAnims[mTopAnimID]->getLength())
            {
                // animation is finished, so return to what we were doing before
                if (mBaseAnimID == ANIM_IDLE_BASE) setTopAnimation(ANIM_IDLE_TOP);
                else
                {
                    setTopAnimation(ANIM_RUN_TOP);
                    mAnims[ANIM_RUN_TOP]->setTimePosition(mAnims[ANIM_RUN_BASE]->getTimePosition());
                }
                mSwordsDrawn = !mSwordsDrawn;
            }
        }
        else if (mTopAnimID == ANIM_SLICE_VERTICAL || mTopAnimID == ANIM_SLICE_HORIZONTAL)
        {
            if (mTimer >= mAnims[mTopAnimID]->getLength())
            {
                // animation is finished, so return to what we were doing before
                if (mBaseAnimID == ANIM_IDLE_BASE) setTopAnimation(ANIM_IDLE_TOP);
                else
                {
                    setTopAnimation(ANIM_RUN_TOP);
                    mAnims[ANIM_RUN_TOP]->setTimePosition(mAnims[ANIM_RUN_BASE]->getTimePosition());
                }
            }

            // don't sway hips from side to side when slicing. that's just embarrassing.
            if (mBaseAnimID == ANIM_IDLE_BASE) baseAnimSpeed = 0;
        }
        else if (mBaseAnimID == ANIM_JUMP_START)
        {
            if (mTimer >= mAnims[mBaseAnimID]->getLength())
            {
                // takeoff animation finished, so time to leave the ground!
                setBaseAnimation(ANIM_JUMP_LOOP, true);
                // apply a jump acceleration to the character
                mVerticalVelocity = JUMP_ACCEL;
            }
        }
        else if (mBaseAnimID == ANIM_JUMP_END)
        {
            if (mTimer >= mAnims[mBaseAnimID]->getLength())
            {
                // safely landed, so go back to running or idling
                if (mKeyDirection == Vector3::ZERO)
                {
                    setBaseAnimation(ANIM_IDLE_BASE);
                    setTopAnimation(ANIM_IDLE_TOP);
                }
                else
                {
                    setBaseAnimation(ANIM_RUN_BASE, true);
                    setTopAnimation(ANIM_RUN_TOP, true);
                }
            }
        }

        // increment the current base and top animation times
        if (mBaseAnimID != ANIM_NONE) mAnims[mBaseAnimID]->addTime(deltaTime * baseAnimSpeed);
        if (mTopAnimID != ANIM_NONE) mAnims[mTopAnimID]->addTime(deltaTime * topAnimSpeed);

        // apply smooth transitioning between our animations
        fadeAnimations(deltaTime);
    }

    void fadeAnimations(Real deltaTime)
    {
        for (int i = 0; i < NUM_ANIMS; i++)
        {
            if (mFadingIn[i])
            {
                // slowly fade this animation in until it has full weight
                Real newWeight = mAnims[i]->getWeight() + deltaTime * ANIM_FADE_SPEED;
                mAnims[i]->setWeight(Math::Clamp<Real>(newWeight, 0, 1));
                if (newWeight >= 1) mFadingIn[i] = false;
            }
            else if (mFadingOut[i])
            {
                // slowly fade this animation out until it has no weight, and then disable it
                Real newWeight = mAnims[i]->getWeight() - deltaTime * ANIM_FADE_SPEED;
                mAnims[i]->setWeight(Math::Clamp<Real>(newWeight, 0, 1));
                if (newWeight <= 0)
                {
                    mAnims[i]->setEnabled(false);
                    mFadingOut[i] = false;
                }
            }
        }
    }

    void updateCamera(Real deltaTime)
    {
        // place the camera pivot roughly at the character's shoulder
        mCameraPivot->setPosition(mBodyNode->getPosition() + Vector3::UNIT_Y * CAM_HEIGHT);
        // move the camera smoothly to the goal
        Vector3 goalOffset = mCameraGoal->_getDerivedPosition() - mCameraNode->getPosition();
        mCameraNode->translate(goalOffset * deltaTime * 9.0f);
        // always look at the pivot
        mCameraNode->lookAt(mCameraPivot->_getDerivedPosition(), Node::TS_PARENT);
    }

    void updateCameraGoal(Real deltaYaw, Real deltaPitch, Real deltaZoom)
    {
        mCameraPivot->yaw(Degree(deltaYaw), Node::TS_PARENT);

        // bound the pitch
        if (!(mPivotPitch + deltaPitch > 25 && deltaPitch > 0) &&
            !(mPivotPitch + deltaPitch < -60 && deltaPitch < 0))
        {
            mCameraPivot->pitch(Degree(deltaPitch), Node::TS_LOCAL);
            mPivotPitch += deltaPitch;
        }
        
        Real dist = mCameraGoal->_getDerivedPosition().distance(mCameraPivot->_getDerivedPosition());
        Real distChange = deltaZoom * dist;

        // bound the zoom
        if (!(dist + distChange < 8 && distChange < 0) &&
            !(dist + distChange > 25 && distChange > 0))
        {
            mCameraGoal->translate(0, 0, distChange, Node::TS_LOCAL);
        }
    }

    void setBaseAnimation(AnimID id, bool reset = false)
    {
        if (mBaseAnimID != ANIM_NONE)
        {
            // if we have an old animation, fade it out
            mFadingIn[mBaseAnimID] = false;
            mFadingOut[mBaseAnimID] = true;
        }

        mBaseAnimID = id;

        if (id != ANIM_NONE)
        {
            // if we have a new animation, enable it and fade it in
            mAnims[id]->setEnabled(true);
            mAnims[id]->setWeight(0);
            mFadingOut[id] = false;
            mFadingIn[id] = true;
            if (reset) mAnims[id]->setTimePosition(0);
        }
    }

    void setTopAnimation(AnimID id, bool reset = false)
    {
        if (mTopAnimID != ANIM_NONE)
        {
            // if we have an old animation, fade it out
            mFadingIn[mTopAnimID] = false;
            mFadingOut[mTopAnimID] = true;
        }

        mTopAnimID = id;

        if (id != ANIM_NONE)
        {
            // if we have a new animation, enable it and fade it in
            mAnims[id]->setEnabled(true);
            mAnims[id]->setWeight(0);
            mFadingOut[id] = false;
            mFadingIn[id] = true;
            if (reset) mAnims[id]->setTimePosition(0);
        }
    }

    SceneNode* mBodyNode;
    SceneNode* mCameraPivot;
    SceneNode* mCameraGoal;
    SceneNode* mCameraNode;
    Real mPivotPitch;
    Entity* mBodyEnt;
    Entity* mSword1;
    Entity* mSword2;
    RibbonTrail* mSwordTrail;
    AnimationState* mAnims[NUM_ANIMS];    // master animation list
    AnimID mBaseAnimID;                   // current base (full- or lower-body) animation
    AnimID mTopAnimID;                    // current top (upper-body) animation
    bool mFadingIn[NUM_ANIMS];            // which animations are fading in
    bool mFadingOut[NUM_ANIMS];           // which animations are fading out
    bool mSwordsDrawn;
    Vector3 mKeyDirection;      // player's local intended direction based on WASD keys
    Vector3 mGoalDirection;     // actual intended direction in world-space
    Real mVerticalVelocity;     // for jumping
    Real mTimer;                // general timer to see how long animations have been playing

	Grid  * m_grid;
};

#endif
