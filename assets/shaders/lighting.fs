#version 330

in vec3 fragPosition;
in vec2 fragTexCoord;
in vec3 fragNormal;

out vec4 finalColor;

uniform sampler2D texture0;
uniform vec4 colDiffuse;
uniform vec3 viewPos;// We need this for both Rim and Fog
uniform vec3 rimColor;

void main()
{
    vec4 texelColor=texture(texture0,fragTexCoord);
    vec3 normal=normalize(fragNormal);
    vec3 viewDir=normalize(viewPos-fragPosition);
    
    // --- 1. Top-Down Lighting ---
    float diffuse=max(dot(normal,vec3(0.,1.,0.)),0.);
    
    // --- 2. Rim Lighting ---
    // High intensity when the normal is perpendicular to the view direction
    float rimThreshold=.5;// Adjust this to make rim wider/thinner
    float viewDot=abs(dot(viewDir,normal));
    float rim=pow(1.-viewDot,3.);
    vec3 rimColor=vec3(0.4, 0.0, 0.0)*rim;
    
    // --- 3. Fog Calculation ---
    float dist=length(viewPos-fragPosition);
    float fogStart=20.;
    float fogEnd=100.;
    // Calculate fog factor (0.0 = full fog, 1.0 = no fog)
    float fogFactor=clamp((fogEnd-dist)/(fogEnd-fogStart),0.,1.);
    vec3 fogColor=vec3(.31372,.31372,.31372);// Match this to your ClearBackground color
    
    // --- Combine Everything ---
    vec3 lighting=vec3(diffuse+.2);// Diffuse + Ambient
    vec3 colorWithRim=(texelColor.rgb*colDiffuse.rgb*lighting)+rimColor;
    
    // Final linear interpolation between model color and fog color
    finalColor=vec4(mix(fogColor,colorWithRim,fogFactor),1.);
}