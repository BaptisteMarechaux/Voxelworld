#version 330 core

// Notice that the "1" here equals the "1" in glVertexAttribPointer
in vec4 fragmentColor;
in vec3 Position_worldspace;
in vec3 Normal_cameraspace;
in vec3 EyeDirection_cameraspace;
in vec3 LightDirection_cameraspace;
//uniform vec4 fragmentColor;

out vec4 color;

uniform mat4 MV;
uniform vec3 lightPos;

void main()
{
	vec3 LightColor = vec3(1,1,1);
	float LightPower = 10.0f;

	float distance = length( lightPos - Position_worldspace );

	// Normal of the computed fragment, in camera space
	vec3 n = normalize( Normal_cameraspace );
	// Direction of the light (from the fragment to the light)
	vec3 l = normalize( LightDirection_cameraspace );

	// Eye vector (towards the camera)
	vec3 E = normalize(EyeDirection_cameraspace);
	// Direction in which the triangle reflects the light
	vec3 R = reflect(-l,n);
	// Cosine of the angle between the Eye vector and the Reflect vector,
	// clamped to 0
	//  - Looking into the reflection -> 1
	//  - Looking elsewhere -> < 1
	float cosAlpha = clamp( dot( E,R ), 0,1 );

	float cosTheta = clamp( dot( n,l ), 0,1 );
    //gl_FragColor = vec4(color_out, 1.0);
	color = fragmentColor + LightPower * pow(cosAlpha, 5) / (distance*distance);
}
