.PHONY: default cmake build tags clean run debug

ifeq ($(OS), Windows_NT)
CMAKE_ARGS= -DSFML_DIR=$(SFML_DIR) -DGLM_DIR=$(GLM_DIR)
BUILD_CMD=MSBuild ecs.sln /p:Configuration=$(BUILD_TYPE) /t:main
CLEAN_CMD=rd /q /s build
RUN_CMD=./build/$(BUILD_TYPE)/main.exe
else
CMAKE_ARGS=-DCMAKE_BUILD_TYPE=$(BUILD_TYPE)
BUILD_CMD=make
CLEAN_CMD=rm -rf build
RUN_CMD=./build/main
DEBUG_CMD=lldb build/main
endif

include configure.mk

default:
	make build && make run

cmake: configure.mk
	cd build && cmake ../ $(CMAKE_ARGS)

build:
	cd build && $(BUILD_CMD)

tags:
	ctags -R src

clean:
	$(CLEAN_CMD)

run:
	$(RUN_CMD)

debug:
	$(DEBUG_CMD)
