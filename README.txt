MOONDOM is a small walk around game written in C++. 

It runs on libtcod and requires the lib and include folders from libtcod in the same directory to compile. 

compile with: g++ src/*.cpp -o moondom -I include -L. -ltcod -Wl,-rpath lib -Wall
