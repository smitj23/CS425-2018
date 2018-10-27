# CS425 Programming Assignment 4: Physics Engine #2

## Due: Oct. 31st (Wed.) 2018 at 11:59 pm

## Objective:

1. Implement Midpoint method or Runge-Kutta 3rd Order method
2. Develop a mass-spring system for simulating deformable objects

## Tasks:

In this assignment, you will be controlling an ogre like what you did in PA02. The ogre can wonder aound among obstacles and other agents and can throw balls and tetrahedra at the direction that the ogre is facing. The balls and tetrahedra have timers and will explode and kill all nearby agents (within a user defined range) when the timer expires. 

The main tasks include the implementation of two particle systems, one for bouncy balls and the other one for deformable tetrahedra. The underlying machenism of both systems is very similar but there are also many subtle differences. Details are listed below.

### OGRE Tasks (100%):

1. Implement MassSpringPhysicsEngine class (90%)
  - Create tetrahedra (20%)
  - ApplyForce (20%)
  - Implement Midpoint method or Runge-Kutta 3rd Order method (30%)
  - Step (20%)
2. Add OGRE particles for explosion effects for tetrahedra and kill nearby agents (10%)

### Unity Tasks (10% bonus):

1. Extend your Unity game from PA03 so the charactor can shoot out tetrahedra. The tetrahedra should bounce off the floor and obstacles. The tetrahedra should also explode after the timer expires and kill all nearby agent within a user defined range (excluding the ogre itself). (10%)

### Notes:

- This assignment requires a significant amount of coding for the OGRE part, please make suer that you start as soon as possible. 


## How to Submit:
1. For the OGRE part, include your level (as your_gmu_id_PA4.txt) and all source code (*h, *cpp and CMakeLists.txt);  do not include the "build" folder  or the "sdk" folder
2. For the **optional** Unity part, video capture the screen showing the animation in mp4 format, and save it as your_gmu_id_PA4.mp4 (mp4, m4a, m4v, ...) 
3. Put everything in the corresponding folder and zip them into your_gmu_id_PA4.zip, e.g., jsmith_PA4.zip, and then submit the zip file on Blackboard.
