#version 130

uniform float angle;

in vec3 position;
in vec2 intexCoords;
out vec4 color;
out vec2 texCoord;

void main() {

	float zang = 0.5235987755982988;
	
	mat4 rz = mat4(1.0, 0.0, 0.0, 0.0,
				   0.0, cos(zang), -sin(zang), 0.0,
				   0.0, sin(zang),  cos(zang), 0.0,
				   0.0, 0.0, 0.0, 1.0);

	mat4 ry = mat4(  cos(angle),  0.0, -sin(angle), 0.0,
					 0.0,         1.0,  0.0,        0.0,
					 sin(angle),  0.0,  cos(angle), 0.0,
					 0.0,         0.0,        0.0, 1.0);

	gl_Position = rz * ry * vec4(position, 1.0);
//`	color = vec4(position, 1.0);

	color = vec4(1.0,1.0,1.0,1.0);
	texCoord = intexCoords;
}
