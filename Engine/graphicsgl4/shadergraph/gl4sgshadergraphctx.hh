#pragma once

#include <cobalt/cstypes.hh>
#include <string>
#include <set>
#include <map>


namespace cs
{
class SGNode;
class SGNodeGL4;
class SGOutput;
class SGInput;
class ShaderGraphGL4;



class ShaderGraphCtx
{
public:
  struct ExternalBinding
  {
    std::string variableName;
    std::string variableType;
    cs::SGNode* node;
    friend bool operator<(const ExternalBinding& b0, const ExternalBinding& b1)
    {
      return b0.variableName < b1.variableName;
    }
  };

public:
  ShaderGraphCtx(cs::ShaderGraphGL4* graph);

  cs::SGNodeGL4* GetNode(cs::SGNode* node);
  cs::SGNodeGL4* GetNode(cs::SGOutput* output);
  cs::SGNodeGL4* GetNode(cs::SGInput* input);

  std::string CreateCode(cs::SGOutput* outputs);

  void EvaluateInlines(std::set<cs::SGOutput*> outputs);
  void GenerateCode(std::set<cs::SGOutput*> outputs);

  void Evaluate(cs::SGNodeGL4* node);
  void Evaluate(cs::SGNode* node);
  void Evaluate(cs::SGOutput* node);
  void Evaluate(cs::SGInput* node);

  void EvaluateInline(cs::SGNodeGL4* node);
  void EvaluateInline(cs::SGNode* node);
  void EvaluateInline(cs::SGOutput* node);
  void EvaluateInline(cs::SGInput* node);

  void AddExpression(const std::string& expression);
  std::string AddAssignment(const std::string& type, const std::string& statement);
  void SetOutputValue(cs::SGOutput* output, const std::string& value);

  bool HasOutputValue(cs::SGOutput* output) const;
  bool HasInputValue(cs::SGInput* input) const;
  std::string GetOutputValue(cs::SGOutput* output);
  std::string GetInputValue(cs::SGInput* input);

  std::string GetFullOutputValue(cs::SGOutput* output);
  std::string GetFullInputValue(cs::SGInput* input);

  std::string GetNextVariable();

  void AddBinding(cs::SGNode* node, const std::string& variableType, const std::string& variableName);
  bool IsBindingApplyingFor(const std::string& bindingName, cs::SGNode* node) const;
  bool IsBindingApplyingFor(const std::string& bindingName, cs::SGOutput* output) const;
  bool IsBindingApplyingFor(const std::string& bindingName, cs::SGInput* input) const;
  std::set<ExternalBinding> GetBindingsFor(std::set<cs::SGOutput*> outputs) const;

  void SetDefaultTextureCoordinate(const std::string& defaultTextureCoordinat);
  const std::string& GetDefaultTextureCoordinate() const;

  const std::string& GetCode() const
  {
    return m_code;
  }

private:
  cs::ShaderGraphGL4* m_graph;

  std::string m_code;
  csUInt32 m_variableCounter;
  std::string m_defaultTextureCoordinate;
  std::map<cs::SGOutput*, std::string> m_outputValue;
  std::map<cs::SGNode*, cs::SGNodeGL4*> m_nodes;
  std::map<cs::SGOutput*, std::string> m_outputToVar;


  std::map<std::string, ExternalBinding> m_unisformBindingNames;
};

}

