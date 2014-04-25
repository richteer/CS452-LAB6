#version 130

in vec4 color;
in vec2 texCoord;
uniform sampler2D texture;
in shadow;

void main(){

	gl_FragColor = shadow texture * color of the object
}
