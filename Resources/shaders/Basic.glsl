#shader vertex
#version 330 core

layout(location = 0) in vec4 _pos;

void main()
{
	gl_Position = _pos;
}

#shader fragment
#version 330 core
out vec4 FragColor;

void main()
{
	FragColor = vec4(1.0);
}
