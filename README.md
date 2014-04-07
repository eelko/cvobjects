cvobjects
=========

Object oriented pipeline functionality for opencv

CVObjects aims to make a modular framework for use with `OpenCV <http://opencv.org/>`_.
This software should enable you to quickly and interactivly create an image processing filter, based on `OpenCV <http://opencv.org/>`_ without writing any code.
The resulting C++ object can save and load its settings as an xml file, be combined into a pipeline, interactivly tuned using the commandline, and be used directly in your own code.

To minimize restrictions for using this code, we use the `boost license <http://www.boost.org/LICENSE_1_0.txt>`_, which only requires the copyright notice to be included in sourcecode distributions.
Furthermore we use `Tinyxml <http://www.grinninglizard.com/tinyxmldocs/index.html>`_, that is licensed unther the zlib license, which is similar. 