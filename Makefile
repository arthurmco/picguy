CXX=g++
CXXFLAGS= -std=c++11 -g

OUT=picguy

all: PicGuy.o Photo.o JPEGPhoto.o PhotoFormats.o
	$(CXX) $^ -o $(OUT) $(CXXFLAGS)

PhotoFormats.o: src/PhotoFormats.cpp src/PhotoFormats.hpp
	$(CXX) -o $@ -c $< $(CXXFLAGS)


Photo.o: src/Photo.cpp src/PhotoFormats.hpp src/Pixel.hpp
	$(CXX) -o $@ -c $< $(CXXFLAGS)

JPEGPhoto.o: src/JPEGPhoto.cpp src/JPEGPhoto.hpp
	$(CXX) -o $@ -c $< $(CXXFLAGS)



PicGuy.o: src/PicGuy.cpp 
	$(CXX) -o $@ -c $< $(CXXFLAGS)


clean:
	rm -f *.o
