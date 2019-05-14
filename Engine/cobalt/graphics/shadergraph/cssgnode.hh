#pragma once

#include <cobalt/csexport.hh>
#include <csrefl/class.hh>
#include <cobalt/graphics/shadergraph/cssginput.hh>
#include <cobalt/graphics/shadergraph/cssgoutput.hh>
#include <cobalt/csdefs.hh>
#include <cobalt/csenums.hh>
#include <cobalt/math/csvector.hh>

#include <string>
#include <vector>
#include <cobalt/graphics/shadergraph/cssgnode.refl.hh>



CS_CLASS()
class CSE_API csSGNode : public CS_SUPER(cs::Object)
{

  CS_CLASS_GEN;
public:
  csSGNode();
  virtual ~csSGNode();

  virtual csSGNode *Copy(csSGNode *node) const;
  const std::string &GetName() const;

  //void SetBindingName(const std::string &bindingName);
  //const std::string &GetBindingName() const;

  csSize GetNumberOfInputs() const;
  csSGInput *GetInput(csSize idx) const;
  csSGInput *GetInput(const std::string &name) const;
  csSGDataType GetInputDataType(csSize idx) const;
  csSGDataType GetInputDataType(const std::string &name) const;

  void SetOutputDataType(csSize idx, csSGDataType dataType);
  void SetOutputDataType(const std::string &name, csSGDataType dataType);

  void SetInput(int inputIdx, csSGNode* node, int outputIdx = 0);
  void SetInput(const std::string &inputName, csSGNode *node, int outputIdx = 0);
  void SetInput(int inputIdx, csSGNode* node, const std::string &outputName);
  void SetInput(const std::string &inputName, csSGNode *node, const std::string &outputName);
  void SetInput(int inputIdx, float constFloat);
  void SetInput(const std::string &inputName, float constFloat);

  csSGNode *GetInputNode(csSize idx);
  csSGNode *GetInputNode(const std::string &name);


  csSize GetNumberOfOutputs() const;
  csSGOutput *GetOutput(csSize idx);
  csSGOutput *GetOutput(const std::string &name);


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
  void SetName(const std::string &name);

  void AddInput(csSGInput *input);
  void AddOutput(csSGOutput *output);

  void SetValidationMessage(const std::string &validationMessage);
  const std::string &GetValidationMessage() const;

private:
  std::string m_name;
  //std::string m_bindingName;
  std::string m_validationMessage;

  std::vector<csSGInput*> m_inputs;
  std::vector<csSGOutput*> m_outputs;

};


CS_FORCEINLINE const std::string &csSGNode::GetName() const
{
  return m_name;
}

/*
CS_FORCEINLINE void csSGNode::SetBindingName(const std::string &bindingName)
{
  m_bindingName = bindingName;
}

CS_FORCEINLINE const std::string &csSGNode::GetBindingName() const
{
  return m_bindingName;
}
*/

CS_FORCEINLINE void csSGNode::SetName(const std::string &name)
{
  m_name = name;
}

CS_FORCEINLINE void csSGNode::SetValidationMessage(const std::string &validationMessage)
{
  m_validationMessage = validationMessage;
}

CS_FORCEINLINE const std::string &csSGNode::GetValidationMessage() const
{
  return m_validationMessage;
}

