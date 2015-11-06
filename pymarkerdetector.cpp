#include <python2.7/Python.h>
#include <algorithm>
#include <boost/python.hpp>
#include <utility>
#include <aruco/aruco.h>
#include <iostream>

#include "conversion.h"


namespace py = boost::python;

static py::tuple point_to_tuple(const cv::Point2f& point) {
    return py::make_tuple(point.x, point.y);
}

static py::list detect_markers(PyObject* mat) {

    cv::Mat in = NDArrayConverter().toMat(mat);
    py::list ret;

    aruco::MarkerDetector detector;
    std::vector<aruco::Marker> markers;
    detector.detect(in, markers);

    for (std::vector<aruco::Marker>::const_iterator marker = markers.begin();
        marker < markers.end(); ++marker) {

        long id = marker->id;
        py::tuple bounding_box = py::make_tuple(
            point_to_tuple((*marker)[0]),
            point_to_tuple((*marker)[1]),
            point_to_tuple((*marker)[2]),
            point_to_tuple((*marker)[3])
        );

        ret.append(
            py::make_tuple(id, bounding_box)
        );

    }
    return ret;
}

BOOST_PYTHON_MODULE( detector ) {

    py::def("detect", detect_markers);
}
