.PHONY: cmake build tags

cmake:
	mkdir -p build
	cd build && cmake ../ -DCMAKE_BUILD_TYPE=Debug

build:
	cd build && make

tags:
	ctags -R src

