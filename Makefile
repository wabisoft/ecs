.PHONY: configure make_build_directory cmake build tags clean run

ifeq ($(OS), Windows_NT)
CMAKE_ARGS= -DSFML_DIR=$(SFML_DIR) -DGLM_DIR=$(GLM_DIR)
CONF_CMD=configure.cmd
BUILD_CMD=MSBuild ecs.sln /p:Configuration=$(BUILD_TYPE) /t:main
CLEAN_CMD=rd /q /s build
RUN_CMD=./build/$(BUILD_TYPE)/main.exe
else
CMAKE_ARGS=-DCMAKE_BUILD_TYPE=$(BUILD_TYPE)
CONF_CMD=sh -c ". configure.sh"
BUILD_CMD=make
CLEAN_CMD=rm -rf build
RUN_CMD=./build/main
endif


configure:
	$(CONF_CMD)

cmake: configure
	cd build && cmake ../ $(CMAKE_ARGS)

build:
	cd build && $(BUILD_CMD)

tags:
	ctags -R src

clean:
	$(CLEAN_CMD)

run:
	$(RUN_CMD)
