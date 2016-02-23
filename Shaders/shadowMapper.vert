#version 330 core

// Input vertex data, different for all executions of this shader.
layout(location = 0) in vec3 vertexPosition_modelspace;

uniform mat4 depthMVP;
uniform mat4 Modelm;
void main(){
	gl_Position = depthMVP*vec4(vertexPosition_modelspace,1);
}

