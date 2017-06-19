
#version 330

in vec3 cs_Position;
in vec2 cs_ParticleSize;
in float cs_ParticleRotation;
in float cs_ParticleTimeToLive;
in vec3 cs_ParticleTextPage;


out vec2 inGeomParticleSize;
out float inGeomParticleRotation;
out float inGeomParticleTimeToLive;
out vec3 inGeomParticleTextPage;

void main ()
{
  // we don't use model matrix here
	gl_Position = vec4(cs_Position, 1.0);
	inGeomParticleSize = cs_ParticleSize;
	inGeomParticleRotation = cs_ParticleRotation;
	inGeomParticleTimeToLive = cs_ParticleTimeToLive;
	inGeomParticleTextPage = cs_ParticleTextPage;
}

