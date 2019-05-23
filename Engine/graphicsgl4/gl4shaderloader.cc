
#include <graphicsgl4/gl4shaderloader.hh>
#include <graphicsgl4/gl4shader.hh>
#include <graphicsgl4/gl4enums.hh>
#include <graphicsgl4/gl4defines.hh>


csShaderGL4Loader::csShaderGL4Loader()
{
  CS_CLASS_GEN_CONSTR;
}

csShaderGL4Loader::~csShaderGL4Loader()
{
}

bool csShaderGL4Loader::CanLoad(cs::iFile *file, const cs::ResourceLocator &locator) const
{
  CS_UNUSED(locator);
  std::string ext = file->GetExtension();
  return
    ext == std::string("vert") ||
    ext == std::string("ctrl") ||
    ext == std::string("eval") ||
    ext == std::string("geom") ||
    ext == std::string("frag") ||
    ext == std::string("comp");
}

const cs::Class *csShaderGL4Loader::EvalClass(cs::iFile *file, const cs::ResourceLocator &locator) const
{
  CS_UNUSED(file);
  CS_UNUSED(locator);
  return csShaderGL4::GetStaticClass();
}

cs::ResourceWrapper *csShaderGL4Loader::Load(cs::iFile *file, const cs::ResourceLocator &locator) const
{
  CS_UNUSED(locator);
  std::string ext = file->GetExtension();

  cs::eShaderType type;
  if (ext == std::string("vert"))
  {
    type = cs::eST_Vertex;
  }
  else if (ext == std::string("ctrl"))
  {
    type = cs::eST_TessCtrl;
  }
  else if (ext == std::string("eval"))
  {
    type = cs::eST_TessEval;
  }
  else if (ext == std::string("geom"))
  {
    type = cs::eST_Geometry;
  }
  else if (ext == std::string("frag"))
  {
    type = cs::eST_Fragment;
  }
  else if (ext == std::string("comp"))
  {
    type = cs::eST_Compute;
  }
  else
  {
    return nullptr;
  }



  csSize length = file->GetLength();

  // create a buffer with an appropriet size and read all
  char *buffer = new char[length + 1];
  file->Read(buffer, length);
  buffer[length] = '\0';


  std::string source(buffer);
  delete[] buffer;

  csShaderGL4 *shader = new csShaderGL4();
  shader->SetSource(source);
  shader->SetShaderType(type);

  if (!shader->Compile())
  {
    std::string log = shader->GetCompileErrorLog();
    printf("Code:\n");
    bool newLine = true;
    int line = 1;
    for (auto c : source)
    {
      if (c == '\r')
      {
        continue;
      }

      if (newLine)
      {
        printf("[%02d] ", line++);
        newLine = false;
      }
      printf("%c", c);
      if (c == '\n')
      {
        newLine = true;
      }
    }
    printf("Log:\n%s\n", log.c_str());
    shader->Release();
    return nullptr;
  }

  return new csShaderGL4Wrapper(shader);
}


