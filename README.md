WIP. 
This is a very basic game engine being built with C++ and OpenGL.


To run on linux or WSL:
1.Install glfw3, OpenGL and glm system wide (will clarify on this later)
2. cd into build folder  
3. cmake ..  
4. make && ./engine   

### Visual Studio
If you are using Visual Studio with the CMake integration, make sure to set the working directory to the `build/` folder (not the project root).  
By default, Visual Studio may try to run from the project root, which will cause issues finding compiled binaries.  


Some screenshots
Absolute Basics:
<img width="1001" height="805" alt="Screenshot from 2025-08-06 21-23-12" src="https://github.com/user-attachments/assets/19cde3bb-4c7b-4be8-9c18-2ba575f9231f" />

Diffuse + Ambient lighting Implemented:
<img width="1410" height="922" alt="image" src="https://github.com/user-attachments/assets/66086f14-1a21-462e-b652-d452d5a3177c" />

Phong lighting implemented + support for diffuse and specular maps.
Also added support for global directional illumination.

<img width="1230" height="842" alt="Phong-and-Maps" src="https://github.com/user-attachments/assets/f56e9987-cbcc-4017-8995-16bb614136fe" />


Spot Light:

<img width="1418" height="886" alt="image" src="https://github.com/user-attachments/assets/2c555d40-9345-45b4-addf-ce76f29f2433" />


