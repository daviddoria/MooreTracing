cmake_minimum_required(VERSION 2.6)

PROJECT(MooreTracing)

# Specify where to copy executables when 'make install' is run
SET( INSTALL_DIR ${CMAKE_INSTALL_PREFIX} )

# Tell CMake to also look in the source directory to find some .cmake files (Eigen3 and Umfpack)
set(CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR} ${CMAKE_MODULE_PATH})

FIND_PACKAGE(ITK REQUIRED)
INCLUDE(${ITK_USE_FILE})

FIND_PACKAGE(VTK REQUIRED)
INCLUDE(${VTK_USE_FILE})

ADD_EXECUTABLE(MooreTracingDemo MooreTracingDemo.cpp MooreTracing.cpp)
TARGET_LINK_LIBRARIES(MooreTracingDemo ${ITK_LIBRARIES} ${VTK_LIBRARIES})
set_target_properties(MooreTracingDemo PROPERTIES COMPILE_DEFINITIONS "${DEFINITIONS}")
INSTALL( TARGETS MooreTracingDemo RUNTIME DESTINATION ${INSTALL_DIR} )

ADD_EXECUTABLE(MooreTracingExample MooreTracingExample.cpp MooreTracing.cpp)
TARGET_LINK_LIBRARIES(MooreTracingExample ${ITK_LIBRARIES} ${VTK_LIBRARIES})
set_target_properties(MooreTracingExample PROPERTIES COMPILE_DEFINITIONS "${DEFINITIONS}")
INSTALL( TARGETS MooreTracingExample RUNTIME DESTINATION ${INSTALL_DIR} )
