#version 330 core

layout(location = 0) in vec3 vertexPosition_modelspace;
//layout(location = 1) in vec3 vertexColor;
uniform vec4 vertexColor;

out vec4 fragmentColor;

uniform mat4 MVP;
uniform vec3 normal;

void main()
{
    gl_Position = MVP * vec4(vertexPosition_modelspace, 1);
	fragmentColor = vertexColor;
	
	fragmentColor = vec4(gl_Position);
}