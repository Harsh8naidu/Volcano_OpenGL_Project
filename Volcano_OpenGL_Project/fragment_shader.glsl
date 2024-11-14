#version 330 core

out vec4 fragColour;
in vec3 vertexColour;
in vec3 vertexNormal;
in vec3 lightDirection;

void main()
{
	vec3 lightColour = vec3(1.0, 1.0, 1.0);
	vec3 normalVector = normalize(vertexNormal);
	vec3 lightVector = normalize(lightDirection);
	float dotProduct = max(dot(normalVector, lightVector), 0.0);
	float brightness = max(dotProduct, 0.0);
	vec3 diffuse = brightness * lightColour;
	fragColour = vec4(diffuse, 1.0) * vec4(vertexColour, 1.0);
}