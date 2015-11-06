project(runtagdetector)
cmake_minimum_required(VERSION 2.8)

if(NOT ARUCO_MODULE_PATH)
    set(ARUCO_MODULE_PATH "/usr/local/share/aruco")
endif()

list (APPEND CMAKE_MODULE_PATH "${ARUCO_MODULE_PATH}")

message(STATUS ${CMAKE_MODULE_PATH} )

find_package( OpenCV REQUIRED )
find_package( aruco REQUIRED )

find_program(PYTHON "python")

set(LIBRARY_OUTPUT_PATH ${CMAKE_BINARY_DIR}/output/ CACHE PATH "Path for libraries")
GET_DIRECTORY_PROPERTY(INCLUDE_DIR INCLUDE_DIRECTORIES)
GET_DIRECTORY_PROPERTY(LINK_DIR LINK_DIRECTORIES)

add_library(numpyconversions SHARED conversion.cpp)
target_link_libraries(numpyconversions ${OPENCV_LIBS} boost_python)
install(TARGETS numpyconversions DESTINATION lib)

if (PYTHON)
    set(SETUP_PY_IN "${CMAKE_CURRENT_SOURCE_DIR}/setup.py.in")
    set(SETUP_PY    "${CMAKE_CURRENT_BINARY_DIR}/setup.py")
    set(DEPS        "${CMAKE_CURRENT_SOURCE_DIR}/pyaruco/__init__.py")
    set(OUTPUT      "${CMAKE_CURRENT_BINARY_DIR}/build/timestamp")

    configure_file(${SETUP_PY_IN} ${SETUP_PY})
    file(COPY pyaruco DESTINATION ${CMAKE_CURRENT_BINARY_DIR})


    add_custom_command(OUTPUT ${OUTPUT}
                       COMMAND ${PYTHON} ${SETUP_PY} build
                       COMMAND ${CMAKE_COMMAND} -E touch ${OUTPUT}
                       DEPENDS ${DEPS})

    add_custom_target(pymarkerdetector ALL DEPENDS ${OUTPUT})

    install(CODE "execute_process(COMMAND ${PYTHON} ${SETUP_PY} install)")
endif()
