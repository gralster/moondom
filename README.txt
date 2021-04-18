MOONDOM is a small walk around game written in C++. 

It runs on libtcod and requires the lib and include folders from libtcod in the same directory to compile. 
all text output goes to the command line!

compile with: g++ src/*.cpp -o moondom -I include -L. -ltcod -Wl,-rpath lib -Wall

run with ./moondom

Sometimes you just get a blank screen! Quit and try running it again. First sign of madness I know..

Controls:
L / R arrows to turn left / right
U / D arrows to move forward / backward
. while standing over an object to pick it up
i to view inventory
d and then type in inventory number to drop an item
l to choose something to look at - move the white x around with your arrow keys and press enter to select
if you look at a living creature you can try to talk to it. They are not very smart, say "bye" to stop talking.
esc to see the menu, nb s does nothing

