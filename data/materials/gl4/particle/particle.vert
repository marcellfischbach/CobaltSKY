
#version 330

in vec3 vk_Position;
in vec2 vk_ParticleSize;
in float vk_ParticleRotation;


out vec2 inGeomParticleSize;
out float inGeomParticleRotation;

void main ()
{
  // we don't use model matrix here
	gl_Position = vec4(vk_Position, 1.0);
	inGeomParticleSize = vk_ParticleSize;
	inGeomParticleRotation = vk_ParticleRotation;
}

