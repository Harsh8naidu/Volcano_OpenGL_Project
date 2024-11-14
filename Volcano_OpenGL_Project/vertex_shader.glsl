#version 330 core

in vec3 position;
in vec3 colour;
in vec3 normal;

out vec3 vertexColour;
out vec3 vertexNormal;
out vec3 lightDirection;

uniform mat4 modelMat;
uniform mat4 viewMat;
uniform mat4 projMat;

void main(){
	vec3 lightPos = vec3(0.0, 1.0, 0.0);
    vec4 worldPos = modelMat * vec4(position, 1.0);
	gl_Position = projMat * viewMat * worldPos;
	vertexColour = colour;
	vertexNormal = (modelMat * vec4(normal, 0.0)).xyz; // vec3 to vec4 with .xyz
	lightDirection = lightPos - worldPos.xyz;
}

