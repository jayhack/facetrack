To compile: (make sure opencv is installed, PKG_CONFIG_PATH is set properly...)
g++ facetrack.cpp -o facetrack `pkg-config --cflags --libs opencv`