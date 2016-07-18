
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

bool vkShaderGraphAssetLoader::CanLoad(const vkString &typeID, const vkString &name, const vkResourceLocator &locator, IObject *userData)
{
  bool bTypeID = typeID == "SHADER_GRAPH";
  bool bName = name == "DATA";
  return bTypeID && bName;
}

IObject *vkShaderGraphAssetLoader::Load(vkAssetInputStream &inputStream, const vkResourceLocator &locator, IObject *userData)
{
  std::map<vkUInt32, vkSGNode *> nodes;

  vkSGShaderGraph *shaderGraph = new vkSGShaderGraph();

  vkUInt32 version;
  inputStream >> version;

  //
  // Read all nodes
  vkUInt16 numNodes;
  inputStream >> numNodes;
  for (vkUInt16 i = 0; i < numNodes; ++i)
  {
    vkUInt32 idx;
    vkString nodeClassName;

    inputStream >> idx >> nodeClassName;
    const vkClass *nodeClass = vkClassRegistry::Get()->GetClass(nodeClassName);
    if (!nodeClass)
    {
      Cleanup(nodes, shaderGraph);
      return 0;
    }

    vkSGNode *node = nodeClass->CreateInstance<vkSGNode>();
    nodes[idx] = node;

    vkSGResourceNode *resNode = vkQueryClass<vkSGResourceNode>(node);
    if (resNode)
    {
      vkString name;
      inputStream >> name;
      resNode->SetResourceName(name);
      float* floats = resNode->GetDefaultFloats();
      int* ints = resNode->GetDefaultInts();
      vkResourceLocator &txtLocator = resNode->GetDefaultTextureResource();

      switch (resNode->GetResourceType())
      {
      case eSPT_Float:
        inputStream >> floats[0];
        break;
      case eSPT_Vector2:
        for (int i = 0; i < 2; ++i) inputStream >> floats[i];
        break;
      case eSPT_Vector3:
        for (int i = 0; i < 3; ++i) inputStream >> floats[i];
        break;
      case eSPT_Vector4:
        for (int i = 0; i < 4; ++i) inputStream >> floats[i];
        break;
      case eSPT_Int:
        inputStream >> ints[0];
        break;
      case eSPT_IVector2:
        for (int i = 0; i < 2; ++i) inputStream >> ints[i];
        break;
      case eSPT_IVector3:
        for (int i = 0; i < 3; ++i) inputStream >> ints[i];
        break;
      case eSPT_IVector4:
        for (int i = 0; i < 4; ++i) inputStream >> ints[i];
        break;
      case eSPT_Color4:
        for (int i = 0; i < 4; ++i) inputStream >> floats[i];
        break;
      case eSPT_Matrix3:
        for (int i = 0; i < 9; ++i) inputStream >> floats[i];
        break;
      case eSPT_Matrix4:
        for (int i = 0; i < 16; ++i) inputStream >> floats[i];
        break;
      case eSPT_Texture:
        {
          vkString loc;
          inputStream >> loc;
          txtLocator = vkResourceLocator(loc);
        }
        break;
      }
    }

    shaderGraph->AddNode(node);
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
      Cleanup(nodes, shaderGraph);
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
        Cleanup(nodes, shaderGraph);
        return 0;
      }
      vkSGNode *outputNode = outputIt->second;
      inputNode->SetInput(inputNodeInputIdx, outputNode, outputNodeOutputIdx);
    }
  }



  vkUInt16 numGraphInputs;
  inputStream >> numGraphInputs;
  for (unsigned i = 0; i < numGraphInputs; ++i)
  {
    vkUInt8 inputType;
    vkUInt32 outputNodeIdx;
    vkUInt8 outputNodeOutputIdx;
    inputStream
      >> inputType
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

    shaderGraph->SetInput((vkSGShaderGraph::InputType)inputType, outputNodeOutput);
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
        >> compareMode
        >> alphaThreshold;
      shaderGraph->SetDiscardAlpha(discardAlpha != 0);
      shaderGraph->SetDiscardAlpha(alphaThreshold, (vkCompareMode)compareMode);
    }
    else
    {
      Cleanup(nodes, shaderGraph);
      return 0;
    }
  }
  IGraphics *graphics = vkEngine::Get()->GetRenderer();
  if (graphics && !graphics->GetShaderGraphFactory()->GenerateShaderGraph(shaderGraph))
  {
    Cleanup(nodes, shaderGraph);
    return 0;
  }
  // there are situtations where there is no renderer present (within editor ofcause)

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




vkShaderGraphAssetXMLLoader::vkShaderGraphAssetXMLLoader()
  : vkBaseXMLLoader()
{

}


vkShaderGraphAssetXMLLoader::~vkShaderGraphAssetXMLLoader()
{

}

bool vkShaderGraphAssetXMLLoader::CanLoad(TiXmlElement *element, const vkResourceLocator &locator, IObject *userData) const
{
  vkString tagName(element->Value());
  return tagName == vkString("shadergraph");
}

IObject *vkShaderGraphAssetXMLLoader::Load(TiXmlElement *element, const vkResourceLocator &locator, IObject *userData) const
{
  vkSGShaderGraph *shaderGraph = new vkSGShaderGraph();

  TiXmlElement *nodesElement = element->FirstChildElement("nodes");
  TiXmlElement *inputsElement = element->FirstChildElement("inputs");
  if (!nodesElement || !inputsElement)
  {
    shaderGraph->Release();
    return 0;
  }

  std::map<vkUInt32, vkSGNode*> nodes;
  for (TiXmlElement *nodeElement = nodesElement->FirstChildElement("node"); nodeElement; nodeElement = nodeElement->NextSiblingElement("node"))
  {
    if (!nodeElement->Attribute("id") || !nodeElement->Attribute("class"))
    {
      shaderGraph->Release();
      return 0;
    }

    vkUInt32 id = atoi(nodeElement->Attribute("id"));
    vkString className(nodeElement->Attribute("class"));

    const vkClass *cls = vkClassRegistry::Get()->GetClass(className);
    if (!cls)
    { 
      shaderGraph->Release();
      return 0;
    }

    vkSGNode *node = cls->CreateInstance<vkSGNode>();
    if (!node)
    {
      shaderGraph->Release();
      return 0;
    }

    vkSGResourceNode *resource = vkQueryClass<vkSGResourceNode>(node);
    TiXmlElement *resourceElement = nodeElement->FirstChildElement("resource");
    if (resource && !resourceElement)
    {
      shaderGraph->Release();
      return 0;
    }

    if (resource && resourceElement)
    {
      
      if (!resourceElement->Attribute("name"))
      {
        shaderGraph->Release();
        return 0;
      }

      vkString resourceName = vkString(resourceElement->Attribute("name"));
      resource->SetResourceName(resourceName);

      TiXmlElement *valueElement = resourceElement->FirstChildElement();
      if (valueElement)
      {
        vkString tagName(valueElement->Value());
        if (tagName == vkString("float"))
        {
          resource->SetDefault(LoadFloat(valueElement->Value()));
        }
        else if (tagName == vkString("float2"))
        {
          resource->SetDefault(LoadVector2f(valueElement->Value()));
        }
        else if (tagName == vkString("float3"))
        {
          resource->SetDefault(LoadVector3f(valueElement->Value()));
        }
        else if (tagName == vkString("float4"))
        {
          resource->SetDefault(LoadVector4f(valueElement->Value()));
        }
        else if (tagName == vkString("color4"))
        {
          resource->SetDefault(LoadColor4f(valueElement->Value()));
        }
        else if (tagName == vkString("locator"))
        {
          resource->SetDefaultTextureResource(vkResourceLocator(vkString(valueElement->GetText())));
        }
      }
    }

    shaderGraph->AddNode(node);
    nodes[id] = node;
    node->Release();
  }




  // now read the inputs for the nodes
  for (TiXmlElement *inputElement = inputsElement->FirstChildElement(); inputElement; inputElement = inputElement->NextSiblingElement())
  {
    vkSGOutput *output = 0;
    float fl = 0.0f;
    bool validInput = false;
    TiXmlElement *nodeElement = inputElement->FirstChildElement("node");
    if (nodeElement && nodeElement->Attribute("id") && nodeElement->Attribute("output"))
    {
      vkUInt32 id = atoi(nodeElement->Attribute("id"));
      vkUInt32 outputID = atoi(nodeElement->Attribute("output"));
      vkSGNode *outputNode = nodes[id];
      if (!outputNode)
      {
        shaderGraph->Release();
        return 0;
      }
      output = outputNode->GetOutput(outputID);
      if (!output)
      {
        shaderGraph->Release();
        return 0;
      }
      validInput = true;
    }
    TiXmlElement *floatElement = inputElement->FirstChildElement("float");
    if (floatElement && floatElement->GetText())
    {
      fl = (float)atof(floatElement->GetText());
      validInput = true;
    }

    if (!validInput)
    {
      shaderGraph->Release();
      return 0;
    }

    vkString tagName(inputElement->Value());
    if (tagName == vkString("shadergraph") && inputElement->Attribute("input"))
    {
      if (!output)
      {
        shaderGraph->Release();
        return 0;
      }
      vkString input(inputElement->Attribute("input"));
      vkSGShaderGraph::InputType it;
      if (input == vkString("Diffuse"))
      {
        it = vkSGShaderGraph::eIT_Diffuse;
      }
      else if (input == vkString("Alpha"))
      {
        it = vkSGShaderGraph::eIT_Alpha;
      }
      else if (input == vkString("Roughness"))
      {
        it = vkSGShaderGraph::eIT_Roughness;
      }
      else if (input == vkString("Normal"))
      {
        it = vkSGShaderGraph::eIT_Normal;
      }
      else 
      {
        shaderGraph->Release();
        return 0;
      }
      shaderGraph->SetInput(it, output);
    }
    else if (tagName == vkString("node") && inputElement->Attribute("id") && inputElement->Attribute("input"))
    {
      vkUInt32 nodeID = atoi(inputElement->Attribute("id"));
      vkUInt32 inputID = atoi(inputElement->Attribute("input"));
      vkSGNode *inputNode = nodes[nodeID];
      if (!inputNode)
      {
        shaderGraph->Release();
        return 0;
      }

      vkSGInput *input = inputNode->GetInput(inputID);
      if (!input)
      {
        shaderGraph->Release();
        return 0;
      }
      if (output)
      {
        input->SetInput(output);
      }
      else
      {
        input->SetConst(fl);
      }
    }
    else
    {
      shaderGraph->Release();
      return 0;
    }

  }

  IGraphics *graphics = vkEngine::Get()->GetRenderer();
  if (graphics && !graphics->GetShaderGraphFactory()->GenerateShaderGraph(shaderGraph))
  {
    shaderGraph->Release();
    return 0;
  }


  return shaderGraph;
}
