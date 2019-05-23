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

namespace cs
{

CS_CLASS()
class CSE_API SGNode : public CS_SUPER(cs::Object)
{

  CS_CLASS_GEN;
public:
  SGNode();
  virtual ~SGNode();

  virtual cs::SGNode* Copy(cs::SGNode * node) const;
  const std::string& GetName() const;

  //void SetBindingName(const std::string &bindingName);
  //const std::string &GetBindingName() const;

  csSize GetNumberOfInputs() const;
  cs::SGInput* GetInput(csSize idx) const;
  cs::SGInput* GetInput(const std::string & name) const;
  cs::eSGDataType GetInputDataType(csSize idx) const;
  cs::eSGDataType GetInputDataType(const std::string & name) const;

  void SetOutputDataType(csSize idx, cs::eSGDataType dataType);
  void SetOutputDataType(const std::string & name, cs::eSGDataType dataType);

  void SetInput(int inputIdx, cs::SGNode * node, int outputIdx = 0);
  void SetInput(const std::string & inputName, cs::SGNode * node, int outputIdx = 0);
  void SetInput(int inputIdx, cs::SGNode * node, const std::string & outputName);
  void SetInput(const std::string & inputName, cs::SGNode * node, const std::string & outputName);
  void SetInput(int inputIdx, float constFloat);
  void SetInput(const std::string & inputName, float constFloat);

  cs::SGNode * GetInputNode(csSize idx);
  cs::SGNode * GetInputNode(const std::string & name);


  csSize GetNumberOfOutputs() const;
  cs::SGOutput * GetOutput(csSize idx);
  cs::SGOutput * GetOutput(const std::string & name);


  bool NotInvalid(cs::eSGDataType dt) const;
  bool ScalarType(cs::eSGDataType dt) const;
  bool FloatType(cs::eSGDataType dt) const;
  bool IntType(cs::eSGDataType dt) const;


  bool NotInvalid(cs::eSGDataType dtA, cs::eSGDataType dtB) const;
  bool ScalarType(cs::eSGDataType dtA, cs::eSGDataType dtB) const;
  bool FloatType(cs::eSGDataType dtA, cs::eSGDataType dtB) const;
  bool IntType(cs::eSGDataType dtA, cs::eSGDataType dtB) const;
  bool SameType(cs::eSGDataType dtA, cs::eSGDataType dtB) const;
  bool SameScalarType(cs::eSGDataType dtA, cs::eSGDataType dtB) const;
  bool SameTypeOrOne(cs::eSGDataType dtA, cs::eSGDataType dtB) const;
  cs::eSGDataType HighOrderType(cs::eSGDataType dtA, cs::eSGDataType dtB) const;

  virtual bool Validate();

  //protected:
  void SetName(const std::string & name);

  void AddInput(cs::SGInput * input);
  void AddOutput(cs::SGOutput * output);

  void SetValidationMessage(const std::string & validationMessage);
  const std::string & GetValidationMessage() const;

private:
  std::string m_name;
  //std::string m_bindingName;
  std::string m_validationMessage;

  std::vector<cs::SGInput*> m_inputs;
  std::vector<cs::SGOutput*> m_outputs;

};

}


CS_FORCEINLINE const std::string &cs::SGNode::GetName() const
{
  return m_name;
}

/*
CS_FORCEINLINE void cs::SGNode::SetBindingName(const std::string &bindingName)
{
  m_bindingName = bindingName;
}

CS_FORCEINLINE const std::string &cs::SGNode::GetBindingName() const
{
  return m_bindingName;
}
*/

CS_FORCEINLINE void cs::SGNode::SetName(const std::string &name)
{
  m_name = name;
}

CS_FORCEINLINE void cs::SGNode::SetValidationMessage(const std::string &validationMessage)
{
  m_validationMessage = validationMessage;
}

CS_FORCEINLINE const std::string &cs::SGNode::GetValidationMessage() const
{
  return m_validationMessage;
}

