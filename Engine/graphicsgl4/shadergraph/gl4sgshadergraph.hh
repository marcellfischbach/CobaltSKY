#pragma once

#include <graphicsgl4/gl4export.hh>
#include <cobalt/graphics/shadergraph/cssgshadergraph.hh>
#include <cobalt/graphics/shadergraph/cssgnode.hh>
#include <map>

class csSGNodeGL4;
class csGraphicsGL4;

class CSGRAPHICSGL4_API csShaderGraphGL4 : public CS_SUPER(ISGShaderGraphFactory)
{
public:
  csShaderGraphGL4(csGraphicsGL4 *graphics);
  ~csShaderGraphGL4();


  std::string CreateCode(csSGNode *node, csSize output);
  std::string CreateCode(csSGNode *node, const std::string &outputName);

  std::string CreateCode(csSGOutput *output);


  csSGNodeGL4 *CreateNode(const cs::Class *nodeClass);


  virtual bool GenerateShaderGraph(csSGShaderGraph *graph, iSGShaderGraphLogger *logger);

private:
  void GenerateGBuffer(csSGShaderGraph *graph, iSGShaderGraphLogger *logger);
  void GenerateShadow(csSGShaderGraph *graph, unsigned layers, csRenderPass renderPass, iSGShaderGraphLogger *logger);

  void DebugCode(const std::string &title, const std::string &source);

private:
  struct Map 
  {
    const cs::Class *nodeClass;
    const cs::Class *nodeClassGL4;
    Map(const cs::Class *nodeClass, const cs::Class *nodeClassGL4)
      : nodeClass(nodeClass)
      , nodeClassGL4(nodeClassGL4)
    {

    }
  };
  csGraphicsGL4 *m_graphics;
  std::vector<Map> m_classMapping;
  //std::map<const cs::Class*, const cs::Class*> m_classMapping;
};
