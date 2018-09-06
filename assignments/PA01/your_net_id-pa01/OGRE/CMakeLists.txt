cmake_minimum_required (VERSION 2.8)
project(CS425PA01)

## [discover_ogre]
# specify which version and components you need
find_package(OGRE 1.11 REQUIRED COMPONENTS Bites RTShaderSystem)

# copy resource.cfg next to our binaries where OGRE looks for it
file(COPY ${OGRE_CONFIG_DIR}/resources.cfg DESTINATION ${CMAKE_BINARY_DIR})
## [discover_ogre]

add_executable(PA01 pa01.cpp Agent.cpp )
target_link_libraries(PA01 ${OGRE_LIBRARIES})


