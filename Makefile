CXX=g++

LIBS= $(shell pkg-config --libs gtk+-3.0) -L/usr/lib/x86_64-linux-gnu -l:libpng.a -l:libjpeg.a -l:libz.a
#CLIBS=-ljpeg -lpng

GTKFLAGS= $(shell pkg-config --cflags gtk+-3.0)
CXXFLAGS=  -std=c++11 -g

OUT=picguy

all: PicGuy.o Photo.o JPEGPhoto.o PNGPhoto.o PhotoFormats.o PhotoGroup.o \
	ThumbnailCache.o PhotoGroupSerializer.o PixelConverter.o \
	PhotoOperationManager.o BlackAndWhiteOperation.o
	$(CXX) $^ -o $(OUT) $(GTKFLAGS) $(CXXFLAGS) $(LIBS)

PhotoFormats.o: src/PhotoFormats.cpp src/PhotoFormats.hpp
	$(CXX) -o $@ -c $<  $(CXXFLAGS)

Photo.o: src/Photo.cpp src/PhotoFormats.hpp src/Pixel.hpp
	$(CXX) -o $@ -c $< $(CXXFLAGS)

JPEGPhoto.o: src/JPEGPhoto.cpp src/JPEGPhoto.hpp
	$(CXX) -o $@ -c $< $(CXXFLAGS) $(LIBS)

PNGPhoto.o: src/PNGPhoto.cpp src/PNGPhoto.hpp
	$(CXX) -o $@ -c $< $(CXXFLAGS) $(LIBS)

PhotoGroup.o: src/PhotoGroup.cpp src/PhotoGroup.hpp
	$(CXX) -o $@ -c $< $(CXXFLAGS)

PhotoGroupSerializer.o: src/PhotoGroupSerializer.cpp src/PhotoGroupSerializer.hpp
	$(CXX) -o $@ -c $< $(CXXFLAGS)

ThumbnailCache.o: src/ThumbnailCache.cpp src/ThumbnailCache.hpp
	$(CXX) -o $@ -c $< $(GTKFLAGS) $(CXXFLAGS) $(LIBS)

PixelConverter.o: src/PixelConverter.cpp src/PixelConverter.hpp
	$(CXX) -o $@ -c $< $(GTKFLAGS) $(CXXFLAGS) $(LIBS)

PhotoOperationManager.o: src/operations/PhotoOperationManager.cpp \
	src/operations/PhotoOperationManager.hpp src/operations/PhotoOperation.hpp
		$(CXX) -o $@ -c $< $(GTKFLAGS) $(CXXFLAGS) $(LIBS)

BlackAndWhiteOperation.o: src/operations/BlackAndWhiteOperation.cpp \
	src/operations/BlackAndWhiteOperation.hpp
		$(CXX) -o $@ -c $< $(GTKFLAGS) $(CXXFLAGS) $(LIBS)

PicGuy.o: src/PicGuy.cpp
	$(CXX) -o $@ -c $< $(GTKFLAGS) $(CXXFLAGS) $(LIBS)

clean:
	rm -f *.o
