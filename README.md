# PyAruco

Basic python wrapper against [aruco](http://www.uco.es/investiga/grupos/ava/node/26) library

Only marker detection is supported, camera parameters and thus marker location are ignored

# Setup

* configure with `cmake ..`, be sure that opencv(-dev) and aruco are installed. 
Place 'Findaruco.cmake' in '/usr/local/share/aruco' or specify ARUCO_MODULE_PATH varaible. 
Also libboost python and numpy libraries and headers are required

* build with `make`

* install with `sudo make install` it will install numpyconversions library and pyaruco python package

# Run

```python
In [4]: d = cv2.imread('./1.png', 0)

In [5]: import pyaruco.detector as det

In [6]: det.detect(d)
Out[6]: 
[(1,
  ((495.0002136230469, 431.0001220703125),
   (828.0, 430.9998474121094),
   (828.0000610351562, 764.0000610351562),
   (494.9999084472656, 764.0000610351562)))]
```

Returned value of `pyaruco.detector.detect` is list of detected markers. Where each marker is tuple of its 
id and corners. Corners are 4-tuple of corner (x,y) pairs. First is TL in clockwise order.
