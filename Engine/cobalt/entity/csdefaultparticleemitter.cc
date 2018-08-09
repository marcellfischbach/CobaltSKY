
#include <cobalt/entity/csdefaultparticleemitter.hh>
#include <cobalt/graphics/csparticle.hh>

csDefaultParticleEmitter::csDefaultParticleEmitter()
  : iParticleEmitter()
  , m_particlesPerSecond(0)
  , m_initialTime(csRandomRange::Value(0.0f))
  , m_timeToLive(csRandomRange::Value(10.0f))
  , m_spawnMode(ePSM_Point)
  , m_spawnPos(0.0f, 0.0f, 0.0f)
  , m_initialDirection(csVector3f(0.0f, 0.0f, 1.0f))
  , m_initialDirectionAngle(csRandomRange::Value(0.0f))
  , m_initialDirectionSpeed(csRandomRange::Value(0.0f))
  , m_initialRotation(csRandomRange::Value(0.0f))
  , m_rotationSpeed(csRandomRange::Value(0.0f))
  , m_rotationMode(ePRM_Both)
  , m_syncSize(true)
{
  CS_CLASS_GEN_CONSTR;
  SetParticlesPerSecond(20.0f);
}

csDefaultParticleEmitter::~csDefaultParticleEmitter()
{

}

void csDefaultParticleEmitter::SetParticlesPerSecond(float particlesPerSecond)
{
  m_particlesPerSecond = particlesPerSecond;
  m_timeToNextParticle = 1.0f / m_particlesPerSecond;
}

float csDefaultParticleEmitter::GetParticlesPerSecond() const
{
  return m_particlesPerSecond;
}

void csDefaultParticleEmitter::SetInitialTime(const csRandomRange &initialTime)
{
  m_initialTime = initialTime;
}

const csRandomRange &csDefaultParticleEmitter::GetInitialTime() const
{
  return m_initialTime;
}

void csDefaultParticleEmitter::SetTimeToLive(const csRandomRange& timeToLive)
{
  m_timeToLive = timeToLive;
}

const csRandomRange &csDefaultParticleEmitter::GetTimeToLive() const
{
  return m_timeToLive;
}

void csDefaultParticleEmitter::SetSpawnPos(const csVector3f &pos)
{
  m_spawnMode = ePSM_Point;
  m_spawnPos = pos;
}

void csDefaultParticleEmitter::SetSpawnBox(const csVector3f &min, const csVector3f &max)
{
  m_spawnMode = ePSM_Box;
  m_spawnBoxMin = min;
  m_spawnBoxMax = max;
}

void csDefaultParticleEmitter::SetSpawnSphere(const csVector3f &center, const csRandomRange &radius)
{
  m_spawnMode = ePSM_Sphere;
  m_spawnPos = center;
  m_spawnRadius = radius;
}

void csDefaultParticleEmitter::SetInitialDirection(const csVector3f &direction, const csRandomRange &angle, const csRandomRange &speed)
{
  m_initialDirection = direction;
  m_initialDirection.Normalize();
  m_initialDirectionAngle = angle;
  m_initialDirectionSpeed = speed;

  csVector3f up = csVector3f(0.0f, 0.0f, 0.0f);
  float upAngle = up.Dot(m_initialDirection);
  if (upAngle < 0.01f)
  {
    up = csVector3f(1.0f, 0.0f, 0.0f);
  }

  csVector3f x = csVector3f::Cross(m_initialDirection, up, x);
  csVector3f y = csVector3f::Cross(m_initialDirection, x, y);
  m_initialDirectionMatrix.SetXAxis(x);
  m_initialDirectionMatrix.SetYAxis(y);
  m_initialDirectionMatrix.SetZAxis(m_initialDirection);
}

const csVector3f &csDefaultParticleEmitter::GetInitialDirection() const
{
  return m_initialDirection;
}

const csRandomRange &csDefaultParticleEmitter::GetInitialDirectionAngle() const
{
  return m_initialDirectionAngle;
}

const csRandomRange &csDefaultParticleEmitter::GetInitialDirectionSpeed() const
{
  return m_initialDirectionSpeed;
}

void csDefaultParticleEmitter::SetInitialRotation(const csRandomRange &initialRotation)
{
  m_initialRotation = initialRotation;
}

const csRandomRange &csDefaultParticleEmitter::GetInitialRotation() const
{
  return m_initialRotation;
}

void csDefaultParticleEmitter::SetRotationSpeed(const csRandomRange &rotationSpeed)
{
  m_rotationSpeed = rotationSpeed;
}

const csRandomRange&csDefaultParticleEmitter::GetRotationSpeed() const
{
  return m_rotationSpeed;
}

void csDefaultParticleEmitter::SetRotationMode(csParticleRotationMode rotationMode)
{
  m_rotationMode = rotationMode;
}

csParticleRotationMode csDefaultParticleEmitter::GetRotationMode() const
{
  return m_rotationMode;
}

void csDefaultParticleEmitter::SetSizeX(const csRandomRange &sizeX)
{
  m_sizeX = sizeX;
}

const csRandomRange &csDefaultParticleEmitter::GetSizeX() const
{
  return m_sizeX;
}

void csDefaultParticleEmitter::SetSizeY(const csRandomRange &sizeY)
{
  m_sizeY = sizeY;
}

const csRandomRange &csDefaultParticleEmitter::GetSizeY() const
{
  return m_sizeY;
}

void csDefaultParticleEmitter::SetSyncSize(bool syncSize)
{
  m_syncSize = syncSize;
}

bool csDefaultParticleEmitter::IsSyncSize() const
{
  return m_syncSize;
}

void csDefaultParticleEmitter::Update(float tpf, csParticle *particle)
{
  csParticle::ParticleData *data = 0;
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
        data[particleID].position = csVector3f(-1.0f + x * 2.0f, -1.0f + y * 2.0f, 10.0f + z * 0.0f);

        switch (m_spawnMode)
        {
        case ePSM_Point:
          data[particleID].position = m_spawnPos;
          break;
        case ePSM_Box:
          data[particleID].position = csVector3f(
            m_spawnBoxMin.x + (float)rand() / (float)RAND_MAX * (m_spawnBoxMax.x - m_spawnBoxMin.x),
            m_spawnBoxMin.y + (float)rand() / (float)RAND_MAX * (m_spawnBoxMax.y - m_spawnBoxMin.y),
            m_spawnBoxMin.z + (float)rand() / (float)RAND_MAX * (m_spawnBoxMax.z - m_spawnBoxMin.z));
          break;
        case ePSM_Sphere:
          {
            float angleY = (float)M_PI_2 - (float)rand() / (float)RAND_MAX * (float)M_PI;
            float angleZ = (float)rand() / (float)RAND_MAX * (float)M_PI * 2.0f;
            csVector3f dir(
              cos(angleY) * cos(angleZ),
              cos(angleY) * sin(angleZ),
              sin(angleY)
            );
            csVector3f::MulAdd(m_spawnPos, dir, m_spawnRadius.Get(), data[particleID].position);
          }
          break;
        }

        float angleY = (float)M_PI_2 - m_initialDirectionAngle.Get();
        float angleZ = (float)rand() / (float)(RAND_MAX * M_PI * 2.0);
        csVector3f dir(
          cos(angleY) * cos(angleZ),
          cos(angleY) * sin(angleZ),
          sin(angleY)
        );
        csMatrix3f::Mult(m_initialDirectionMatrix, dir, dir);
        csVector3f::Mul(dir, m_initialDirectionSpeed.Get(), data[particleID].direction);

        //data[particleID].direction = csVector3f(-10.0f + x * 20.0f, -10.0f + y * 20.0f, 10.0f + z * 15.0f);


        //csRandomRange sizeX = csRandomRange::Range(m_sizeX.Get(), m_sizeX.Get());
        //csRandomRange sizeY = csRandomRange::Range(m_sizeY.Get(), m_sizeY.Get());

        if (m_syncSize)
        {
          float size = m_sizeX.Get();
          data[particleID].size = csVector2f(size, size);
          data[particleID].sizeRange = csVector4f(m_sizeX.GetMin(), m_sizeX.GetRange(), m_sizeX.GetMin(), m_sizeX.GetRange());
        }
        else
        {
          data[particleID].size = csVector2f(m_sizeX.Get(), m_sizeY.Get());
          data[particleID].sizeRange = csVector4f(m_sizeX.GetMin(), m_sizeX.GetRange(), m_sizeY.GetMin(), m_sizeY.GetRange());
        }
        data[particleID].rotation = m_initialRotation.Get();
        data[particleID].rotationSpeed = m_rotationSpeed.Get();
        switch (m_rotationMode)
        {
        case ePRM_Neg:
          data[particleID].rotation *= -1.0f;
          data[particleID].rotationSpeed *= -1.0f;
          break;
        case ePRM_Both:
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