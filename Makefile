SOURCES = src/main.cpp src/recipe.cpp src/download.cpp src/archive.cpp src/package.cpp src/build.cpp

lath: $(SOURCES)
	g++ -std=c++23 -Wall -Wextra -Wpedantic -g $(SOURCES) -llua5.3 -lcurl -larchive -o lath
