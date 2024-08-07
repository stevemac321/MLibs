This works on Linux, FreeBSD too, but I haven't tried in a while.  
1. Extract STLibs.tar in place.  Here is the directory structure under MLibs:
├── cinc
├── lib
│   └── asm
├── STLibs
│   
└── tests
    ├── array-test
    │   └── Debug
    ├── asmtest
    │   └── Debug
    └── c-algo
        └── Debug
2. I use bfy.py takes up to three args:  ./bfy tests clean build flashrun.  I like doing them each separately,
   ./bfy tests clean, then ./bfy tests build, then ./bfy tests flashrun.  The build does the objcopy to get the .bin file.
   Flash run uses st-flash (sudo apt stlink-tools) to erase, write, and reset the bin file.
3. ./bfy flashrun also spawns openocd in a separate process, the gdb with a batch script that runs the tests in automated
   fashion all in one shell.  Semihosting is enabled, you get lots of output.
4. Edit the bfy.py to use whatever arm toolchain you want.  I build with gcc-arm-none-eabi (I have version 13.3 at this time),
   I get better results with openocd with gdb-multiarch that you install with aptitude.
 5.  See my cortex-m-quickstart which is a fork from the Embedded Rust Book folks, all I do is provide extra steps I did to get
    Rust to work with my board.
