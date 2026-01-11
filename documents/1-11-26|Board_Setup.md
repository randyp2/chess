# 1/11/2026 - Board Setup
**Start Time: 1/11/26 **
**Endt Time: Present **

## Goals 
- Setup file directory 
- Learn SMFL Graphics
- Learn chess board bit mapping

### Setup 
- **src/**: Contains definitions for the includes
- **include/**: Contains the prototypes for the src code

Often good to seperate header files from cpp files in different directories for the following:
    - Readability
    - Clear interfaces
    - **Improves build times**
        - C++ compiles **translation units** (.cpp file after the preprocessor expands #includes and all macros)
        - By seperating .hpp files and .cpp files it doesn't trigger a recursive recompile 
            - i.e. changing one line in a .cpp file recompiles that single translation unit



### New Technologies
**CMAKE:** Build system generator that generates configuration files
    - **DOES NOT** compile code it orchestrates compilation
    - Writes the makefile for us 

**cmake -S . -B build**
    - Configure my project, read the source here, and generate build/ with build files

**cmake --build build**
    - Use generated build system (Makefile) under build/ to compile my project
    - When modifying files rerun this command

**cmake --build build --target clean**
    - Remove build artifacts under build/
    - Only do so if:
        - Config is broken
        - Compilter or flags changed


