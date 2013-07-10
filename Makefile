CC = g++
OBJS = facetrack_demo.o face.o
DEBUG = -g
CFLAGS = -Wall -c $(DEBUG)
OPENCV = `pkg-config --cflags --libs opencv` 

factrack_demo: $(OBJS)
	$(CC) $(CFLAGS) -o facetrack_demo $(OBJS) $(OPENCV)
facetrack_demo.o: facetrack_demo.cpp face.h
	$(CC) $(CFLAGS) facetrack_demo.cpp $(OPENCV)
face.o: face.cpp face.h
	$(CC) $(CFLAGS) face.cpp $(OPENCV)