#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;

out vec3 outNormal;
out vec3 outPos;

uniform mat4 _modelMatrix;
uniform mat4 _viewMatrix;
uniform mat4 _projMatrix;

void main()
{
   gl_Position = _projMatrix * _viewMatrix * _modelMatrix * vec4(aPos.x, aPos.y, aPos.z, 1.0);
   outNormal = mat3(transpose(inverse(_modelMatrix))) * aNormal;//法线位移变换矩阵
   outPos = vec3(_modelMatrix * vec4(aPos.x, aPos.y, aPos.z, 1.0));
};