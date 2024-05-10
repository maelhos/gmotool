# my everyday makefile
.PHONY: default
default: build

cpp_source_files := $(shell find src -name *.cpp)
cpp_object_files := $(patsubst src/%.cpp, build/%.o, $(cpp_source_files))

CPP_INCLUDE_PATH := include

OUT_NAME := gmotool

LIBS := -lm -lgmp -lgmpxx -lmpfr -fopenmp
OPTS := -march=znver3 -g -O3

$(cpp_object_files): build/%.o : src/%.cpp
	@mkdir -p $(dir $@) && \
	g++ $(OPTS) -c -Wall -I $(CPP_INCLUDE_PATH) $(patsubst build/%.o, src/%.cpp, $@) -o $@ $(LIBS)

.PHONY: build
build: $(cpp_object_files)
	@echo "[CPP Files] :"
	@echo $(cpp_source_files)
	@echo "[CPP OBJ Files] :"
	@echo $(cpp_object_files)
	@g++ $(OPTS) -Wall -o $(OUT_NAME) $(cpp_object_files) $(LIBS)

CLEAN:
	@rm -rfv build/
	@mkdir build
	@echo "CLEAN"