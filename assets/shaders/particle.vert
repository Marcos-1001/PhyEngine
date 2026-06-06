#version 330 core
layout (location = 0) in vec3 aPos;   // the position variable has attribute position 0
layout (location = 1) in vec3 aColor; // the color variable has attribute position 1

uniform float uRadius; // uniform for particle radius
uniform mat4 uViewProj; // uniform for view matrix

out vec3 ourColor; // output a color to the fragment shader

void main()
{
    gl_Position = uViewProj * vec4(aPos, 1.0);
    gl_PointSize = uRadius; // set the size of the point based on the uniform radius
    ourColor = aColor; // set ourColor to the input color we got from the vertex data
}       