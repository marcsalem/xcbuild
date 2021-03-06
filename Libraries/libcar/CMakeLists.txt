# Copyright 2013-present Facebook. All Rights Reserved.

add_library(car SHARED
            Sources/Reader.cpp
            Sources/AttributeList.cpp
            Sources/Facet.cpp
            Sources/Rendition.cpp
            Sources/car_format.c
            )

find_package(ZLIB REQUIRED)
target_include_directories(car PRIVATE "${ZLIB_INCLUDE_DIR}")
target_link_libraries(car PRIVATE ${ZLIB_LIBRARIES})

find_library(COMPRESSION compression)
if ("${COMPRESSION}" STREQUAL "COMPRESSION-NOTFOUND")
  set(COMPRESSION "")
endif ()

target_link_libraries(car PUBLIC ext bom ${COMPRESSION})
target_include_directories(car PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/Headers")
install(TARGETS car DESTINATION lib)

add_executable(dump_car Tools/dump_car.cpp)
target_link_libraries(dump_car PRIVATE car)
target_link_libraries(dump_car PRIVATE ${ZLIB_LIBRARIES})
