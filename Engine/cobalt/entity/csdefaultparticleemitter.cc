
#include <cobalt/entity/csdefaultparticleemitter.hh>
#include <cobalt/graphics/csparticle.hh>
#include <cobalt/math/csmath.hh>

cs::DefaultParticleEmitter::DefaultParticleEmitter()
  : cs::iParticleEmitter()
  , m_particlesPerSecond(0.0f)
  , m_timeToNextParticle(0.0f)
  , m_initialTime(cs::RandomRange::Value(0.0f))
  , m_timeToLive(cs::RandomRange::Value(10.0f))
  , m_spawnMode(cs::ePSM_Point)
  , m_spawnPos(0.0f, 0.0f, 0.0f)
  , m_initialDirection(cs::Vector3f(0.0f, 0.0f, 1.0f))
  , m_initialDirectionAngle(cs::RandomRange::Value(0.0f))
  , m_initialDirectionSpeed(cs::RandomRange::Value(0.0f))
  , m_initialRotation(cs::RandomRange::Value(0.0f))
  , m_rotationSpeed(cs::RandomRange::Value(0.0f))
  , m_rotationMode(cs::ePRM_Both)
  , m_syncSize(true)
{
  CS_CLASS_GEN_CONSTR;
  SetParticlesPerSecond(20.0f);
}

cs::DefaultParticleEmitter::~DefaultParticleEmitter()
{

}

void cs::DefaultParticleEmitter::SetParticlesPerSecond(float particlesPerSecond)
{
  m_particlesPerSecond = particlesPerSecond;
  m_timeToNextParticle = 1.0f / m_particlesPerSecond;
}

float cs::DefaultParticleEmitter::GetParticlesPerSecond() const
{
  return m_particlesPerSecond;
}

void cs::DefaultParticleEmitter::SetInitialTime(const cs::RandomRange &initialTime)
{
  m_initialTime = initialTime;
}

const cs::RandomRange &cs::DefaultParticleEmitter::GetInitialTime() const
{
  return m_initialTime;
}

void cs::DefaultParticleEmitter::SetTimeToLive(const cs::RandomRange& timeToLive)
{
  m_timeToLive = timeToLive;
}

const cs::RandomRange &cs::DefaultParticleEmitter::GetTimeToLive() const
{
  return m_timeToLive;
}

void cs::DefaultParticleEmitter::SetSpawnPos(const cs::Vector3f &pos)
{
  m_spawnMode = cs::ePSM_Point;
  m_spawnPos = pos;
}

void cs::DefaultParticleEmitter::SetSpawnBox(const cs::Vector3f &min, const cs::Vector3f &max)
{
  m_spawnMode = cs::ePSM_Box;
  m_spawnBoxMin = min;
  m_spawnBoxMax = max;
}

void cs::DefaultParticleEmitter::SetSpawnSphere(const cs::Vector3f &center, const cs::RandomRange &radius)
{
  m_spawnMode = cs::ePSM_Sphere;
  m_spawnPos = center;
  m_spawnRadius = radius;
}

void cs::DefaultParticleEmitter::SetInitialDirection(const cs::Vector3f &direction, const cs::RandomRange &angle, const cs::RandomRange &speed)
{
  m_initialDirection = direction;
  m_initialDirection.Normalize();
  m_initialDirectionAngle = angle;
  m_initialDirectionSpeed = speed;

  cs::Vector3f up = cs::Vector3f(0.0f, 0.0f, 0.0f);
  float upAngle = up.Dot(m_initialDirection);
  if (upAngle < 0.01f)
  {
    up = cs::Vector3f(1.0f, 0.0f, 0.0f);
  }

  cs::Vector3f x = cs::Vector3f::Cross(m_initialDirection, up, x);
  cs::Vector3f y = cs::Vector3f::Cross(m_initialDirection, x, y);
  m_initialDirectionMatrix.SetXAxis(x);
  m_initialDirectionMatrix.SetYAxis(y);
  m_initialDirectionMatrix.SetZAxis(m_initialDirection);
}

const cs::Vector3f &cs::DefaultParticleEmitter::GetInitialDirection() const
{
  return m_initialDirection;
}

const cs::RandomRange &cs::DefaultParticleEmitter::GetInitialDirectionAngle() const
{
  return m_initialDirectionAngle;
}

const cs::RandomRange &cs::DefaultParticleEmitter::GetInitialDirectionSpeed() const
{
  return m_initialDirectionSpeed;
}

void cs::DefaultParticleEmitter::SetInitialRotation(const cs::RandomRange &initialRotation)
{
  m_initialRotation = initialRotation;
}

const cs::RandomRange &cs::DefaultParticleEmitter::GetInitialRotation() const
{
  return m_initialRotation;
}

void cs::DefaultParticleEmitter::SetRotationSpeed(const cs::RandomRange &rotationSpeed)
{
  m_rotationSpeed = rotationSpeed;
}

const cs::RandomRange&cs::DefaultParticleEmitter::GetRotationSpeed() const
{
  return m_rotationSpeed;
}

void cs::DefaultParticleEmitter::SetRotationMode(cs::eParticleRotationMode rotationMode)
{
  m_rotationMode = rotationMode;
}

cs::eParticleRotationMode cs::DefaultParticleEmitter::GetRotationMode() const
{
  return m_rotationMode;
}

void cs::DefaultParticleEmitter::SetSizeX(const cs::RandomRange &sizeX)
{
  m_sizeX = sizeX;
}

const cs::RandomRange &cs::DefaultParticleEmitter::GetSizeX() const
{
  return m_sizeX;
}

void cs::DefaultParticleEmitter::SetSizeY(const cs::RandomRange &sizeY)
{
  m_sizeY = sizeY;
}

const cs::RandomRange &cs::DefaultParticleEmitter::GetSizeY() const
{
  return m_sizeY;
}

void cs::DefaultParticleEmitter::SetSyncSize(bool syncSize)
{
  m_syncSize = syncSize;
}

bool cs::DefaultParticleEmitter::IsSyncSize() const
{
  return m_syncSize;
}

void cs::DefaultParticleEmitter::Update(float tpf, cs::Particle *particle)
{
  cs::Particle::ParticleData *data = nullptr;
  m_timeToNextParticle -= tpf;
  csSize particleID = 0;
  while (true)
  {
    if (m_timeToNextParticle > 0.0f)
    {
      break;
    }

    m_timeToNextParticle += 1.0f / m_particlesPerSecond;
    if (!data)
    {
      particle->LockParticleData(false);
      data = particle->GetParticleData();
    }

    for (csSize in = particle->GetNumberOfParticles(); particleID < in; ++particleID)
    {
      if (data[particleID].timeToLive < 0.0)
      {
        float x = (float)rand() / (float)RAND_MAX;
        float y = (float)rand() / (float)RAND_MAX;
        float z = (float)rand() / (float)RAND_MAX;
        data[particleID].position = cs::Vector3f(-1.0f + x * 2.0f, -1.0f + y * 2.0f, 10.0f + z * 0.0f);

        switch (m_spawnMode)
        {
        case cs::ePSM_Point:
          data[particleID].position = m_spawnPos;
          break;
        case cs::ePSM_Box:
          data[particleID].position = cs::Vector3f(
            m_spawnBoxMin.x + (float)rand() / (float)RAND_MAX * (m_spawnBoxMax.x - m_spawnBoxMin.x),
            m_spawnBoxMin.y + (float)rand() / (float)RAND_MAX * (m_spawnBoxMax.y - m_spawnBoxMin.y),
            m_spawnBoxMin.z + (float)rand() / (float)RAND_MAX * (m_spawnBoxMax.z - m_spawnBoxMin.z));
          break;
        case cs::ePSM_Sphere:
          {
            float angleY = (float)M_PI_2 - (float)rand() / (float)RAND_MAX * (float)M_PI;
            float angleZ = (float)rand() / (float)RAND_MAX * (float)M_PI * 2.0f;
            cs::Vector3f dir(
              cs::cos(angleY) * cs::cos(angleZ),
              cs::cos(angleY) * cs::sin(angleZ),
              cs::sin(angleY)
            );
            cs::Vector3f::MulAdd(m_spawnPos, dir, m_spawnRadius.Get(), data[particleID].position);
          }
          break;
        }

        float angleY = (float)M_PI_2 - m_initialDirectionAngle.Get();
        float angleZ = (float)rand() / (float)(RAND_MAX * M_PI * 2.0);
        cs::Vector3f dir(
          cs::cos(angleY) * cs::cos(angleZ),
          cs::cos(angleY) * cs::sin(angleZ),
          cs::sin(angleY)
        );
        cs::Matrix3f::Mult(m_initialDirectionMatrix, dir, dir);
        cs::Vector3f::Mul(dir, m_initialDirectionSpeed.Get(), data[particleID].direction);

        //data[particleID].direction = cs::Vector3f(-10.0f + x * 20.0f, -10.0f + y * 20.0f, 10.0f + z * 15.0f);


        //cs::RandomRange sizeX = cs::RandomRange::Range(m_sizeX.Get(), m_sizeX.Get());
        //cs::RandomRange sizeY = cs::RandomRange::Range(m_sizeY.Get(), m_sizeY.Get());

        if (m_syncSize)
        {
          float size = m_sizeX.Get();
          data[particleID].size = cs::Vector2f(size, size);
          data[particleID].sizeRange = cs::Vector4f(m_sizeX.GetMin(), m_sizeX.GetRange(), m_sizeX.GetMin(), m_sizeX.GetRange());
        }
        else
        {
          data[particleID].size = cs::Vector2f(m_sizeX.Get(), m_sizeY.Get());
          data[particleID].sizeRange = cs::Vector4f(m_sizeX.GetMin(), m_sizeX.GetRange(), m_sizeY.GetMin(), m_sizeY.GetRange());
        }
        data[particleID].rotation = m_initialRotation.Get();
        data[particleID].rotationSpeed = m_rotationSpeed.Get();
        switch (m_rotationMode)
        {
        case cs::ePRM_Neg:
          data[particleID].rotation *= -1.0f;
          data[particleID].rotationSpeed *= -1.0f;
          break;
        case cs::ePRM_Both:
          if (((float)rand() / (float)RAND_MAX) > 0.5)
          {
            data[particleID].rotation *= -1.0f;
            data[particleID].rotationSpeed *= -1.0f;
          }
          break;
        }

        data[particleID].timeToLive = m_timeToLive.Get();
        data[particleID].time = m_initialTime.Get();
        break;
      }
    }

  }
  if (data)
  {
    particle->UnlockParticleData();
  }

}