#version 130

in pass

void main() {
	gl_FragColor = pass.z;

}
