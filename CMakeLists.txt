cmake_minimum_required(VERSION 3.6)

set(CMAKE_CXX_COMPILER g++-4.8)
set(CMAKE_CXX_STANDARD 11)

project(algorithms)

include_directories(${PROJECT_SOURCE_DIR}/src)

# Helpers only to make CLion do all its magic in those files :)
add_library(util src/util/io.cc src/util/grouped_radix.cc)
add_library(struts src/struts/geometry.cc src/struts/groups.cc src/struts/heap.cc)

add_executable(u10098 src/uva/contest/10098_fast_permutations.cc)
add_executable(u10249 src/uva/contest/10249_grand_dinner.cc)
add_executable(u10078 src/uva/contest/10078_art_gallery.cc)
add_executable(u10147 src/uva/contest/10147_highways.cc)
add_executable(u10702 src/uva/contest/10702_travelling_salesman.cc)
add_executable(u10803 src/uva/contest/10803_thunder_mountain.cc)
add_executable(u11475 src/uva/contest/11475_extend_palindrome.cc)

add_executable(u105_easy src/uva/sets/105_skyline_easy.cc)
add_executable(u105 src/uva/sets/105_skyline.cc)
add_executable(u108 src/uva/sets/108_max_sum.cc)
add_executable(u111 src/uva/sets/111_hist_grading.cc)
add_executable(u644 src/uva/sets/644_imm_decod.cc)
add_executable(u775 src/uva/sets/775_hamiltonian.cc)
#add_executable(u775_nice src/uva/sets/775_hamiltonian_nice.cc)
add_executable(u374 src/uva/sets/374_bigmod.cc)
add_executable(u424 src/uva/sets/424_int_inquiry.cc)
add_executable(u452 src/uva/sets/452_project_scheduling.cc)
add_executable(u681 src/uva/sets/681_convex_hull.cc)
add_executable(u820 src/uva/sets/820_internet_bandwith.cc)
