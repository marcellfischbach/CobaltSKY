#pragma once

#include <Valkyrie/vkexport.hh>
#include <Valkyrie/core/vkclass.hh>
#include <Valkyrie/graphics/shadergraph/vksginput.hh>
#include <Valkyrie/graphics/shadergraph/vksgoutput.hh>
#include <Valkyrie/vkdefs.hh>
#include <Valkyrie/vkenums.hh>
#include <Valkyrie/Math/vkvector.hh>

#include <Valkyrie/core/vkstring.hh>
#include <vector>
#include <Valkyrie/graphics/shadergraph/vksgnode.refl.hh>



VK_CLASS()
class VKE_API vkSGNode : public VK_SUPER(vkObject)
{

  VK_CLASS_GEN;
public:
  vkSGNode();
  virtual ~vkSGNode();

  const vkString &GetName() const;

  //void SetBindingName(const vkString &bindingName);
  //const vkString &GetBindingName() const;

  vkSize GetNumberOfInputs() const;
  vkSGInput *GetInput(vkSize idx) const;
  vkSGInput *GetInput(const vkString &name) const;
  vkSGDataType GetInputDataType(vkSize idx) const;
  vkSGDataType GetInputDataType(const vkString &name) const;

  void SetOutputDataType(vkSize idx, vkSGDataType dataType);
  void SetOutputDataType(const vkString &name, vkSGDataType dataType);

  void SetInput(int inputIdx, vkSGNode* node, int outputIdx = 0);
  void SetInput(const vkString &inputName, vkSGNode *node, int outputIdx = 0);
  void SetInput(int inputIdx, vkSGNode* node, const vkString &outputName);
  void SetInput(const vkString &inputName, vkSGNode *node, const vkString &outputName);
  void SetInput(int inputIdx, float constFloat);
  void SetInput(const vkString &inputName, float constFloat);

  vkSGNode *GetInputNode(vkSize idx);
  vkSGNode *GetInputNode(const vkString &name);


  vkSize GetNumberOfOutputs() const;
  vkSGOutput *GetOutput(vkSize idx);
  vkSGOutput *GetOutput(const vkString &name);


  bool NotInvalid(vkSGDataType dt) const;
  bool ScalarType(vkSGDataType dt) const;
  bool FloatType(vkSGDataType dt) const;
  bool IntType(vkSGDataType dt) const;


  bool NotInvalid(vkSGDataType dtA, vkSGDataType dtB) const;
  bool ScalarType(vkSGDataType dtA, vkSGDataType dtB) const;
  bool FloatType(vkSGDataType dtA, vkSGDataType dtB) const;
  bool IntType(vkSGDataType dtA, vkSGDataType dtB) const;
  bool SameType(vkSGDataType dtA, vkSGDataType dtB) const;
  bool SameScalarType(vkSGDataType dtA, vkSGDataType dtB) const;
  bool SameTypeOrOne(vkSGDataType dtA, vkSGDataType dtB) const;
  vkSGDataType HighOrderType(vkSGDataType dtA, vkSGDataType dtB) const;

  virtual bool Validate();

//protected:
  void SetName(const vkString &name);

  void AddInput(vkSGInput *input);
  void AddOutput(vkSGOutput *output);

  void SetValidationMessage(const vkString &validationMessage);
  const vkString &GetValidationMessage() const;

#ifdef VK_BUILD_EDITOR
  const vkVector2f &GetPosition() const
  {
    return m_position;
  }
  void SetPosition(const vkVector2f &position)
  {
    m_position = position;
  }
#endif

private:
  vkString m_name;
  //vkString m_bindingName;
  vkString m_validationMessage;

  std::vector<vkSGInput*> m_inputs;
  std::vector<vkSGOutput*> m_outputs;

#ifdef VK_BUILD_EDITOR
  vkVector2f m_position;
#endif
};


VK_FORCEINLINE const vkString &vkSGNode::GetName() const
{
  return m_name;
}

/*
VK_FORCEINLINE void vkSGNode::SetBindingName(const vkString &bindingName)
{
  m_bindingName = bindingName;
}

VK_FORCEINLINE const vkString &vkSGNode::GetBindingName() const
{
  return m_bindingName;
}
*/

VK_FORCEINLINE void vkSGNode::SetName(const vkString &name)
{
  m_name = name;
}

VK_FORCEINLINE void vkSGNode::SetValidationMessage(const vkString &validationMessage)
{
  m_validationMessage = validationMessage;
}

VK_FORCEINLINE const vkString &vkSGNode::GetValidationMessage() const
{
  return m_validationMessage;
}

