cmake_minimum_required(VERSION 3.0.0) 
project(LAB3)

include(CTest)
enable_testing()

# find_package(Threads REQUIRED)

add_executable(data main.c mem_allocators.c mem_allocators.h
allocs.c allocs.h)


# target_link_libraries(${PROJECT_NAME} Threads::Threads)

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
SET(CMAKE_CXX_FLAGS_DEBUG "... -lpthread")
SET(CMAKE_CXX_FLAGS_RELEASE "... -lpthread")
include(CPack)