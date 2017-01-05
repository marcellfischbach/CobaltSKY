

#include <PhysicsBullet/BulletCharacterController.hh>
#include <PhysicsBullet/BulletScene.hh>
#include <Valkyrie/entity/itransformationcallback.hh>
#include <Valkyrie/entity/vktransformation.hh>
#include <Valkyrie/math/vkmatrix.hh>
#include <bullet/btBulletDynamicsCommon.h>
#include <bullet/BulletDynamics/Character/btKinematicCharacterController.h>
#include <bullet/BulletCollision/CollisionDispatch/btGhostObject.h>




vkBulletCapsuleCharacterController::vkBulletCapsuleCharacterController()
  : IPhysicsCapsuleCharacterController()
  , m_characterController(0)
  , m_ghostObject(0)
  , m_transformationCallback(0)
{

}

vkBulletCapsuleCharacterController::~vkBulletCapsuleCharacterController()
{

}


void vkBulletCapsuleCharacterController::Initialize(float height, float radius)
{
  m_capsuleShape = new btCapsuleShapeZ(radius, height - 2.0f * radius);
  m_ghostObject = new btPairCachingGhostObject();
  m_ghostObject->setCollisionShape(m_capsuleShape);
  m_ghostObject->setCollisionFlags(btCollisionObject::CF_CHARACTER_OBJECT);


  m_characterController = new btKinematicCharacterController(m_ghostObject, m_capsuleShape, height / 8.0f, 2 /* Z-Axis up */);
  m_characterController->setGravity(9.81f);
  m_characterController->setJumpSpeed(7.0f);
//  m_characterController->setGravity(0.01);

}

void vkBulletCapsuleCharacterController::SetTransformationCallback(ITransformationCallback *callback)
{
  m_transformationCallback = callback;
}

void vkBulletCapsuleCharacterController::AttachToScene(vkBulletScene *scene)
{
  btDiscreteDynamicsWorld *world = scene->GetBulletScene();
  world->addCharacter(m_characterController);
  world->addCollisionObject(m_ghostObject, btBroadphaseProxy::CharacterFilter, btBroadphaseProxy::StaticFilter | btBroadphaseProxy::DefaultFilter);
  world->getPairCache()->setInternalGhostPairCallback(new btGhostPairCallback());

}


void vkBulletCapsuleCharacterController::DetachFromScene(vkBulletScene *scene)
{
  btDiscreteDynamicsWorld *world = scene->GetBulletScene();
  world->removeCharacter(m_characterController);
  world->removeCollisionObject(m_ghostObject);
}

void vkBulletCapsuleCharacterController::UpdateCallbacks()
{
  if (m_transformationCallback)
  {
    vkMatrix4f trans;
    m_ghostObject->getWorldTransform().getOpenGLMatrix(static_cast<btScalar*>(&trans.m00));
    m_transformationCallback->TransformationChanged(trans);
  }
}

void vkBulletCapsuleCharacterController::Warp(const vkMatrix4f &transformation)
{
  vkVector3f translation;
  transformation.GetTranslation(translation);
  WarpToPosition(translation);

  btTransform trans;
  trans.setFromOpenGLMatrix(static_cast<const btScalar*>(&transformation.m00));
  m_ghostObject->setWorldTransform(trans);
}

void vkBulletCapsuleCharacterController::WarpToPosition(const vkVector3f &position)
{
  m_characterController->warp(btVector3(position.x, position.y, position.z));
}

void vkBulletCapsuleCharacterController::SetLocalWalkDirection(const vkVector3f &direction)
{
  btTransform trans = m_ghostObject->getWorldTransform();
  vkMatrix4f mat;
  trans.getOpenGLMatrix(static_cast<btScalar*>(&mat.m00));
  vkVector3f absDir = vkMatrix4f::Mult(mat, direction, absDir);

  m_characterController->setWalkDirection(btVector3(absDir.x, absDir.y, absDir.z));
}


void vkBulletCapsuleCharacterController::SetGlobalWalkDirection(const vkVector3f &direction)
{
  m_characterController->setWalkDirection(btVector3(direction.x, direction.y, direction.z));
}

void vkBulletCapsuleCharacterController::Rotate(float angle)
{
  btTransform trans = m_ghostObject->getWorldTransform();
  btTransform r;
  r.setIdentity();
  r.setRotation(btQuaternion(btVector3(0.0, 0.0, 1.0), angle));
  trans = trans * r;
  m_ghostObject->setWorldTransform(trans);
}


bool vkBulletCapsuleCharacterController::IsOnGround() const
{
  return m_characterController->onGround();
}

bool vkBulletCapsuleCharacterController::CanJump() const
{
  return m_characterController->canJump();
}

void vkBulletCapsuleCharacterController::Jump()
{
  m_characterController->jump();
}

void vkBulletCapsuleCharacterController::SetJumpSpeed(float jumpSpeed)
{
  m_characterController->setJumpSpeed(jumpSpeed);
}

void vkBulletCapsuleCharacterController::SetMaxFallSpeed(float fallSpeed)
{
  m_characterController->setFallSpeed(fallSpeed);
}

void vkBulletCapsuleCharacterController::SetGravity(const vkVector3f &gravity)
{
  // m_characterController->setGravity()
}


void vkBulletCapsuleCharacterController::SetGravity(float gravity)
{
  m_characterController->setGravity(gravity);
}

void vkBulletCapsuleCharacterController::SetMaxSlope(float maxSlope)
{
  m_characterController->setMaxSlope(maxSlope);
}
