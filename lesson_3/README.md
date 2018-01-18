# Using QTCreator
- Open QTCreator
- Press "Open Project" and navigate to project directory
- Select CMakeLists.txt
- If you are asked to configure kits, make sure desktop is checked and press continue
- Build -> Run (Only this needed after editing)

# Using CLI
```
cd path/to/project
mkdir build
cd build
cmake ..
# Run from here after editing
make
./tut
```

# About the code
This code has been adopted from here
http://www.opengl-tutorial.org/beginners-tutorials/tutorial-2-the-first-triangle/

If you want to self-learn, I recommend going through the site and downloading their original code
https://github.com/opengl-tutorials/ogl/archive/master.zip

The build process is specified here http://www.opengl-tutorial.org/beginners-tutorials/tutorial-1-opening-a-window/

But it takes time to build and you might encounter errors since they build the libraries
