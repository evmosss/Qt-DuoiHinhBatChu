# Additional clean files
cmake_minimum_required(VERSION 3.16)

if("${CONFIG}" STREQUAL "" OR "${CONFIG}" STREQUAL "Debug")
  file(REMOVE_RECURSE
  "CMakeFiles\\DuoiHinhBatChu_Backend_autogen.dir\\AutogenUsed.txt"
  "CMakeFiles\\DuoiHinhBatChu_Backend_autogen.dir\\ParseCache.txt"
  "DuoiHinhBatChu_Backend_autogen"
  )
endif()
