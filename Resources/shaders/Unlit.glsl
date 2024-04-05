#shader vertex
#version 420 core

#include "shaders/Core.verth"

out vec2 TexCoords;

void main()
{
	mat4 mvp = pth_viewProjection * pth_modelMat;
	gl_Position = mvp * vec4(_pos, 1.0);
	TexCoords = _texCoords;
}

#shader fragment
#version 330 core

in vec2 TexCoords;

out vec4 FragColor;

uniform vec4      u_tint;
uniform vec2      u_uvOffset;
uniform vec2      u_uvScale;

uniform sampler2D u_diffuse;

void main()
{
	vec2 texCoords = TexCoords * u_uvScale + u_uvOffset;
	vec4 texColor = texture(u_diffuse, texCoords);

	FragColor = texColor * u_tint;
}