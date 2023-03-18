#version 330 core
out vec4 FragColor;

in vec2 outUV;
in vec3 outFragPos;
in vec3 outNormal;

struct Material
{
    sampler2D   m_diffuse;
    sampler2D   m_specular;

    float       m_shiness;
};

uniform Material myMaterial;

struct DirLight
{
    vec3 m_direction;
    vec3 m_ambient;
    vec3 m_diffuse;
    vec3 m_specular;
};

struct PointLight
{
    vec3 m_pos;

    vec3 m_ambient;
    vec3 m_diffuse;
    vec3 m_specular;

    float m_c;
    float m_l;
    float m_q;
};

struct SpotLight
{
    vec3 m_pos;
    vec3 m_direction;
    float m_cutOff;
    float m_outCutOff;

    vec3 m_ambient;
    vec3 m_diffuse;
    vec3 m_specular;

    float m_c;
    float m_l;
    float m_q;
};
#define MAX_POINT_NUMBER 4
uniform DirLight         _dirLight;
uniform PointLight       _pointLight[MAX_POINT_NUMBER];
uniform SpotLight        _spotLight;

uniform vec3 view_pos;

vec3 calculateDir(DirLight _light , vec3 _normal , vec3 _viewDir)
{
    vec3 _lightDir =  normalize(_light.m_direction);
//环境光
    vec3 _ambient = _light.m_ambient * vec3(texture(myMaterial.m_diffuse , outUV));

//漫反射
    float _diff = max(dot(_normal , -_lightDir) , 0.0f);
    vec3  _diffuse = _light.m_diffuse * _diff *  vec3(texture(myMaterial.m_diffuse , outUV));

//镜面反射
    vec3 _reflectDir = reflect(_lightDir , _normal);
    float _spec = pow(max(dot(_reflectDir , _viewDir) , 0.0f) , myMaterial.m_shiness);
    vec3  _specular = _light.m_specular * _spec * vec3(texture(myMaterial.m_specular , outUV));

    return (_ambient + _diffuse + _specular);
}

vec3 calculatePoint(PointLight _light , vec3 _normal , vec3 _viewDir , vec3 _fragPos)
{
    vec3 _lightDir =  normalize( _fragPos - _light.m_pos);
//环境光
    vec3 _ambient = _light.m_ambient * vec3(texture(myMaterial.m_diffuse , outUV));
//漫反射
    float _diff = max(dot(_normal , -_lightDir) , 0.0f);
    vec3  _diffuse = _light.m_diffuse * _diff *  vec3(texture(myMaterial.m_diffuse , outUV));
//镜面反射
    vec3 _reflectDir = reflect(_lightDir , _normal);
    float _spec = pow(max(dot(_reflectDir , _viewDir) , 0.0f) , myMaterial.m_shiness);
    vec3  _specular = _light.m_specular * _spec * vec3(texture(myMaterial.m_specular , outUV));

//衰减系数计算
    float _dist = length(_light.m_pos - _fragPos);
    float _attenuation = 1.0f / (_light.m_c + _light.m_l * _dist + _light.m_q * _dist * _dist);


    return (_ambient + _diffuse + _specular) * _attenuation;
}

vec3 calculateSpot(SpotLight _light , vec3 _normal , vec3 _viewDir , vec3 _fragPos)
{
    vec3 _lightDir =  normalize( _fragPos - _light.m_pos);
    vec3 _spotDir= normalize(_light.m_direction);
    float _cosTheta = dot(-_lightDir , -_spotDir);
    float _epsilon = _light.m_cutOff - _light.m_outCutOff;
    float _intensity = clamp((_cosTheta - _light.m_outCutOff) / _epsilon , 0.0f , 1.0f);

//环境光
    vec3 _ambient = _light.m_ambient * vec3(texture(myMaterial.m_diffuse , outUV));
//漫反射
    float _diff = max(dot(_normal , -_lightDir) , 0.0f);
    vec3  _diffuse = _light.m_diffuse * _diff *  vec3(texture(myMaterial.m_diffuse , outUV));
//镜面反射
    vec3 _reflectDir = reflect(_lightDir , _normal);
    float _spec = pow(max(dot(_reflectDir , _viewDir) , 0.0f) , myMaterial.m_shiness);
    vec3  _specular = _light.m_specular * _spec * vec3(texture(myMaterial.m_specular , outUV));

//衰减系数计算
    float _dist = length(_light.m_pos - _fragPos);
    float _attenuation = 1.0f / (_light.m_c + _light.m_l * _dist + _light.m_q * _dist * _dist);

    return (_ambient + _diffuse + _specular) * _attenuation * _intensity;
}

void main()
{
    vec3 _normal = normalize(outNormal);
    vec3 _viewDir = normalize(view_pos - outFragPos);

    vec3 _result = calculateDir(_dirLight ,_normal , _viewDir);
    
    for(int i = 0;i < MAX_POINT_NUMBER;i++)
    {
        _result += calculatePoint(_pointLight[i] , _normal , _viewDir , outFragPos);
    }

    _result += calculateSpot(_spotLight ,  _normal , _viewDir , outFragPos); 

    FragColor = vec4(_result ,1.0f);
};