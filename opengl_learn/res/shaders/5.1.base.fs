#version 330 core
out vec4 FragColor;

in vec2 TexCoords;

uniform sampler2D texture1;

// void main()
// {    
//     FragColor = texture(texture1, TexCoords);
//    // FragColor = vec4(0.2, 0.3, 0.8, 1.0);
// }

void main()
{
    vec3 col = texture(texture1, TexCoords).rgb;
    FragColor = vec4(col, 1.0);
} 