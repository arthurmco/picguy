CXX=g++

GTKLIBS= $(shell pkg-config --libs gtk+-3.0)
CLIBS= -ljpeg -lpng 

GTKFLAGS= $(shell pkg-config --cflags gtk+-3.0)
CXXFLAGS=  -std=c++11 -g $(CLIBS)

OUT=picguy

all: PicGuy.o Photo.o JPEGPhoto.o PNGPhoto.o PhotoFormats.o PhotoGroup.o
	$(CXX) $^ -o $(OUT) $(GTKFLAGS) $(CXXFLAGS) $(GTKLIBS)

PhotoFormats.o: src/PhotoFormats.cpp src/PhotoFormats.hpp
	$(CXX) -o $@ -c $<  $(CXXFLAGS)

Photo.o: src/Photo.cpp src/PhotoFormats.hpp src/Pixel.hpp
	$(CXX) -o $@ -c $< $(CXXFLAGS)

JPEGPhoto.o: src/JPEGPhoto.cpp src/JPEGPhoto.hpp
	$(CXX) -o $@ -c $< $(CXXFLAGS)

PNGPhoto.o: src/PNGPhoto.cpp src/PNGPhoto.hpp
	$(CXX) -o $@ -c $< $(CXXFLAGS)

PhotoGroup.o: src/PhotoGroup.cpp src/PhotoGroup.hpp
	$(CXX) -o $@ -c $< $(CXXFLAGS)

PicGuy.o: src/PicGuy.cpp 
	$(CXX) -o $@ -c $< $(GTKFLAGS) $(CXXFLAGS) $(GTKLIBS)


clean:
	rm -f *.o
