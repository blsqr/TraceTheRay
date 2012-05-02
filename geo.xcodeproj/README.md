TraceTheRay
===========

This is a raytracing 3D-renderer, as the name suggests. It is one of my first C++ projects, and there sure is a lot more to learn, so I am thankful for every kind of tip.
The current version is 0.5, but is still a very early stage of development and there are still plenty of features missing as well as bugs here and there.

I am on Mac OS X Snow Leopard, am using g++ to compile and run it from the terminal. I guess on other platforms errors might occur...




Changelog
=========
	- up to v0.5:	(not in git) basic development



Features so far:
================
2D:	- create a PixelMap, draw lines on it and save as bmp
3D:	- create polygons and trace them
	- set up a camera
	- output traced polygons as bmp
	


Known bugs:
===========
	- drawn line is not always correct
	- outer pixels of PixelMap have wrong coordinates
	- Camera: calculating the needed vectors is wrong for direction parallel to z-axis
	- render output is still not correct