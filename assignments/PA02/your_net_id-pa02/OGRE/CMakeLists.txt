cmake_minimum_required (VERSION 2.8)
project(CS425-PA02)

## [discover_ogre]
# specify which version and components you need
find_package(OGRE 1.11 REQUIRED COMPONENTS Bites RTShaderSystem)

# copy resource.cfg next to our binaries where OGRE looks for it
file(COPY ${OGRE_CONFIG_DIR}/resources.cfg DESTINATION ${CMAKE_BINARY_DIR})
## [discover_ogre]

add_executable(PA02 pa02.cpp Agent.cpp Grid.cpp )
target_link_libraries(PA02 ${OGRE_LIBRARIES})


