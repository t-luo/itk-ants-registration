cmake_minimum_required(VERSION 2.8)

project (AverageImages)

find_package (ITK REQUIRED)
include (${ITK_USE_FILE})

add_executable (AverageImages AverageImages.cxx)

target_link_libraries (AverageImages ${ITK_LIBRARIES})
