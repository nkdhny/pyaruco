project(runtagdetector)
cmake_minimum_required(VERSION 2.8)

if(NOT ARUCO_MODULE_PATH)
    set(ARUCO_MODULE_PATH "C:/Program Files (x86)/aruco/lib/cmake")
endif()

list (APPEND CMAKE_MODULE_PATH "${ARUCO_MODULE_PATH}")

message(STATUS ${CMAKE_MODULE_PATH} )

find_package( OpenCV REQUIRED )
set (REQUIRED_LIBRARIES ${OpenCV_LIBS})
INCLUDE_DIRECTORIES(${OpenCV_INCLUDE_DIRS})


find_package( aruco REQUIRED )

find_program(PYTHON "python")
find_package( PythonLibs REQUIRED )

include_directories(${PYTHON_INCLUDE_DIRS})
if(WIN32)
  if(NOT BOOST_ROOT)
    set(BOOST_ROOT "C:/boost_1_55_0")
  endif()
  if(NOT BOOST_ARCH)
    set(BOOST_ARCH "-vc120-1_55")
  endif()
  if(NOT BOOST_TCH)
    set(BOOST_TCH "-msvc-12.0")
  endif()
  include_directories(${PYTHON_INCLUDE_DIRS}/../Lib/site-packages/numpy/core/include)
  include_directories(${BOOST_ROOT})
  link_directories(${BOOST_ROOT}/lib32${BOOST_TCH})
  message(STATUS ${BOOST_ROOT} )
endif()

message(STATUS ${PYTHON_LIBRARIES})
message(STATUS ${OpenCV_LIBS})

set(LIBRARY_OUTPUT_PATH ${CMAKE_BINARY_DIR}/output/ CACHE PATH "Path for libraries")
GET_DIRECTORY_PROPERTY(INCLUDE_DIR INCLUDE_DIRECTORIES)
GET_DIRECTORY_PROPERTY(LINK_DIR LINK_DIRECTORIES)

add_library(numpyconversions SHARED conversion.cpp)
target_link_libraries(numpyconversions ${OpenCV_LIBS} boost_python${BOOST_ARCH} ${PYTHON_LIBRARIES})
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
