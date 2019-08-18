cmake_minimum_required(VERSION 3.12)
project(algorithms)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED TRUE)

function(collect_files lst_files root)
	file(GLOB_RECURSE lst_files_local ${root}/*.cpp ${root}/*.c ${root}/*.h ${root}/*.hpp)
	set(${lst_files} ${lst_files_local} PARENT_SCOPE)
endfunction()

collect_files(sources Src)
collect_files(external External)
collect_files(sources_test Tests)

add_executable(tests ${sources_test} ${sources} ${external})
target_include_directories(tests PUBLIC Tests Src External)