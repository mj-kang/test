CC = arm-linux-gnueabihf-gcc
CXX = arm-linux-gnueabihf-g++
CFLAGS = 
LDFALGS = -pthread
.SUFFIXES: .cpp .o
.cpp.o: 
	$(CXX) -c $(CFLAGS) $<

TARGET: cam_V4L2

all: $(TARGET)

cam_V4L2: CamWithV4L2.o main.o
	$(CXX) -o $@ $(LDFALGS) $^

clean:
	rm -rf *.o $(TARGET)
