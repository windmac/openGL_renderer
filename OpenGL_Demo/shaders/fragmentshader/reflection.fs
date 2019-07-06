#version 330 core
out vec4 FragColor;

in vec3 Normal;
in vec3 Position;

uniform vec3 cameraPos;
uniform samplerCube skybox;

void main()
{             
	float ratio = 1.00 / 1.2;
	//Normal = vec3(1-Normal.x,1-Normal.y,1-Normal.z);
    vec3 I = normalize(Position - cameraPos);
    vec3 Re = reflect(I, normalize(Normal));
	vec3 Ra = refract(I, normalize(Normal), ratio);
	//  FragColor = vec4(texture(skybox, Ra).rgb, 0.0);
    FragColor = vec4((texture(skybox, Re).rgb/2.8)+(texture(skybox, Ra).rgb/1.2), 1.0);
   //FragColor = vec4(texture(skybox, Ra).rgb, 1.0);
}