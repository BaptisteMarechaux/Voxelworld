#version 330 core

// Notice that the "1" here equals the "1" in glVertexAttribPointer
in vec3 fragmentColor;

out vec3 color;

void main()
{
    //gl_FragColor = vec4(color_out, 1.0);
	color = fragmentColor;
}
