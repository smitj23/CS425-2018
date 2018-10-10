# CS425 Programming Assignment 3: Physics Engine #1

## Due: Oct. 21st (Sun.) 2018 at 11:59 pm

## Objective:

1. Build a simple physics engine from scratch
2. Learn to solve ODE using Euler, midpoint and Runge Kutta methods
3. Learn to resolve collisions
4. Develop a mass-spring system for simulating deformable objects

## Tasks:

In this assignment, you will be controlling an ogre like what you did in PA02. The ogre can wonder aound among obstacles and other agents and can throw balls and tetrahedra at the direction that the ogre is facing. The balls and tetrahedra have timers and will explode and kill all nearby agents (within a user defined range) when the timer expires. 

The main tasks include the implementation of two particle systems, one for bouncy balls and the other one for deformable tetrahedra. The underlying machenism of both systems is very similar but there are also many subtle differences. Details are listed below.

### OGRE Tasks (70%):

1. Implement BallPhysicsEngine class (30%)
2. Implement MassSpringPhysicsEngine class (30%)
3. Add OGRE particles for explosion effects (for both balls and tetrahedra) and kill nearby agents (10%)

### Unity Tasks (30% + 10% bonus):

1. Extend your Unity game from PA02 so the charactor can shoot out balls (timed bombs). The ball should bounce off the floor and obstacles. The ball should also explode after the timer expires and kill all nearby agent within a user defined range (excluding the ogre itself). (30%)

2. Further extend your Unity game to include explosive deformable tetrahedra (10% bonus)

### Notes:

- This assignment requires a significant amount of coding for the OGRE part, please make suer that you start as soon as possible. 


## How to Submit:
1. For the OGRE part, include your level (as your_gmu_id_PA3.txt) and all source code (*h, *cpp and CMakeLists.txt);  do not include the "build" folder  or the "sdk" folder
2. For the Unity part, video capture the screen showing the animation in mp4 format, and save it as your_gmu_id_PA3.mp4 (mp4, m4a, m4v, ...) 
3. Put everything in the corresponding folder and zip them into your_gmu_id_PA3.zip, e.g., jsmith_PA3.zip, and then submit the zip file on Blackboard.
