project(ABMotionPlanning)
cmake_minimum_required(VERSION 2.8)
#aux_source_directory(. SRC_LIST)
#add_executable(${PROJECT_NAME} ${SRC_LIST})

set(CMAKE_CXX_FLAGS "-O2")        # Optimize
SET(CMAKE_CXX_FLAGS "-Wall")

find_package(OpenCV REQUIRED)
find_package(OpenGL REQUIRED)
find_package(GLUT REQUIRED)
include_directories( ${OpenCV_INCLUDE_DIRS} ${OPENGL_INCLUDE_DIRS}  ${GLUT_INCLUDE_DIRS} )

find_package(SDL REQUIRED)
find_package(SDL_image REQUIRED)
find_package(SDL_ttf REQUIRED)
include_directories(${SDL_INCLUDE_DIR})
include_directories(${SDLIMAGE_INCLUDE_DIR})  
include_directories(${SDLTTF_INCLUDE_DIR})

if(NOT SDL_FOUND)
    message( FATAL ERROR "SDL not found!")
endif(NOT SDL_FOUND)

if(NOT OpenCV_FOUND)
	message(FATAL ERROR "OpenCV no se encuentra")
endif(NOT OpenCV_FOUND)

#set the default path for built executables to the "bin" directory
set(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/bin)

file(GLOB_RECURSE SOURCES "src/*.cpp", "src/*.c", "bin/*.frag", "bin/*.vert")
file(GLOB_RECURSE HEADERS "include/*.h")



set (INCLUDE_DIRS "")
foreach (_headerFile ${HEADERS})
    get_filename_component(_dir ${_headerFile} PATH)
    list (APPEND INCLUDE_DIRS ${_dir})
endforeach()
list(REMOVE_DUPLICATES INCLUDE_DIRS)

include_directories(${INCLUDE_DIRS})

include_directories(/usr/local/include)



set(EXEC main)


foreach(program ${EXEC})
  add_executable(${program} ${HEADERS} ${SOURCES} ${program}.cpp)
  target_link_libraries(${program} ${OpenCV_LIBS} ${OPENGL_LIBRARIES} ${GLUT_LIBRARY}
                                   ${SDL_LIBRARY}
                                   ${SDLIMAGE_LIBRARY}
				   ${SDLTTF_LIBRARY}
                                   SDLmain
                       )
endforeach(program)
