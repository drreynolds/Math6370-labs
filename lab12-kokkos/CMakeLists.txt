cmake_minimum_required (VERSION 3.10)
project (lab12Kokkos)

set(SPACK_CXX g++)
if(SPACK_CXX)
  message("found spack compiler g++")
  set(CMAKE_CXX_COMPILER g++ CACHE STRING "the C++ compiler" FORCE)
  set(ENV{CXX} g++)
endif()

set(Kokkos_DIR "$ENV{Kokkos_ROOT}" CACHE STRING "Kokkos root directory")
find_package(Kokkos REQUIRED)

add_executable(1_basics.exe 1_basics.cpp)
target_link_libraries(1_basics.exe Kokkos::kokkos)

add_executable(2_views.exe 2_views.cpp)
target_link_libraries(2_views.exe Kokkos::kokkos)

add_executable(3_data_transfers.exe 3_data_transfers.cpp)
target_link_libraries(3_data_transfers.exe Kokkos::kokkos)

add_executable(4_spaces_layouts.exe 4_spaces_layouts.cpp)
target_link_libraries(4_spaces_layouts.exe Kokkos::kokkos)
