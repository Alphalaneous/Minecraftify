#version 300 es

precision highp float;

uniform vec2 center; 
uniform vec2 resolution;
uniform float time;
uniform vec2 mouse; 

uniform samplerCube skybox;

out vec4 FragColor;

void main(){

    float rotX = (time / resolution.x) * -20.0 * 3.14;
    float rotY = (resolution.y / 2.0 / resolution.y) * 3.14;
    
    vec2 uv = 4.0 * (gl_FragCoord.xy - 0.5 * resolution.xy) / resolution.xx;
    
    vec3 camO = vec3(cos(rotX), cos(rotY), sin(rotX));
    
    vec3 camD = normalize(vec3(0)-camO);
    
    vec3 camR = normalize(cross(camD, vec3(0, 1, 0)));
    
    vec3 camU = cross(camR,camD);
    
   	vec3 dir =  normalize(-uv.x * camR + uv.y * camU + camD);

    FragColor = texture(skybox, dir);

}