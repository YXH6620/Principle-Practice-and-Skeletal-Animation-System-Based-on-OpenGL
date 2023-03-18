#version 330 core
out vec4 FragColor;

in vec3 outNormal;
in vec3 outPos;

uniform vec3 _viewPos;//观察点
uniform samplerCube  ourTexture;

void main()
{
    float ratio = 1.00 / 1.52;//水折射向玻璃
    vec3 I = normalize(outPos - _viewPos);//入射
    vec3 R = refract(I , normalize(outNormal) , ratio);//折射
    FragColor = texture(ourTexture , R);
};