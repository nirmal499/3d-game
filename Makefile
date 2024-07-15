project-configure-fresh:
	rm -fr build
	mkdir build
	cmake -B build -S .

project-configure:
	cmake -B build -S .

project-build:
	cmake --build build

project-run-client:
	./build/main

project-run-server:
	./build/server

project-run-debugger-for-client:
	gdb ./build/main

project-run-debugger-for-server:
	gdb ./build/server