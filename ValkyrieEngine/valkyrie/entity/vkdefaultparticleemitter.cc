
#include <valkyrie/entity/vkdefaultparticleemitter.hh>
#include <valkyrie/graphics/vkparticle.hh>

vkDefaultParticleEmitter::vkDefaultParticleEmitter()
  : iParticleEmitter()
  , m_particlesPerSecond(0)
  , m_initialTime(vkRandomRange::Value(0.0f))
  , m_timeToLive(vkRandomRange::Value(10.0f))
  , m_spawnMode(ePSM_Point)
  , m_spawnPos(0.0f, 0.0f, 0.0f)
  , m_initialDirection(vkVector3f(0.0f, 0.0f, 1.0f))
  , m_initialDirectionAngle(vkRandomRange::Value(0.0f))
  , m_initialDirectionSpeed(vkRandomRange::Value(0.0f))
  , m_initialRotation(vkRandomRange::Value(0.0f))
  , m_rotationSpeed(vkRandomRange::Value(0.0f))
  , m_rotationMode(ePRM_Both)
  , m_syncSize(true)
{
  VK_CLASS_GEN_CONSTR;
  SetParticlesPerSecond(20.0f);
}

vkDefaultParticleEmitter::~vkDefaultParticleEmitter()
{

}

void vkDefaultParticleEmitter::SetParticlesPerSecond(float particlesPerSecond)
{
  m_particlesPerSecond = particlesPerSecond;
  m_timeToNextParticle = 1.0f / m_particlesPerSecond;
}

float vkDefaultParticleEmitter::GetParticlesPerSecond() const
{
  return m_particlesPerSecond;
}

void vkDefaultParticleEmitter::SetInitialTime(const vkRandomRange &initialTime)
{
  m_initialTime = initialTime;
}

const vkRandomRange &vkDefaultParticleEmitter::GetInitialTime() const
{
  return m_initialTime;
}

void vkDefaultParticleEmitter::SetTimeToLive(const vkRandomRange& timeToLive)
{
  m_timeToLive = timeToLive;
}

const vkRandomRange &vkDefaultParticleEmitter::GetTimeToLive() const
{
  return m_timeToLive;
}

void vkDefaultParticleEmitter::SetSpawnPos(const vkVector3f &pos)
{
  m_spawnMode = ePSM_Point;
  m_spawnPos = pos;
}

void vkDefaultParticleEmitter::SetSpawnBox(const vkVector3f &min, const vkVector3f &max)
{
  m_spawnMode = ePSM_Box;
  m_spawnBoxMin = min;
  m_spawnBoxMax = max;
}

void vkDefaultParticleEmitter::SetSpawnSphere(const vkVector3f &center, const vkRandomRange &radius)
{
  m_spawnMode = ePSM_Sphere;
  m_spawnPos = center;
  m_spawnRadius = radius;
}

void vkDefaultParticleEmitter::SetInitialDirection(const vkVector3f &direction, const vkRandomRange &angle, const vkRandomRange &speed)
{
  m_initialDirection = direction;
  m_initialDirection.Normalize();
  m_initialDirectionAngle = angle;
  m_initialDirectionSpeed = speed;

  vkVector3f up = vkVector3f(0.0f, 0.0f, 0.0f);
  float upAngle = up.Dot(m_initialDirection);
  if (upAngle < 0.01f)
  {
    up = vkVector3f(1.0f, 0.0f, 0.0f);
  }

  vkVector3f x = vkVector3f::Cross(m_initialDirection, up, x);
  vkVector3f y = vkVector3f::Cross(m_initialDirection, x, y);
  m_initialDirectionMatrix.SetXAxis(x);
  m_initialDirectionMatrix.SetYAxis(y);
  m_initialDirectionMatrix.SetZAxis(m_initialDirection);
}

const vkVector3f &vkDefaultParticleEmitter::GetInitialDirection() const
{
  return m_initialDirection;
}

const vkRandomRange &vkDefaultParticleEmitter::GetInitialDirectionAngle() const
{
  return m_initialDirectionAngle;
}

const vkRandomRange &vkDefaultParticleEmitter::GetInitialDirectionSpeed() const
{
  return m_initialDirectionSpeed;
}

void vkDefaultParticleEmitter::SetInitialRotation(const vkRandomRange &initialRotation)
{
  m_initialRotation = initialRotation;
}

const vkRandomRange &vkDefaultParticleEmitter::GetInitialRotation() const
{
  return m_initialRotation;
}

void vkDefaultParticleEmitter::SetRotationSpeed(const vkRandomRange &rotationSpeed)
{
  m_rotationSpeed = rotationSpeed;
}

const vkRandomRange&vkDefaultParticleEmitter::GetRotationSpeed() const
{
  return m_rotationSpeed;
}

void vkDefaultParticleEmitter::SetRotationMode(vkParticleRotationMode rotationMode)
{
  m_rotationMode = rotationMode;
}

vkParticleRotationMode vkDefaultParticleEmitter::GetRotationMode() const
{
  return m_rotationMode;
}

void vkDefaultParticleEmitter::SetSizeX(const vkRandomRange &sizeX)
{
  m_sizeX = sizeX;
}

const vkRandomRange &vkDefaultParticleEmitter::GetSizeX() const
{
  return m_sizeX;
}

void vkDefaultParticleEmitter::SetSizeY(const vkRandomRange &sizeY)
{
  m_sizeY = sizeY;
}

const vkRandomRange &vkDefaultParticleEmitter::GetSizeY() const
{
  return m_sizeY;
}

void vkDefaultParticleEmitter::SetSyncSize(bool syncSize)
{
  m_syncSize = syncSize;
}

bool vkDefaultParticleEmitter::IsSyncSize() const
{
  return m_syncSize;
}

void vkDefaultParticleEmitter::Update(float tpf, vkParticle *particle)
{
  vkParticle::ParticleData *data = 0;
  m_timeToNextParticle -= tpf;
  vkSize particleID = 0;
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

    for (vkSize in = particle->GetNumberOfParticles(); particleID < in; ++particleID)
    {
      if (data[particleID].timeToLive < 0.0)
      {
        float x = (float)rand() / (float)RAND_MAX;
        float y = (float)rand() / (float)RAND_MAX;
        float z = (float)rand() / (float)RAND_MAX;
        data[particleID].position = vkVector3f(-1.0f + x * 2.0f, -1.0f + y * 2.0f, 10.0f + z * 0.0f);

        switch (m_spawnMode)
        {
        case ePSM_Point:
          data[particleID].position = m_spawnPos;
          break;
        case ePSM_Box:
          data[particleID].position = vkVector3f(
            m_spawnBoxMin.x + (float)rand() / (float)RAND_MAX * (m_spawnBoxMax.x - m_spawnBoxMin.x),
            m_spawnBoxMin.y + (float)rand() / (float)RAND_MAX * (m_spawnBoxMax.y - m_spawnBoxMin.y),
            m_spawnBoxMin.z + (float)rand() / (float)RAND_MAX * (m_spawnBoxMax.z - m_spawnBoxMin.z));
          break;
        case ePSM_Sphere:
          {
            float angleY = (float)M_PI_2 - (float)rand() / (float)RAND_MAX * (float)M_PI;
            float angleZ = (float)rand() / (float)RAND_MAX * (float)M_PI * 2.0f;
            vkVector3f dir(
              cos(angleY) * cos(angleZ),
              cos(angleY) * sin(angleZ),
              sin(angleY)
            );
            vkVector3f::MulAdd(m_spawnPos, dir, m_spawnRadius.Get(), data[particleID].position);
          }
          break;
        }

        float angleY = (float)M_PI_2 - m_initialDirectionAngle.Get();
        float angleZ = (float)rand() / (float)(RAND_MAX * M_PI * 2.0);
        vkVector3f dir(
          cos(angleY) * cos(angleZ),
          cos(angleY) * sin(angleZ),
          sin(angleY)
        );
        vkMatrix3f::Mult(m_initialDirectionMatrix, dir, dir);
        vkVector3f::Mul(dir, m_initialDirectionSpeed.Get(), data[particleID].direction);

        //data[particleID].direction = vkVector3f(-10.0f + x * 20.0f, -10.0f + y * 20.0f, 10.0f + z * 15.0f);


        //vkRandomRange sizeX = vkRandomRange::Range(m_sizeX.Get(), m_sizeX.Get());
        //vkRandomRange sizeY = vkRandomRange::Range(m_sizeY.Get(), m_sizeY.Get());

        if (m_syncSize)
        {
          float size = m_sizeX.Get();
          data[particleID].size = vkVector2f(size, size);
          data[particleID].sizeRange = vkVector4f(m_sizeX.GetMin(), m_sizeX.GetRange(), m_sizeX.GetMin(), m_sizeX.GetRange());
        }
        else
        {
          data[particleID].size = vkVector2f(m_sizeX.Get(), m_sizeY.Get());
          data[particleID].sizeRange = vkVector4f(m_sizeX.GetMin(), m_sizeX.GetRange(), m_sizeY.GetMin(), m_sizeY.GetRange());
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