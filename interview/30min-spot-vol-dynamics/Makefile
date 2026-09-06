# Convenience wrapper. Prefer g++ so we don't pick up a clang that cannot
# link libstdc++ in some environments.
CXX ?= g++

.PHONY: all solution test clean

all:
	cmake -S . -B build -DCMAKE_CXX_COMPILER=$(CXX)
	cmake --build build

solution:
	cmake -S . -B build-sol -DCMAKE_CXX_COMPILER=$(CXX) -DUSE_REFERENCE_SOLUTION=ON
	cmake --build build-sol

test: all
	./build/test_properties

clean:
	rm -rf build build-sol
