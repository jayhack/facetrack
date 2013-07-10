CC = g++
OBJS = facetrack_demo.o face.o
DEBUG = -g
LFLAGS = -Wall -c $(DEBUG)

OPENCV_CFLAGS = `pkg-config --cflags opencv`
OPENCV_LIBS = `pkg-config --libs opencv`


factrack_demo: $(OBJS)
	$(CC) -o facetrack_demo $(OBJS) $(OPENCV_CFLAGS) $(OPENCV_LIBS)
facetrack_demo.o: facetrack_demo.cpp face.h
	$(CC) $(LFLAGS) facetrack_demo.cpp $(OPENCV_CFLAGS) $(OPENCV_LIBS)
face.o: face.cpp face.h
	$(CC) $(LFLAGS) face.cpp $(OPENCV_CFLAGS) $(OPENCV_LIBS)