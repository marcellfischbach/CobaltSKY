
#include <GraphicsGL4/gl4shaderloader.hh>
#include <GraphicsGL4/gl4shader.hh>
#include <GraphicsGL4/gl4enums.hh>
#include <GraphicsGL4/gl4defines.hh>


vkShaderGL4Loader::vkShaderGL4Loader()
{
  VK_CLASS_GEN_CONSTR;
}

vkShaderGL4Loader::~vkShaderGL4Loader()
{
}

bool vkShaderGL4Loader::CanLoad(IFile *file, const vkResourceLocator &locator, IObject *) const
{
  vkString ext = file->GetExtension();
  return
    ext == vkString("vert") ||
    ext == vkString("ctrl") ||
    ext == vkString("eval") ||
    ext == vkString("geom") ||
    ext == vkString("frag") ||
    ext == vkString("comp");
}

const vkClass *vkShaderGL4Loader::EvalClass(IFile *file, const vkResourceLocator &locator, IObject *) const
{
  return vkShaderGL4::GetStaticClass();
}

IObject *vkShaderGL4Loader::Load(IFile *file, const vkResourceLocator &locator, IObject *) const
{
  vkString ext = file->GetExtension();

  vkShaderType type;
  if (ext == vkString("vert"))
  {
    type = eST_Vertex;
  }
  else if (ext == vkString("ctrl"))
  {
    type = eST_TessCtrl;
  }
  else if (ext == vkString("eval"))
  {
    type = eST_TessEval;
  }
  else if (ext == vkString("geom"))
  {
    type = eST_Geometry;
  }
  else if (ext == vkString("frag"))
  {
    type = eST_Fragment;
  }
  else if (ext == vkString("comp"))
  {
    type = eST_Compute;
  }
  else
  {
    return 0;
  }



  vkSize length = file->GetLength();

  // create a buffer with an appropriet size and read all
  char *buffer = new char[length + 1];
  file->Read(buffer, length);
  buffer[length] = '\0';


  vkString source(buffer);
  delete[] buffer;

  vkShaderGL4 *shader = new vkShaderGL4();
  shader->SetSource(source);
  shader->SetShaderType(type);

  if (!shader->Compile())
  {
    vkString log = shader->GetCompileErrorLog();
    printf("Code:\n%s\n", source.c_str());
    printf("Log:\n%s\n", log.c_str());
    shader->Release();
    return 0;
  }

  return shader;
}


