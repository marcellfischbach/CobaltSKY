
#include <Valkyrie/Loaders/ShaderGraphLoader.hh>
#include <Valkyrie/Core/ClassRegistry.hh>
#include <Valkyrie/Graphics/ShaderGraph/SGNode.hh>
#include <Valkyrie/Graphics/ShaderGraph/SGShaderGraph.hh>
#include <Valkyrie/Engine.hh>
#include <Valkyrie/Graphics/IGraphics.hh>
#include <map>

vkShaderGraphAssetLoader::vkShaderGraphAssetLoader()
  : IAssetLoader()
{
  VK_CLASS_GEN_CONSTR;
}

vkShaderGraphAssetLoader::~vkShaderGraphAssetLoader()
{

}

bool vkShaderGraphAssetLoader::CanLoad(vkAssetInputStream &inputStream, const vkResourceLocator &locator, IObject *userData) const
{
  return true;
}

IObject *vkShaderGraphAssetLoader::Load(vkAssetInputStream &inputStream, const vkResourceLocator &locator, IObject *userData) const
{
  std::map<vkUInt32, vkSGNode *> nodes;


  //
  // Read all nodes
  vkUInt16 numNodes;
  inputStream >> numNodes;
  for (vkUInt16 i = 0; i < numNodes; ++i)
  {
    vkUInt32 idx;
    vkString nodeClassName;
    vkVector2f editorPos;

    inputStream >> idx >> nodeClassName >> editorPos;
    const vkClass *nodeClass = vkClassRegistry::Get()->GetClass(nodeClassName);
    if (!nodeClass)
    {
      Cleanup(nodes, 0);
      return 0;
    }

    vkSGNode *node = nodeClass->CreateInstance<vkSGNode>();
    nodes[idx] = node;
  }

  //
  // Read all node inputs
  vkUInt16 numNodeInputs;
  inputStream >> numNodeInputs;
  for (vkUInt16 i = 0; i < numNodeInputs; ++i)
  {
    vkUInt32 inputNodeIdx, outputNodeIdx;
    vkUInt8 inputNodeInputIdx, outputNodeOutputIdx, inputType;
    float constFloat;

    inputStream
      >> inputNodeIdx
      >> inputNodeInputIdx
      >> inputType
      >> constFloat
      >> outputNodeIdx
      >> outputNodeOutputIdx;

    auto inputIt = nodes.find(inputNodeIdx);
    if (inputIt == nodes.end())
    {
      Cleanup(nodes, 0);
      return 0;
    }
    vkSGNode *inputNode = inputIt->second;


    if (inputType == 0)
    {
      // const input
      inputNode->SetInput(inputNodeInputIdx, constFloat);
    }
    else
    {
      // another node
      auto outputIt = nodes.find(outputNodeIdx);
      if (outputIt == nodes.end())
      {
        Cleanup(nodes, 0);
        return 0;
      }
      vkSGNode *outputNode = outputIt->second;
      inputNode->SetInput(inputNodeInputIdx, outputNode, outputNodeOutputIdx);
    }
  }


  vkSGShaderGraph *shaderGraph = new vkSGShaderGraph();

  vkUInt16 numGraphInputs;
  inputStream >> numGraphInputs;
  for (unsigned i = 0; i < numGraphInputs; ++i)
  {
    vkString attribute;
    vkUInt32 outputNodeIdx;
    vkUInt8 outputNodeOutputIdx;
    inputStream
      >> attribute
      >> outputNodeIdx
      >> outputNodeOutputIdx;

    auto outputIt = nodes.find(outputNodeIdx);
    if (outputIt == nodes.end())
    {
      Cleanup(nodes, shaderGraph);
      return 0;
    }
    vkSGNode *outputNode = outputIt->second;
    vkSGOutput *outputNodeOutput = outputNode->GetOutput(outputNodeOutputIdx);

    if (attribute == vkString("diffuse"))
    {
      shaderGraph->SetDiffuse(outputNodeOutput);
    }
    else if (attribute == vkString("roughness"))
    {
      shaderGraph->SetDiffuse(outputNodeOutput);
    }
    else if (attribute == vkString("alpha"))
    {
      shaderGraph->SetAlpha(outputNodeOutput);
    }
    else
    {
      Cleanup(nodes, shaderGraph);
      return 0;
    }
  }

  //
  // Shader graph attributes
  vkUInt16 numAttributes;
  inputStream >> numAttributes;
  for (vkUInt16 i = 0; i < numAttributes; ++i)
  {
    vkString attributeName;
    inputStream >> attributeName;
    if (attributeName == vkString("blendOutWithBinaryGradient"))
    {
      vkUInt8 blendOutWithBinaryGradient;
      inputStream >> blendOutWithBinaryGradient;
      shaderGraph->SetBlendOutWithBinaryGradient(blendOutWithBinaryGradient != 0);
    }
    else if (attributeName == vkString("discardAlpha"))
    {
      vkUInt8 discardAlpha, compareMode;
      float alphaThreshold;
      inputStream
        >> discardAlpha
        >> alphaThreshold
        >> compareMode;
      shaderGraph->SetDiscardAlpha(discardAlpha != 0);
      shaderGraph->SetDiscardAlpha(alphaThreshold, (vkCompareMode)compareMode);
    }
    else
    {
      Cleanup(nodes, shaderGraph);
      return 0;
    }
  }

  if (!vkEngine::Get()->GetRenderer()->GetShaderGraphFactory()->GenerateShaderGraph(shaderGraph))
  {
    Cleanup(nodes, shaderGraph);
    return 0;
  }

  Cleanup(nodes, 0);
  return shaderGraph;
}

void vkShaderGraphAssetLoader::Cleanup(std::map<vkUInt32, vkSGNode*> &nodes, vkSGShaderGraph *shaderGraph) const
{
  for (auto it = nodes.begin(); it != nodes.end(); ++it)
  {
    if (it->second)
    {
      it->second->Release();
    }
  }
  nodes.clear();
  if (shaderGraph)
  {
    shaderGraph->Release();
  }
}
