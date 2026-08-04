#include "boilerPlate.h"
#include <string>
#include "shaderFile.h"

using std::string;

unsigned int shaderProgramLoader() {
    string vertexShader = R"(#version 330 core 

layout(location = 0) in vec3 aPos;
layout(location = 1) in vec2 aLoc;
layout(location = 2) in vec3 pPos;
layout(location = 3) in float rad;
layout(location = 4) in vec3 pVel;

uniform mat4 projection;
uniform mat4 view;
uniform float is_box;
uniform vec3 lightPos;

out vec2 locPos;
out vec3 changed_coordinate;
flat out float nVel;
flat out float box;
flat out float radius;
flat out vec3 srcPos;
flat out vec3 particlePos;

void main() {
    vec3 finalPosition;

    mat4 mat = mat4(
        vec4(2.0f * rad, 0.0f,       0.0f,       0.0f),
        vec4(0.0f,       2.0f * rad, 0.0f,       0.0f),
        vec4(0.0f,       0.0f,       2.0f * rad, 0.0f),
        vec4(0.0f,       0.0f,       0.0f,       1.0f)
    );

    mat4 translate = mat4(
        vec4(1.0f,   0.0f,   0.0f,   0.0f),
        vec4(0.0f,   1.0f,   0.0f,   0.0f),
        vec4(0.0f,   0.0f,   1.0f,   0.0f),
        vec4(pPos.x, pPos.y, pPos.z, 1.0f)
    );

    mat4 pv = projection * view;

    if (is_box == 1.0f) {
        gl_Position = pv * vec4(aPos, 1.0f);
    } else {
        finalPosition = vec3(view * vec4(pPos, 1.0f));
        gl_Position = projection * vec4(finalPosition + vec3(mat * vec4(aPos, 1.0f)), 1.0f);
    }

    locPos = aLoc;
    box = is_box;
    nVel = length(pVel) / 1.73f;
    srcPos = lightPos;
    changed_coordinate = finalPosition + vec3(mat * vec4(aPos, 1.0f));
    radius = rad; 
    particlePos = vec3((view * vec4(pPos, 1.0f)).xyz);
}
)";

    string fragmentShader = R"(#version 330 core 

in vec2 locPos;
in vec3 changed_coordinate;
flat in float nVel;
flat in float box;
flat in float radius;
flat in vec3 particlePos;
flat in vec3 srcPos;

out vec4 fragColor;

void main() {
    vec3 actual_color;
    vec3 lightIntensity = vec3(0.5f, 0.5f, 0.5f);
    
    if (box == 0.0f) {
        float x = clamp(nVel, 0.0f, 1.0f); 

        float r = clamp(1.0f - (10.0f * x * 1.5f), 0.0f, 1.0f);
        float g = clamp(1.0f - 4.0f * (10.0f * x - 0.5f) * (10.0f * x - 0.5f), 0.0f, 1.0f);
        if (x > 0.5f) {
            g = 1.0f; 
        }

        float b = clamp((10.0f * x - 0.2f) * 1.25f, 0.0f, 1.0f);

        actual_color = vec3(r, g, b);

        float distance = locPos.x * locPos.x + locPos.y * locPos.y;
        if (distance > 0.25f) {
            discard;
        }

        vec3 ambient = lightIntensity * actual_color;

        float z = sqrt(
            radius * radius - 
            (changed_coordinate.x - particlePos.x) * (changed_coordinate.x - particlePos.x) - 
            (changed_coordinate.y - particlePos.y) * (changed_coordinate.y - particlePos.y)
        ) + particlePos.z;
       
        vec3 surface_coordinate = vec3(changed_coordinate.x, changed_coordinate.y, z);

        // Lighting calculation (Phong reflection model)
        vec3 normal = normalize(surface_coordinate - particlePos);
        float ratio1 = max(dot(normal, -normalize(surface_coordinate - srcPos)), 0.0f);

        vec3 diffuse = lightIntensity * ratio1 * actual_color;

        vec3 reflected = reflect(normalize(surface_coordinate - srcPos), normal);
        float ratio2 = max(dot(normalize(reflected), -normalize(surface_coordinate)), 0.0f);

        vec3 specular = lightIntensity * pow(ratio2, 20.0f) * actual_color;

        fragColor = vec4(ambient + diffuse + specular, 1.0f);
    } else {
        fragColor = vec4(0.0f, 1.0f, 1.0f, 1.0f);
    }
}
)";

    const char* vcode = vertexShader.c_str();
    const char* fcode = fragmentShader.c_str();
    const char* shaderCode[] = { vcode, fcode };

    unsigned int shaderProgram = glCompileShaders(2, shaderCode);

    return shaderProgram;
}