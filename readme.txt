Requirments- GLEWs, GLM, GLFW

Files included:Main.cpp, ShaderProgram.h,TextureMesh.cpp, TextureMesh.h,datastruct.h, camera.h, example 1-3.png

Datastruct.h: as specified in the assignment for VertexData and Tridata class. Used in texturemesh

Camera.h: have the function to move the camera around as specified (forward/backward/rotate left and right)
 and return a view matrix for MVP.

ShaderProgram.h: Class for a program that contains vertex/frag shaders. Also contain helper function to modify uniform variable
within the shader's code. Create the shader, compile it and linking a program to both shader and the class itself holds an instance
of ID that can be reference in texture mesh and main. 

TextureMesh.cpp/h: contain the readPLYfile, loadBMP, constructor and draw. The constructor takes both ply file path and bmp file path,
load the corresponding vertices, texture, faces into VAO/VBO, define the vertexattribarray for position and texture, and generate/bind 
texture. the draw function uses the shader program created and set the corresponding texture and MVP to draw the elements.

Main.cpp: get a shader program instance and load all of the texture mesh object into two list for opaque and transparent object.
In the loop, keep track of time so camera can update view matrix and use the view matrix for MVP. Then for each texture mesh object,
call it's draw function to render.

Known bug: the hay and the pitchfork beside the door looks off