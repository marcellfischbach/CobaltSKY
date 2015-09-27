
#include <RenderGL4/ShaderLoader.hh>
#include <RenderGL4/Enums.hh>
#include <RenderGL4/Shader.hh>


vkShaderLoader::vkShaderLoader()
{
  VK_CLASS_GEN_CONSTR;
}

vkShaderLoader::~vkShaderLoader()
{
}

bool vkShaderLoader::CanLoad(IFile *file, const vkResourceLocator &locator) const
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

IObject *vkShaderLoader::Load(IFile *file, const vkResourceLocator &locator) const
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



  // save the current position (we will reset this possition at the end)
  long currentPos = file->Tell();

  // get the length of the file
  file->Seek(eSP_End, 0);
  long length = file->Tell();
  file->Seek(eSP_Set, 0);

  // create a buffer with an appropriet size and read all
  char *buffer = new char[length + 1];
  file->Read(buffer, length);
  buffer[length] = '\0';

  file->Seek(eSP_Set, currentPos);

  vkString source(buffer);
  delete[] buffer;

  vkShader *shader = new vkShader();
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