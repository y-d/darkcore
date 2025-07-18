Simply just a bad gdi virus (drops out some effects and byte beats)

but this also has an mbr payload which will just flash a red and black screen

if you wanna build it:

Build Unsafety:

    g++ -g main.cpp -lwinmm -lgdi32 -mwindows -static
    
Build Safety:

    g++ -g main-safety.cpp -lwinmm -lgdi32 -mwindows -static


im not sure if u can recover from this

but there has to be a way
