#pragma once


#include <GraphicsGL4/Export.hh>
#include <Valkyrie/Core/vkstring.hh>
#include <Valkyrie/Graphics/ShaderGraph/SGNode.hh>
#include <map>
#include <set>
#include <GraphicsGL4/ShaderGraph/ShaderGraphNodeGL4.refl.hh>

class vkSGNodeGL4;
class vkShaderGraphGL4;


class vkShaderGraphCtx
{
public:
  struct ExternalBinding
  {
    vkString variableName;
    vkString variableType;
    vkSGNode *node;
    friend bool operator<(const ExternalBinding &b0, const ExternalBinding &b1)
    {
      return b0.variableName < b1.variableName;
    }
  };

public:
  vkShaderGraphCtx(vkShaderGraphGL4 *graph);

  vkSGNodeGL4 *GetNode(vkSGNode *node);
  vkSGNodeGL4 *GetNode(vkSGOutput *output);
  vkSGNodeGL4 *GetNode(vkSGInput *input);

  vkString CreateCode(vkSGOutput* outputs);

  void EvaluateInlines(std::set<vkSGOutput*> outputs);
  void GenerateCode(std::set<vkSGOutput*> outputs);

  void Evaluate(vkSGNodeGL4 *node);
  void Evaluate(vkSGNode *node);
  void Evaluate(vkSGOutput *node);
  void Evaluate(vkSGInput *node);

  void EvaluateInline(vkSGNodeGL4 *node);
  void EvaluateInline(vkSGNode *node);
  void EvaluateInline(vkSGOutput *node);
  void EvaluateInline(vkSGInput *node);

  void AddExpression(const vkString &expression);
  vkString AddAssignment(const vkString &type, const vkString &statement);
  void SetOutputValue(vkSGOutput *output, const vkString &value);

  bool HasOutputValue(vkSGOutput *output) const;
  bool HasInputValue(vkSGInput *input) const;
  vkString GetOutputValue(vkSGOutput *output);
  vkString GetInputValue(vkSGInput *input);

  vkString GetFullOutputValue(vkSGOutput *output);
  vkString GetFullInputValue(vkSGInput *input);

  vkString GetNextVariable();

  void AddBinding(vkSGNode *node, const vkString &variableType, const vkString &variableName);
  bool IsBindingApplyingFor(const vkString &bindingName, vkSGNode *node) const;
  bool IsBindingApplyingFor(const vkString &bindingName, vkSGOutput *output) const;
  bool IsBindingApplyingFor(const vkString &bindingName, vkSGInput *input) const;
  std::set<ExternalBinding> GetBindingsFor(std::set<vkSGOutput*> outputs) const;

  void SetDefaultTextureCoordinate(const vkString &defaultTextureCoordinat);
  const vkString &GetDefaultTextureCoordinate() const;

  const vkString &GetCode() const
  {
    return m_code;
  }

private:
  vkShaderGraphGL4 *m_graph;

  vkString m_code;
  vkUInt32 m_variableCounter;
  vkString m_defaultTextureCoordinate;
  std::map<vkSGOutput *, vkString> m_outputValue;
  std::map<vkSGNode *, vkSGNodeGL4*> m_nodes;
  std::map<vkSGOutput *, vkString> m_outputToVar;


  std::map<vkString, ExternalBinding> m_unisformBindingNames;
};


VK_INTERFACE()
class VKGL4_API vkSGNodeGL4 : public VK_SUPER(vkObject)
{
  VK_CLASS_GEN;
public:
  vkSGNodeGL4();
  virtual ~vkSGNodeGL4();

  void SetNode(vkSGNode *node)
  {
    m_node = node;
  }

  virtual bool EvaluateInline(vkShaderGraphCtx &ctx);
  virtual bool Evaluate(vkShaderGraphCtx &ctx);

  vkSGNode* GetNode()
  {
    return m_node;
  }

  void SetForceInline(bool forceInline)
  {
    m_forceInline = forceInline;
  }
  void SetInline(bool iinline)
  {
    m_inline = iinline;
  }
  bool IsInline() const
  {
    return m_forceInline || m_inline;
  }


  vkString AssignOutput(vkShaderGraphCtx &ctx, vkSGOutput *output, const vkString &exp, const vkString &type);

  static vkSGDataType GetHigher(vkSGDataType dtA, vkSGDataType dtB);
  static vkString GetDataTypeVar(vkSGDataType dt);

protected:
  virtual void PrivEvaluate(vkShaderGraphCtx &ctx) = 0;
  void SetDoubleInlineEvaluateInput();

private:
  bool m_forceInline;
  bool m_inline;
  vkSGNode *m_node;
  bool m_inlineEvaluated;
  bool m_evaluated;
  bool m_doubleInlineEvaluateInput;

  vkString GetFloat(vkShaderGraphCtx &ctx, int x);
  vkString GetInt(vkShaderGraphCtx &ctx, int x);

};

VK_CLASS()
class VKGL4_API vkSGVarGL4 : public VK_SUPER(vkSGNodeGL4)
{
  VK_CLASS_GEN;
public:
  vkSGVarGL4() : vkSGNodeGL4() { }
  vkSGVarGL4(const vkString &typeName);
  virtual ~vkSGVarGL4 () { }
private:
  vkString m_typeName;
protected:
  void PrivEvaluate(vkShaderGraphCtx &ctx);
};

VK_CLASS()
class VKGL4_API vkSGVarFloatGL4 : public VK_SUPER(vkSGVarGL4)
{
  VK_CLASS_GEN;
public:
  vkSGVarFloatGL4() : vkSGVarGL4("float") { }
  virtual ~vkSGVarFloatGL4() { }
};

VK_CLASS()
class VKGL4_API vkSGVarFloat2GL4 : public VK_SUPER(vkSGVarGL4)
{
  VK_CLASS_GEN;
public:
  vkSGVarFloat2GL4 () : vkSGVarGL4("vec2") { }
  virtual ~vkSGVarFloat2GL4() { }
};

VK_CLASS()
class VKGL4_API vkSGVarFloat3GL4 : public VK_SUPER(vkSGVarGL4)
{
  VK_CLASS_GEN;
public:
  vkSGVarFloat3GL4() : vkSGVarGL4("vec3") { }
  virtual ~vkSGVarFloat3GL4() { }

};

VK_CLASS()
class VKGL4_API vkSGVarFloat4GL4 : public VK_SUPER(vkSGVarGL4)
{
  VK_CLASS_GEN;
public:
  vkSGVarFloat4GL4() : vkSGVarGL4("vec4") { }
  virtual ~vkSGVarFloat4GL4() { }
};


VK_CLASS()
class VKGL4_API vkSGConstFloatGL4 : public VK_SUPER(vkSGNodeGL4)
{
  VK_CLASS_GEN;
public:
  vkSGConstFloatGL4() : vkSGNodeGL4() { }
  virtual ~vkSGConstFloatGL4() { }

protected:
  void PrivEvaluate(vkShaderGraphCtx &ctx);
};


VK_CLASS()
class VKGL4_API vkSGConstFloat2GL4 : public VK_SUPER(vkSGNodeGL4)
{
  VK_CLASS_GEN;
public:
  vkSGConstFloat2GL4() : vkSGNodeGL4() { }
  virtual ~vkSGConstFloat2GL4() { }

protected:
  void PrivEvaluate(vkShaderGraphCtx &ctx);
};

VK_CLASS()
class VKGL4_API vkSGConstFloat3GL4 : public VK_SUPER(vkSGNodeGL4)
{
  VK_CLASS_GEN;
public:
  vkSGConstFloat3GL4() : vkSGNodeGL4() { }
  virtual ~vkSGConstFloat3GL4() { }

protected:
  void PrivEvaluate(vkShaderGraphCtx &ctx);
};

VK_CLASS()
class VKGL4_API vkSGConstFloat4GL4 : public VK_SUPER(vkSGNodeGL4)
{
  VK_CLASS_GEN;
public:
  vkSGConstFloat4GL4() : vkSGNodeGL4() { }
  virtual ~vkSGConstFloat4GL4() { }

protected:
  void PrivEvaluate(vkShaderGraphCtx &ctx);
};

VK_CLASS()
class VKGL4_API vkSGFloat2GL4 : public VK_SUPER(vkSGNodeGL4)
{
  VK_CLASS_GEN;
public:
  vkSGFloat2GL4() : vkSGNodeGL4() { }
  virtual ~vkSGFloat2GL4() { }

protected:
  void PrivEvaluate(vkShaderGraphCtx &ctx);
};

VK_CLASS()
class VKGL4_API vkSGFloat3GL4 : public VK_SUPER(vkSGNodeGL4)
{
  VK_CLASS_GEN;
public:
  vkSGFloat3GL4() : vkSGNodeGL4() { }
  virtual ~vkSGFloat3GL4() { }

protected:
  void PrivEvaluate(vkShaderGraphCtx &ctx);
};

VK_CLASS()
class VKGL4_API vkSGFloat4GL4 : public VK_SUPER(vkSGNodeGL4)
{
  VK_CLASS_GEN;
public:
  vkSGFloat4GL4() : vkSGNodeGL4() { }
  virtual ~vkSGFloat4GL4() { }

protected:
  void PrivEvaluate(vkShaderGraphCtx &ctx);
};


VK_CLASS()
class VKGL4_API vkSGSplitFloat2GL4 : public VK_SUPER(vkSGNodeGL4)
{
  VK_CLASS_GEN;
public:
  vkSGSplitFloat2GL4();
  virtual ~vkSGSplitFloat2GL4() { }

protected:
  void PrivEvaluate(vkShaderGraphCtx &ctx);
};


VK_CLASS()
class VKGL4_API vkSGSplitFloat3GL4 : public VK_SUPER(vkSGNodeGL4)
{
  VK_CLASS_GEN;
public:
  vkSGSplitFloat3GL4();
  virtual ~vkSGSplitFloat3GL4() { }

protected:
  void PrivEvaluate(vkShaderGraphCtx &ctx);
};

VK_CLASS()
class VKGL4_API vkSGSplitFloat4GL4 : public VK_SUPER(vkSGNodeGL4)
{
  VK_CLASS_GEN;
public:
  vkSGSplitFloat4GL4();
  virtual ~vkSGSplitFloat4GL4() { }

protected:
  void PrivEvaluate(vkShaderGraphCtx &ctx);
};

VK_CLASS()
class VKGL4_API vkSGAddGL4 : public VK_SUPER(vkSGNodeGL4)
{
  VK_CLASS_GEN;
public:
  vkSGAddGL4() : vkSGNodeGL4() { }
  virtual ~vkSGAddGL4() { }

protected:
  void PrivEvaluate(vkShaderGraphCtx &ctx);
};

VK_CLASS()
class VKGL4_API vkSGSubGL4 : public VK_SUPER(vkSGNodeGL4)
{
  VK_CLASS_GEN;
public:
  vkSGSubGL4() : vkSGNodeGL4() { }
  virtual ~vkSGSubGL4() { }

protected:
  void PrivEvaluate(vkShaderGraphCtx &ctx);
};

VK_CLASS()
class VKGL4_API vkSGMulGL4 : public VK_SUPER(vkSGNodeGL4)
{
  VK_CLASS_GEN;
public:
  vkSGMulGL4() : vkSGNodeGL4() { }
  virtual ~vkSGMulGL4() { }

protected:
  void PrivEvaluate(vkShaderGraphCtx &ctx);
};

VK_CLASS()
class VKGL4_API vkSGDivGL4 : public VK_SUPER(vkSGNodeGL4)
{
  VK_CLASS_GEN;
public:
  vkSGDivGL4() : vkSGNodeGL4() { }
  virtual ~vkSGDivGL4() { }

protected:
  void PrivEvaluate(vkShaderGraphCtx &ctx);
};

VK_CLASS()
class VKGL4_API vkSGDotGL4 : public VK_SUPER(vkSGNodeGL4)
{
  VK_CLASS_GEN;
public:
  vkSGDotGL4() : vkSGNodeGL4() { }
  virtual ~vkSGDotGL4() { }

protected:
  void PrivEvaluate(vkShaderGraphCtx &ctx);
};

VK_CLASS()
class VKGL4_API vkSGCrossGL4 : public VK_SUPER(vkSGNodeGL4)
{
  VK_CLASS_GEN;
public:
  vkSGCrossGL4() : vkSGNodeGL4() { }
  virtual ~vkSGCrossGL4() { }

protected:
  void PrivEvaluate(vkShaderGraphCtx &ctx);
};

VK_CLASS()
class VKGL4_API vkSGLerpGL4 : public VK_SUPER(vkSGNodeGL4)
{
  VK_CLASS_GEN;
public:
  vkSGLerpGL4() : vkSGNodeGL4() { }
  virtual ~vkSGLerpGL4() { }

protected:
  void PrivEvaluate(vkShaderGraphCtx &ctx);
};

VK_CLASS()
class VKGL4_API vkSGDefaultTextureCoordinateGL4 : public VK_SUPER(vkSGNodeGL4)
{
  VK_CLASS_GEN;
public:
  vkSGDefaultTextureCoordinateGL4() : vkSGNodeGL4() { }
  virtual ~vkSGDefaultTextureCoordinateGL4() { }

protected:
  void PrivEvaluate(vkShaderGraphCtx &ctx);
};



VK_CLASS()
class VKGL4_API vkSGTexture2DGL4 : public VK_SUPER(vkSGNodeGL4)
{
  VK_CLASS_GEN;
public:
  vkSGTexture2DGL4() : vkSGNodeGL4() { }
  virtual ~vkSGTexture2DGL4() { }

protected:
  void PrivEvaluate(vkShaderGraphCtx &ctx);
};
