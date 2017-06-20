

#include <physicsbullet/bulletcapsulecharactercontroller.hh>
#include <physicsbullet/bulletscene.hh>
#include <cobalt/entity/itransformationcallback.hh>
#include <cobalt/entity/cstransformation.hh>
#include <cobalt/math/csmatrix.hh>
#include <bullet/btBulletDynamicsCommon.h>
#include <bullet/BulletDynamics/Character/btKinematicCharacterController.h>
#include <bullet/BulletCollision/CollisionDispatch/btGhostObject.h>




csBulletCapsuleCharacterController::csBulletCapsuleCharacterController()
  : iPhysicsCapsuleCharacterController()
  , m_characterController(0)
  , m_ghostObject(0)
  , m_transformationCallback(0)
{

}

csBulletCapsuleCharacterController::~csBulletCapsuleCharacterController()
{

}


void csBulletCapsuleCharacterController::Initialize(float height, float radius)
{
  m_capsuleShape = new btCapsuleShapeZ(radius, height - 2.0f * radius);
  m_ghostObject = new btPairCachingGhostObject();
  m_ghostObject->setCollisionShape(m_capsuleShape);
  m_ghostObject->setCollisionFlags(btCollisionObject::CF_CHARACTER_OBJECT);


  m_characterController = new btKinematicCharacterController(m_ghostObject, m_capsuleShape, height / 8.0f, btVector3(0.0, 0.0, 1.));
  m_characterController->setGravity(btVector3 (0.0f, 0.0f, -9.81f));
  m_characterController->setJumpSpeed(7.0f);
//  m_characterController->setGravity(0.01);

}

void csBulletCapsuleCharacterController::SetTransformationCallback(iTransformationCallback *callback)
{
  m_transformationCallback = callback;
}

void csBulletCapsuleCharacterController::AttachToScene(csBulletScene *scene)
{
  btDiscreteDynamicsWorld *world = scene->GetBulletScene();
  world->addCharacter(m_characterController);
  world->addCollisionObject(m_ghostObject, btBroadphaseProxy::CharacterFilter, btBroadphaseProxy::StaticFilter | btBroadphaseProxy::DefaultFilter);
  world->getPairCache()->setInternalGhostPairCallback(new btGhostPairCallback());

}


void csBulletCapsuleCharacterController::DetachFromScene(csBulletScene *scene)
{
  btDiscreteDynamicsWorld *world = scene->GetBulletScene();
  world->removeCharacter(m_characterController);
  world->removeCollisionObject(m_ghostObject);
}

void csBulletCapsuleCharacterController::UpdateCallbacks()
{
  if (m_transformationCallback)
  {
    csMatrix4f trans;
    m_ghostObject->getWorldTransform().getOpenGLMatrix(static_cast<btScalar*>(&trans.m00));
    m_transformationCallback->TransformationChanged(trans);
  }
}

void csBulletCapsuleCharacterController::Warp(const csMatrix4f &transformation)
{
  csVector3f translation;
  transformation.GetTranslation(translation);
  WarpToPosition(translation);

  btTransform trans;
  trans.setFromOpenGLMatrix(static_cast<const btScalar*>(&transformation.m00));
  m_ghostObject->setWorldTransform(trans);
}

void csBulletCapsuleCharacterController::WarpToPosition(const csVector3f &position)
{
  m_characterController->warp(btVector3(position.x, position.y, position.z));
}

void csBulletCapsuleCharacterController::SetLocalWalkDirection(const csVector3f &direction)
{
  btTransform trans = m_ghostObject->getWorldTransform();
  csMatrix4f mat;
  trans.getOpenGLMatrix(static_cast<btScalar*>(&mat.m00));
  csVector3f absDir = csMatrix4f::Mult(mat, direction, absDir);

  m_characterController->setWalkDirection(btVector3(absDir.x, absDir.y, absDir.z));
}


void csBulletCapsuleCharacterController::SetGlobalWalkDirection(const csVector3f &direction)
{
  m_characterController->setWalkDirection(btVector3(direction.x, direction.y, direction.z));
}

void csBulletCapsuleCharacterController::Rotate(float angle)
{
  btTransform trans = m_ghostObject->getWorldTransform();
  btTransform r;
  r.setIdentity();
  r.setRotation(btQuaternion(btVector3(0.0, 0.0, 1.0), angle));
  trans = trans * r;
  m_ghostObject->setWorldTransform(trans);
}


bool csBulletCapsuleCharacterController::IsOnGround() const
{
  return m_characterController->onGround();
}

bool csBulletCapsuleCharacterController::CanJump() const
{
  return m_characterController->canJump();
}

void csBulletCapsuleCharacterController::Jump()
{
  m_characterController->jump(btVector3(0, 0, 0));
}

void csBulletCapsuleCharacterController::SetJumpSpeed(float jumpSpeed)
{
  m_characterController->setJumpSpeed(jumpSpeed);
}

void csBulletCapsuleCharacterController::SetMaxFallSpeed(float fallSpeed)
{
  m_characterController->setFallSpeed(fallSpeed);
}

void csBulletCapsuleCharacterController::SetGravity(const csVector3f &gravity)
{
  // m_characterController->setGravity()
}


void csBulletCapsuleCharacterController::SetGravity(float gravity)
{
  m_characterController->setGravity(btVector3(0.0f, 0.0f, -gravity));
}

void csBulletCapsuleCharacterController::SetMaxSlope(float maxSlope)
{
  m_characterController->setMaxSlope(maxSlope);
}
