#pragma once

#include <cobalt/csexport.hh>
#include <cobalt/core/csclass.hh>
#include <cobalt/graphics/shadergraph/cssginput.hh>
#include <cobalt/graphics/shadergraph/cssgoutput.hh>
#include <cobalt/csdefs.hh>
#include <cobalt/csenums.hh>
#include <cobalt/math/csvector.hh>

#include <cobalt/core/csstring.hh>
#include <vector>
#include <cobalt/graphics/shadergraph/cssgnode.refl.hh>



CS_CLASS()
class CSE_API csSGNode : public CS_SUPER(csObject)
{

  CS_CLASS_GEN;
public:
  csSGNode();
  virtual ~csSGNode();

  virtual csSGNode *Copy(csSGNode *node) const;
  const csString &GetName() const;

  //void SetBindingName(const csString &bindingName);
  //const csString &GetBindingName() const;

  csSize GetNumberOfInputs() const;
  csSGInput *GetInput(csSize idx) const;
  csSGInput *GetInput(const csString &name) const;
  csSGDataType GetInputDataType(csSize idx) const;
  csSGDataType GetInputDataType(const csString &name) const;

  void SetOutputDataType(csSize idx, csSGDataType dataType);
  void SetOutputDataType(const csString &name, csSGDataType dataType);

  void SetInput(int inputIdx, csSGNode* node, int outputIdx = 0);
  void SetInput(const csString &inputName, csSGNode *node, int outputIdx = 0);
  void SetInput(int inputIdx, csSGNode* node, const csString &outputName);
  void SetInput(const csString &inputName, csSGNode *node, const csString &outputName);
  void SetInput(int inputIdx, float constFloat);
  void SetInput(const csString &inputName, float constFloat);

  csSGNode *GetInputNode(csSize idx);
  csSGNode *GetInputNode(const csString &name);


  csSize GetNumberOfOutputs() const;
  csSGOutput *GetOutput(csSize idx);
  csSGOutput *GetOutput(const csString &name);


  bool NotInvalid(csSGDataType dt) const;
  bool ScalarType(csSGDataType dt) const;
  bool FloatType(csSGDataType dt) const;
  bool IntType(csSGDataType dt) const;


  bool NotInvalid(csSGDataType dtA, csSGDataType dtB) const;
  bool ScalarType(csSGDataType dtA, csSGDataType dtB) const;
  bool FloatType(csSGDataType dtA, csSGDataType dtB) const;
  bool IntType(csSGDataType dtA, csSGDataType dtB) const;
  bool SameType(csSGDataType dtA, csSGDataType dtB) const;
  bool SameScalarType(csSGDataType dtA, csSGDataType dtB) const;
  bool SameTypeOrOne(csSGDataType dtA, csSGDataType dtB) const;
  csSGDataType HighOrderType(csSGDataType dtA, csSGDataType dtB) const;

  virtual bool Validate();

//protected:
  void SetName(const csString &name);

  void AddInput(csSGInput *input);
  void AddOutput(csSGOutput *output);

  void SetValidationMessage(const csString &validationMessage);
  const csString &GetValidationMessage() const;

#ifdef CS_BUILD_EDITOR
  const csVector2f &GetPosition() const
  {
    return m_position;
  }
  void SetPosition(const csVector2f &position)
  {
    m_position = position;
  }
#endif

private:
  csString m_name;
  //csString m_bindingName;
  csString m_validationMessage;

  std::vector<csSGInput*> m_inputs;
  std::vector<csSGOutput*> m_outputs;

#ifdef CS_BUILD_EDITOR
  csVector2f m_position;
#endif
};


CS_FORCEINLINE const csString &csSGNode::GetName() const
{
  return m_name;
}

/*
CS_FORCEINLINE void csSGNode::SetBindingName(const csString &bindingName)
{
  m_bindingName = bindingName;
}

CS_FORCEINLINE const csString &csSGNode::GetBindingName() const
{
  return m_bindingName;
}
*/

CS_FORCEINLINE void csSGNode::SetName(const csString &name)
{
  m_name = name;
}

CS_FORCEINLINE void csSGNode::SetValidationMessage(const csString &validationMessage)
{
  m_validationMessage = validationMessage;
}

CS_FORCEINLINE const csString &csSGNode::GetValidationMessage() const
{
  return m_validationMessage;
}

