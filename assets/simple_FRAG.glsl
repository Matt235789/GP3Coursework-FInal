#version 400

uniform vec3 objectColor;
uniform sampler2D diffuse;

in vec2 texCoords0;


void main()
{
	 
	    gl_FragColor = texture2D(diffuse, texCoords0);
	 
    
}
