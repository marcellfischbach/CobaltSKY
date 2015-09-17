
#ifndef __TEST_HH__
#define __TEST_HH__

#include <Valkyrie/Export.hh>
#include <Valkyrie/Core/Object.hh>
#include <Valkyrie/TestInterface.hh>
#include <Valkyrie/Test.refl.hh>


VK_CLASS()
class VKE_API vkTest : public vkObject, public iTestInterface
{
  VK_CLASS_GEN;
  VK_WEAK_OBJECT(vkObject)

};



#endif /* __TEST_HH__ */
