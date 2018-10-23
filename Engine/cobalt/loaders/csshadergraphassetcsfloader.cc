
#include <cobalt/loaders/csshadergraphassetcsfloader.hh>
#include <cobalt/core/csclassregistry.hh>
#include <cobalt/graphics/shadergraph/cssgnodes.hh>
#include <cobalt/graphics/shadergraph/cssgshadergraph.hh>
#include <cobalt/csengine.hh>
#include <cobalt/graphics/igraphics.hh>
#include <map>





csShaderGraphAssetCSFLoader::csShaderGraphAssetCSFLoader()
  : csBaseCSFLoader()
{

}


csShaderGraphAssetCSFLoader::~csShaderGraphAssetCSFLoader()
{

}

bool csShaderGraphAssetCSFLoader::CanLoad(const csfEntry *entry, const csResourceLocator &locator, iObject *userData) const
{
  return entry->GetTagName() == std::string("shaderGraph");
}

const csClass *csShaderGraphAssetCSFLoader::EvalClass(const csfEntry *entry, const csResourceLocator &locator, iObject *userData) const
{
  return csSGShaderGraph::GetStaticClass();
}

iObject *csShaderGraphAssetCSFLoader::Load(const csfEntry *entry, const csResourceLocator &locator, iObject *userData) const
{
  csSGShaderGraph *shaderGraph = new csSGShaderGraph();

  const csfEntry *nodesEntry = entry->GetEntry("nodes");
  const csfEntry *inputsEntry = entry->GetEntry("inputs");
  const csfEntry *attributesEntry = entry->GetEntry("attributes");

  std::map<csUInt32, csSGNode*> nodes;
  if (nodesEntry)
  {
    for (const csfEntry *nodeElement = nodesEntry->GetEntry("node"); nodeElement; nodeElement = nodeElement->GetSiblingEntry("node"))
    {
      if (!nodeElement->HasAttribute("id") || !nodeElement->HasAttribute("class"))
      {
        continue;
      }

      csUInt32 id = nodeElement->GetAttributeInt("id");
      std::string className = nodeElement->GetAttribute("class");

      const csClass *cls = csClassRegistry::Get()->GetClass(className);
      if (!cls)
      {
        continue;
      }

      csSGNode *node = cls->CreateInstance<csSGNode>();
      if (!node)
      {
        continue;
      }

      // 
      // add the node may be exiting later, but the node must be stored.
      shaderGraph->AddNode(node);
      nodes[id] = node;
      node->Release();


      csSGResourceNode *resource = csQueryClass<csSGResourceNode>(node);
      const csfEntry *resourceElement = nodeElement->GetEntry("resource");
      if (resource && resourceElement)
      {

        if (resourceElement->HasAttribute("name"))
        {
          resource->SetResourceName(resourceElement->GetAttribute("name"));
        }
        if (resourceElement->HasAttribute("id"))
        {
          resource->SetResourceId(resourceElement->GetAttribute("id"));
        }


        const csfEntry *valueElement = resourceElement->GetEntry();
        if (valueElement)
        {
          std::string tagName = valueElement->GetTagName();
          if (tagName == std::string("float"))
          {
            resource->SetDefault(LoadFloat(valueElement));
          }
          else if (tagName == std::string("float2"))
          {
            resource->SetDefault(LoadVector2f(valueElement));
          }
          else if (tagName == std::string("float3"))
          {
            resource->SetDefault(LoadVector3f(valueElement));
          }
          else if (tagName == std::string("float4"))
          {
            resource->SetDefault(LoadVector4f(valueElement));
          }
          else if (tagName == std::string("color4"))
          {
            resource->SetDefault(LoadColor4f(valueElement));
          }
          else if (tagName == std::string("locator") && HasLocator(valueElement))
          {
            resource->SetDefaultTextureResource(GetLocator(valueElement));
          }
        }
      }

    }
  }




  // now read the inputs for the nodes
  if (inputsEntry)
  {
    for (const csfEntry *inputElement = inputsEntry->GetEntry(); inputElement; inputElement = inputElement->GetSiblingEntry())
    {
      csSGOutput *output = 0;
      float fl = 0.0f;
      bool validInput = false;
      const csfEntry *nodeElement = inputElement->GetEntry("node");
      if (nodeElement && nodeElement->HasAttribute("id") && nodeElement->HasAttribute("output"))
      {
        csUInt32 id = nodeElement->GetAttributeInt("id");
        csUInt32 outputID = nodeElement->GetAttributeInt("output");
        std::map<csUInt32, csSGNode*>::iterator it = nodes.find(id);
        if (it == nodes.end())
        {
          continue;
        }
        csSGNode *outputNode = it->second;
        if (!outputNode)
        {
          continue;
        }
        output = outputNode->GetOutput(outputID);
        if (!output)
        {
          continue;
        }
        validInput = true;
      }
      const csfEntry *floatElement = inputElement->GetEntry("float");
      if (floatElement && floatElement->HasAttribute())
      {
        fl = floatElement->GetAttributeFloat();
        validInput = true;
      }

      if (!validInput)
      {
        continue;
      }

      std::string tagName = inputElement->GetTagName();
      if (tagName == std::string("shaderGraph") && inputElement->HasAttribute("input"))
      {
        if (!output)
        {
          continue;
        }
        std::string input = inputElement->GetAttribute("input");
        csSGShaderGraph::InputType it;
        if (input == std::string("Diffuse"))
        {
          it = csSGShaderGraph::eIT_Diffuse;
        }
        else if (input == std::string("Alpha"))
        {
          it = csSGShaderGraph::eIT_Alpha;
        }
        else if (input == std::string("Roughness"))
        {
          it = csSGShaderGraph::eIT_Roughness;
        }
        else if (input == std::string("Normal"))
        {
          it = csSGShaderGraph::eIT_Normal;
        }
        else
        {
          continue;
        }
        shaderGraph->SetInput(it, output);
      }
      else if (tagName == std::string("node") && inputElement->HasAttribute("id") && inputElement->HasAttribute("input"))
      {
        csUInt32 nodeID = inputElement->GetAttributeInt("id");
        csUInt32 inputID = inputElement->GetAttributeInt("input");

        std::map<csUInt32, csSGNode*>::iterator it = nodes.find(nodeID);
        if (it == nodes.end())
        {
          continue;
        }

        csSGNode *inputNode = it->second;
        if (!inputNode)
        {
          continue;
        }

        csSGInput *input = inputNode->GetInput(inputID);
        if (!input)
        {
          continue;
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
        continue;
      }

    }
  }

  if (attributesEntry)
  {
    //
    // load the attributes
    const csfEntry *blendOutWithBinaryGradientElement = attributesEntry->GetEntry("blendOutWithBinaryGradient");
    if (blendOutWithBinaryGradientElement)
    {
      if (LoadBool(blendOutWithBinaryGradientElement, "enabled"))
      {
        shaderGraph->SetBlendOutWithBinaryGradient(LoadBool(blendOutWithBinaryGradientElement, "enabled"));
      }
      else
      {
        shaderGraph->SetBlendOutWithBinaryGradient(false);
      }
    }
    const csfEntry *discardAlphaElement = attributesEntry->GetEntry("discardAlpha");
    if (discardAlphaElement)
    {
      if (LoadBool(discardAlphaElement, "enabled"))
      {
        shaderGraph->SetDiscardAlpha(LoadBool(discardAlphaElement, "enabled"));
        const csfEntry *thresholdElement = discardAlphaElement->GetEntry("threshold");
        const csfEntry *modeElement = discardAlphaElement->GetEntry("mode");
        float threshold = 0.0f;
        csCompareMode compareMode = eCM_Less;
        if (thresholdElement)
        {
          threshold = LoadFloat(thresholdElement);
        }
        if (modeElement && modeElement->HasAttribute())
        {
          std::string cmpStr(modeElement->GetAttribute());
#define CMP(cmp)  (cmpStr == #cmp)  compareMode = eCM_##cmp
          if CMP(LessOrEqual);
          else if CMP(GreaterOrEqual);
          else if CMP(Less);
          else if CMP(Greater);
          else if CMP(Equal);
          else if CMP(NotEqual);
          else if CMP(Never);
          else if CMP(Always);
#undef CMP
        }
        shaderGraph->SetDiscardAlpha(threshold, compareMode);
      }
    }
  }


  iGraphics *graphics = csEng->GetRenderer();
  if (graphics)
  {
    graphics->GetShaderGraphFactory()->GenerateShaderGraph(shaderGraph);
  }


  return new csSGShaderGraphWrapper(shaderGraph);
}
