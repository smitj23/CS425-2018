# CS425 Programming Assignment 1: Movin’

## Due: September 16th (Sunday) 2018 at 11:59 pm

## Objective:

Understand the basics of animating a game character, including playing back animation clips and positioning and orienting the character. This framework will be used in future assignments. This assignment should be done in both OGRE and Unity. 

## Tasks:

Create a program so that the charactor follows a list of randomly generately waypoints. As the charactor walks or stops, the corresponding animation clips should be used. Please remember that this is an individual programming assignment. 

### OGRE Tasks:
1. Read Agent.cpp/h. 
   - The Agent.cpp/h code provided includes the required variables and method declarations in the Agent class. Use it to setup your charactor. 
   - The update method of the Agent class includes the necessary call to updateLocomote
   - The Agent constructor includes a for loop that adds a few random points onto the agent’s mWalkList.
2. You will need to fill in code for the nextLocation and updateLocomotion methods of the Agent class.
3. While moving between the points, the character should display the run animation (top and base) and be properly oriented.
4. After the character reaches the last point, it should automatically switch to the idle animation (top and base) and continue displaying this idle behavior.
5. Your character should NOT keep looping between the points.

### Unity Tasks:
1. Create a Unity project that demonstrates the same charactor and animations as your OGRE program. It is fine that the waypoints are different from OGRE's waypoints. This means you must have
    - A charactor following a list of 10 randomly created waypoints
    - The charactor must be displaying running and stopping animations.

### Notes:
- You are free to use your own mesh and animations, as long as it uses two animation clips, namely running and stopping, and both OGRE and Unity should use the same charactor.

## How to Submit:
1. For the OGRE project, remove "build" fold and zip everything else (including CMakeLists.txt) into your_gmu_id_PA1.zip, e.g., jsmith_PA1.zip, and submit it through Blackboard by the due date.
2. For the Unity project, video capture the screen showing the animation in mp4 format, and save it as your_gmu_id_PA1.mp4 (mp4, m4a, m4v, ...) and submit it through Blackboard by the due date.
