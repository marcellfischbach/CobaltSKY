
#include <cobalt/entity/csdefaultparticlestepper.hh>
#include <cobalt/graphics/ivertexbuffer.hh>

csDefaultParticleStepper::csDefaultParticleStepper()
  : iParticleStepper()
  , m_sizeMode(ePSM_Linear)
  , m_gravity(0.0f, 0.0f, 0.0f)
  , m_numTextPages(1)
  , m_textPageTime(1.0f)
{
  CS_CLASS_GEN_CONSTR;
}

csDefaultParticleStepper::~csDefaultParticleStepper()
{

}

void csDefaultParticleStepper::SetSizeMode(csParticleSizeMode sizeMode)
{
  m_sizeMode = sizeMode;
}

csParticleSizeMode csDefaultParticleStepper::GetSizeMode() const
{
  return m_sizeMode;
}

void csDefaultParticleStepper::SetSizeCicleTime(float sizeCicleTime)
{
  m_sizeCicleTime = sizeCicleTime;
}

float csDefaultParticleStepper::GetSizeCicleTime() const
{
  return m_sizeCicleTime;
}

void csDefaultParticleStepper::SetGravity(const csVector3f &gravity)
{
  m_gravity = gravity;
}

const csVector3f &csDefaultParticleStepper::GetGravity() const
{
  return m_gravity;
}

void csDefaultParticleStepper::SetNumberOfTextPages(csUInt8 numTextPages)
{
  m_numTextPages = numTextPages;
}

csUInt8 csDefaultParticleStepper::GetNumberOfTextPages() const
{
  return m_numTextPages;
}

void csDefaultParticleStepper::SetTextPageTime(float textPageTime)
{
  m_textPageTime = textPageTime;
}

float csDefaultParticleStepper::GetTextPageTime() const
{
  return m_textPageTime;
}

void csDefaultParticleStepper::Update(float tpf, csParticle *particle)
{
  csParticle::ParticleData *data;
  if (particle->GetParticleBuffer()->Lock(0, (void **)&data, eBAM_ReadWrite))
  {
    for (unsigned i = 0; i < particle->GetNumberOfParticles(); ++i)
    {
      csParticle::ParticleData *part = &data[i];
      if (part->timeToLive > 0.0f)
      {
        UpdateParticle(tpf, part);
        part->time += tpf;
        part->timeToLive -= tpf;
      }
    }
    particle->GetParticleBuffer()->Unlock();
  }

}

void csDefaultParticleStepper::UpdateParticle(float tpf, csParticle::ParticleData* particle)
{
  csVector3f gr(m_gravity);
  gr *= tpf;

  particle->direction += gr;
  csVector3f::MulAdd(particle->position, particle->direction, tpf, particle->position);

  particle->rotation += particle->rotationSpeed * tpf;


  bool updateSize = true;
  float tf = particle->time / m_sizeCicleTime;
  switch (m_sizeMode)
  {
  case ePSM_Constant:
    updateSize = false;
    break;

  case ePSM_Linear:
    if (tf > 1.0f)
    {
      tf = 1.0f;
    }
    break;

  case ePSM_Saw:
    tf = fmod(tf, 1.0f);
    break;

  case ePSM_Triangle:
    tf = fmod(tf, 1.0f);
    if (tf >= 0.5f)
    {
      tf = (1.0f - tf);
    }
    tf *= 2.0f;
    break;

  case ePSM_Pulse:
    tf *= 3.141569f;
    tf = sin(tf) + 0.5f;
    break;
  }
  if (updateSize)
  {
    particle->size.x = particle->sizeRange.x + particle->sizeRange.y * tf;
    particle->size.y = particle->sizeRange.z + particle->sizeRange.w * tf;
  }

  unsigned page = (unsigned)(particle->time / m_textPageTime);
  page %= m_numTextPages;
  unsigned nextPage = (page + 1) % m_numTextPages;
  float fact = 1.0f - fmod(particle->time, m_textPageTime);

  particle->textPage = csVector3f((float)page, (float)nextPage, fact);

}