CXX=g++
CXXFLAGS= -std=c++11

OUT=picguy

all: PicGuy.o Photo.o PhotoFormats.o
	$(CXX) $^ -o $(OUT) $(CXXFLAGS)

PhotoFormats.o: src/PhotoFormats.cpp
	$(CXX) -o $@ -c $< $(CXXFLAGS)

Photo.o: src/Photo.cpp
	$(CXX) -o $@ -c $< $(CXXFLAGS)

PicGuy.o: src/PicGuy.cpp
	$(CXX) -o $@ -c $< $(CXXFLAGS)


clean:
	rm -f *.o
