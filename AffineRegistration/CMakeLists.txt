cmake_minimum_required(VERSION 2.8)

project (AffineReg)

find_package (ITK REQUIRED)
include (${ITK_USE_FILE})

add_executable (AffineReg AffineReg.cxx)

target_link_libraries (AffineReg ${ITK_LIBRARIES})
