cmake_minimum_required(VERSION 3.0.0)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -pthread")

project(AudioLib VERSION 0.1.0)



include(CTest)
enable_testing()


set(SRC_FILES 
# audio
${PROJECT_SOURCE_DIR}/src/recorder.cpp
${PROJECT_SOURCE_DIR}/src/fft_processor.cpp
${PROJECT_SOURCE_DIR}/dependencies/kissfft/kiss_fft.c
${PROJECT_SOURCE_DIR}/src/Audio.cpp
)

set(TARGET AudioLib)

find_package(ALSA REQUIRED)

add_library(
    ${TARGET} 
    ${SRC_FILES}
)

target_include_directories( 
${TARGET} PUBLIC
${PROJECT_SOURCE_DIR}/include
${PROJECT_SOURCE_DIR}/dependencies/kissfft
)

if (ALSA_FOUND)                                                
    include_directories(${ALSA_INCLUDE_DIRS} )                  
    target_link_libraries (${TARGET} ${ALSA_LIBRARIES})      
endif(ALSA_FOUND)  

target_link_libraries(${TARGET} -lpthread -lm)


# create test app
add_executable(Test test.cpp)
target_link_libraries(Test LINK_PUBLIC ${TARGET})


set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)
