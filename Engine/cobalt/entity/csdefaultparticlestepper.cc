
#include <cobalt/entity/csdefaultparticlestepper.hh>
#include <cobalt/graphics/ivertexbuffer.hh>
#include <cobalt/math/csmath.hh>

cs::DefaultParticleStepper::DefaultParticleStepper()
  : cs::iParticleStepper()
  , m_sizeMode(cs::ePSM_Linear)
  , m_sizeCicleTime(0.0f)
  , m_numTextPages(1)
  , m_textPageTime(1.0f)
  , m_gravity(0.0f, 0.0f, 0.0f)
{
  CS_CLASS_GEN_CONSTR;
}

cs::DefaultParticleStepper::~DefaultParticleStepper()
{

}

void cs::DefaultParticleStepper::SetSizeMode(cs::eParticleSizeMode sizeMode)
{
  m_sizeMode = sizeMode;
}

cs::eParticleSizeMode cs::DefaultParticleStepper::GetSizeMode() const
{
  return m_sizeMode;
}

void cs::DefaultParticleStepper::SetSizeCicleTime(float sizeCicleTime)
{
  m_sizeCicleTime = sizeCicleTime;
}

float cs::DefaultParticleStepper::GetSizeCicleTime() const
{
  return m_sizeCicleTime;
}

void cs::DefaultParticleStepper::SetGravity(const cs::Vector3f &gravity)
{
  m_gravity = gravity;
}

const cs::Vector3f &cs::DefaultParticleStepper::GetGravity() const
{
  return m_gravity;
}

void cs::DefaultParticleStepper::SetNumberOfTextPages(csUInt8 numTextPages)
{
  m_numTextPages = numTextPages;
}

csUInt8 cs::DefaultParticleStepper::GetNumberOfTextPages() const
{
  return m_numTextPages;
}

void cs::DefaultParticleStepper::SetTextPageTime(float textPageTime)
{
  m_textPageTime = textPageTime;
}

float cs::DefaultParticleStepper::GetTextPageTime() const
{
  return m_textPageTime;
}

void cs::DefaultParticleStepper::Update(float tpf, cs::Particle *particle)
{
  cs::Particle::ParticleData *data;
  if (particle->GetParticleBuffer()->Lock(0, (void **)&data, cs::eBAM_ReadWrite))
  {
    for (unsigned i = 0; i < particle->GetNumberOfParticles(); ++i)
    {
      cs::Particle::ParticleData *part = &data[i];
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

void cs::DefaultParticleStepper::UpdateParticle(float tpf, cs::Particle::ParticleData* particle)
{
  cs::Vector3f gr(m_gravity);
  gr *= tpf;

  particle->direction += gr;
  cs::Vector3f::MulAdd(particle->position, particle->direction, tpf, particle->position);

  particle->rotation += particle->rotationSpeed * tpf;


  bool updateSize = true;
  float tf = particle->time / m_sizeCicleTime;
  switch (m_sizeMode)
  {
  case cs::ePSM_Constant:
    updateSize = false;
    break;

  case cs::ePSM_Linear:
    if (tf > 1.0f)
    {
      tf = 1.0f;
    }
    break;

  case cs::ePSM_Saw:
    tf = cs::mod(tf, 1.0f);
    break;

  case cs::ePSM_Triangle:
    tf = cs::mod(tf, 1.0f);
    if (tf >= 0.5f)
    {
      tf = (1.0f - tf);
    }
    tf *= 2.0f;
    break;

  case cs::ePSM_Pulse:
    tf *= 3.141569f;
    tf = cs::sin(tf) + 0.5f;
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
  float fact = 1.0f - cs::mod(particle->time, m_textPageTime);

  particle->textPage = cs::Vector3f((float)page, (float)nextPage, fact);

}