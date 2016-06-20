
#version 330

in vec4 vk_Position;
in vec4 vk_ParticleSize;
in vec3 vk_ParticleRotation;
in vec4 vk_ParticleTexCoord;


out vec4 inGeomParticleSize;
out vec3 inGeomParticleRotation;
out vec4 inGeomParticleTexCoord;

void main ()
{
  // we don't use model matrix here
	gl_Position = vk_Position;
	inGeomParticleSize = vk_ParticleSize;
	inGeomParticleRotation = vk_ParticleRotation;
	inGeomParticleTexCoord = vk_ParticleTexCoord;
}

