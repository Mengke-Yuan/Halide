include ../support/Makefile.inc

CXXFLAGS += -g -Wall

.PHONY: clean

interpolate: interpolate.cpp
	$(CXX) $(CXXFLAGS) interpolate.cpp $(LIB_HALIDE) -o interpolate \
	$(PNGFLAGS) $(LDFLAGS) $(LLVM_SHARED_LIBS)

out.png: interpolate
	./interpolate ../images/rgba.png out.png

clean:
	rm -f interpolate interpolate.h out.png
