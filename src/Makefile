CLANGFILES = */*.cc */*.h

.PHONY: tests build

all: install tests

build:
	cmake -S ./ -B build -DCMAKE_BUILD_TYPE=Debug -DCMAKE_CXX_FLAGS_DEBUG="-g -fprofile-arcs -ftest-coverage -std=c++17"
	cmake --build build

rebuild: uninstall clean build

install: build

uninstall:
	rm -rf build/*

tests:
	./build/tests/tests --gtest_repeat=1

run:
ifeq ($(shell uname -s), Linux)
	build/view/view
else
	open build/view/view.app
endif

clang-check:
	clang-format -n --style=Google -verbose $(CLANGFILES)

clang-format:
	clang-format -i --style=Google -verbose $(CLANGFILES)

leaks:
ifeq ($(shell uname -s), Linux)
	valgrind --tool=memcheck --leak-check=yes --track-origins=yes --quiet ./build/tests/tests
else
	CK_FORK=no leaks --atExit -- ./build/tests/tests
endif

clean:
	rm -rf *user gcov* report *.tar.gz guide/html guide/latex

gcov_report: tests
	mkdir -p report
	gcovr -r . --html-details -o ./report/gcov_report.html -f model/ -f controller/
	open ./report/gcov_report.html
