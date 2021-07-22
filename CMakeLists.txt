cmake_minimum_required(VERSION 3.16)
project(testphil C)

set(CMAKE_C_STANDARD 11)

add_executable(testphil main.c philo.h philo_ultis.c philo_checkfiles.c)

set(THREADS_PREFER_PTHREAD_FLAG ON)
find_package(Threads REQUIRED)
target_link_libraries(testphil Threads::Threads)