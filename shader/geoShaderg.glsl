#version 330 core

layout(triangles) in;//输入三角形
layout(line_strip , max_vertices = 6) out;//输出线段，增加一倍点，

in vec3 _normal[];

uniform mat4 _projMatrix;

void generateLine(int _index)
{
	float _mag = 0.4;//
	gl_Position = _projMatrix * gl_in[_index].gl_Position;
	EmitVertex();

	gl_Position = _projMatrix * (gl_in[_index].gl_Position + vec4(_mag * _normal[_index] , 1.0));//该点向法向量方向移动
	EmitVertex();

	EndPrimitive();
}

void main()
{
	generateLine(0);
	generateLine(1);
	generateLine(2);
}