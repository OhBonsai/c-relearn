cmake_minimum_required(VERSION 3.15)
project(redis_learn C)

set(CMAKE_C_STANDARD 99)


add_executable(redis_learn main.c sds.h sds.c zmalloc.h zmalloc.c adlist.c adlist.h dict.c dict.h)

add_executable(test_sds test/test_sds.c sds.h sds.c zmalloc.c zmalloc.h)
