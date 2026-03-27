# Square++  

A simple game built using raylib in c++.

![Here is a demo:](https://github.com/user-attachments/assets/341cdf35-a34c-4a29-af45-6e765d969f6b)

To play download the release for the appropriate os.  

# To build the project

*YOU NEED TO HAVE CMAKE AND MAKE INSTALLED AND ALSO ZIG INSTALLED IF CROSS-COMPILING FOR DIFFERENT PLATFORMS.*  

## For the current OS  

1. Clone the repo.  
2. Go into the project directory.
3. Build it using cmake.  

```bash
cmake -B build  
cmake --build build
```
  
## If building for a different platform

Build it using cmake by defining the TARGET variable.  

### For Windows

```bash
cmake -B build -D TARGET=x86_64_windows-gnu
cmake --build build
```

### For Linux

```bash
cmake -B build -D TARGET=x86_64-linux-musl  
cmake --build build  
```

Arrow keys or vim keys(H J K L) can be used to play.  
Press spacebar to pause.  
Both arrow keys can be used at the same time.  
