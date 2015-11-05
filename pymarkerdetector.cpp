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

    std::for_each(
        markers.begin(), markers.end(),
        [&](aruco::Marker marker) -> void {
            long id = marker.id;

            std::vector<py::tuple> pts(4);
            std::transform(marker.begin(), marker.end(), pts.begin(),
                           [](cv::Point2f p) { return point_to_tuple(p);});

            py::tuple bounding_box = py::make_tuple(
                pts[0], pts[1], pts[2], pts[3]
            );

            ret.append(
                py::make_tuple(id, bounding_box)
            );
        }
    );

    return ret;
}

BOOST_PYTHON_MODULE( detector ) {

    py::def("detect", detect_markers);
}
