cmake_minimum_required(VERSION 2.8)

project (DeformableReg)

find_package (ITK REQUIRED)
include (${ITK_USE_FILE})

add_executable (DeformableReg DeformableReg.cxx)

target_link_libraries (DeformableReg ${ITK_LIBRARIES})
