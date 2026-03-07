#version 330

in vec3 vertexPosition;
in vec2 vertexTexCoord;
in vec3 vertexNormal;

uniform mat4 mvp;      // Combined Model-View-Projection
uniform mat4 matModel; // Model transform (World space)

out vec3 fragPosition;
out vec2 fragTexCoord;
out vec3 fragNormal;

void main() {
    // Calculate world position for lighting
    fragPosition = vec3(matModel * vec4(vertexPosition, 1.0));
    
    // Normal in world space (ignoring scale for simplicity)
    fragNormal = normalize(vec3(matModel * vec4(vertexNormal, 0.0)));
    
    fragTexCoord = vertexTexCoord;

    // This line is what actually puts the model in 3D space on your screen
    gl_Position = mvp * vec4(vertexPosition, 1.0);
}