_fmt:=\x1b[
_fmt_clear:=\x1b[0m
_fmt_note:=\x1b[1;37m
_fmt_warning:=\x1b[1;33m
_fmt_passed:=\x1b[1;32m
_fmt_failed:=\x1b[1;31m


help:
	@ echo "$(_fmt_warning)--- make options ---"
	@ echo "${_fmt_note}make help                  : - list of possible commands"
	@ echo             "make clean                 : - removes build directory"
	@ echo             "make clean-shaders         : - removes compiled shaders"
	@ echo             "make setup-project-debug   : - generates the project build directory"
	@ echo             "make setup-project-release : - generates the project build directory"
	@ echo             "make compile-deps          : - compile required dependencies"
	@ echo             "make compile-shaders       : - compiles shaders"
	@ echo             "make compile-project       : - compile project from build directory"
	@ echo             "make install-project       : - installs project to internal"
	@ echo             "make run-project           : - runs project from ./internal/install$(_fmt_clear)"

clean:
	@ echo "${_fmt_warning}# removing build contents$(_fmt_clear)"
	@ rm -rf ./internal/build

clean-shaders:
	@ echo "${_fmt_warning}# removing compiled shaders$(_fmt_clear)"
	@ rm -rf ./assets/shaders/compiled/*

setup-project-debug:
	@ echo "${_fmt_warning}# setting up project$(_fmt_clear)"
	-@ rm -rf ./internal/build
	@ cmake -S ./internal -B ./internal/build -DCMAKE_BUILD_TYPE=Debug

setup-project-release:
	@ echo "${_fmt_warning}# setting up project$(_fmt_clear)"
	-@ rm -rf ./internal/build
	@ cmake -S ./internal -B ./internal/build -DCMAKE_BUILD_TYPE=Release

compile-deps:
	@ echo "$(_fmt_warning)# compiling dependencies$(_fmt_clear)"
	@ cd ./internal; sh ./bash/compile_deps.sh

compile-shaders:
	@ echo "${_fmt_warning}# compiling shaders$(_fmt_clear)"
	@ sh ./internal/bash/compile_sdl3_shaders.sh

compile-project:
	@ echo "${_fmt_warning}# compiling project$(_fmt_clear)"
	@ cmake --build ./internal/build

install-project:
	@ echo "$(_fmt_warning)# installing project$(_fmt_clear)"
	-@ rm -rf ./internal/install
	@ cmake --install ./internal/build --prefix ./internal/install

run-project:
	@ echo "$(_fmt_warning)# running project"
	@ echo                "-----=-=-=-=-=-=-=-----$(_fmt_clear)"
	@ ./internal/install/bin/app
	@ echo "$(_fmt_warning)-----=-=-=-=-=-=-=-----$(_fmt_clear)"
