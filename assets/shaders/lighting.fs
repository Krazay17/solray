#version 330

in vec3 fragPosition;
in vec2 fragTexCoord;
in vec3 fragNormal;

out vec4 finalColor;

uniform sampler2D texture0;
uniform vec4 colDiffuse;
uniform vec3 lightPos;   // This is actually the DIRECTION for directional lights
uniform vec4 lightColor;
uniform vec3 viewPos;

void main()
{
    vec4 texelColor = texture(texture0, fragTexCoord);
    vec3 normal = normalize(fragNormal);
    
    // FIX: For Directional Light, we use the lightPos directly as the direction
    // We normalize it to be safe.
    vec3 lightDir = normalize(lightPos);
    
    // Standard Diffuse (Lambertian)
    float diff = max(dot(normal, lightDir), 0.0);
    vec3 diffuse = diff * lightColor.rgb;
    
    // Ambient: Keep this low so shadows aren't pitch black
    vec3 ambient = vec3(1); 

    // Combine
    vec3 lighting = ambient + diffuse;
    
    // Apply to texture and the tint color (make sure to pass WHITE in C)
    finalColor = vec4(lighting, 1.0) * texelColor * colDiffuse;
}