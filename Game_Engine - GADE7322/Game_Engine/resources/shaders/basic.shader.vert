#version 330 core
layout (location = 0) in vec3 aPos; // this is related to vertex input
layout (location = 1) in vec3 vCol; // vertex colour position, also the location needs be the same as 
layout (location = 2) in vec2 aTexCoord; //location for texture co ordinate, we not trying to move our vertices but instead colour them

uniform vec3 colourIn; // this is our colour, uniform affects the entire mesh
uniform mat4 transform;
uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

out vec3 outColour; // this is our communication to the frag shader
out vec2 texCoord;

void main()
{
    //gl_Position = transform * vec4(aPos, 1.0);
    gl_Position = projection * view * model * vec4(aPos, 1.0);

    //outColour = colourIn; //to make sure outColor is what is being passed in by the uniform colourIn
    //outColour = vCol; // instaed of uniform colour take vertex colour
    texCoord = vec2(aTexCoord);
    //vert shader always loads first, communication only happens in this shader we cannot communicate with
    // frag shader, if you want to pass a value to the frag it has to be done here
    // take colourIn and pass it to the frag shader using outColour
};
    