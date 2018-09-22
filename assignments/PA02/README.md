# CS425 Programming Assignment 2: Stop!

## Due: September 30th (Sunday) 2018 at 11:59 pm

## Objective:

1. Learn how to create a level populated with static and dynamic objects and load the level into the game.
2. Use a grid-based scene manager to detect the proximity of the objects and characters in the scene. 
3. Learn how collision detection is done in OGRE. 

## Tasks:

In this assignment, you will be controlling an ogre; Your task is to
modify the given code so that the ogre cannot penetrate through objects and other moving agents;
Also modify the code so that the agents keep moving to randomly generated waypoints without stopping. 

Please remember that this is an individual programming assignment. 

### OGRE Tasks (70 pts + 20 bonus pts):

- Create a level that is at least 50x50; (10 pts)
- Modify the code so that the agents can keep moving to randomly generated waypoints without stopping. (10 pts)
- Use Grid class to detect if the user-controlled ogre overlaps with an object or a moving agent. You need to make sure that the cells occupied by the moving agents are correctly marked. (30 pts)
- Modify the code so that, the user-controlled ogre will not move into a cell that would cause collision. (20 pts)
- **Bonus**: You might notice that, for larger models, the grid cells are not correctly marked as occupied. Fix this problem using OGRE's collision detection methods. (20 pts)

### Unity Tasks (30 pts):
- Create a similar level in Unity: with the same number of objects and moving agents. The models and animations of these objects and agents need not be identical to your OGRE scene.
- Add a user-controlled character and make sure that it cannot penetrate through objects and moving agents. 

### Notes:
- With bonus, the total points of this assignment is 120.

## How to Submit:
1. For the OGRE part, include your level (as your_gmu_id_PA2.txt) and all source code (*h, *cpp and CMakeLists.txt);  do not include the "build" folder  or the "sdk" folder
2. For the Unity part, video capture the screen showing the animation in mp4 format, and save it as your_gmu_id_PA2.mp4 (mp4, m4a, m4v, ...) 
3. Put everything in the corresponding folder and zip them into your_gmu_id_PA2.zip, e.g., jsmith_PA2.zip, and then submit the zip file on Blackboard.
