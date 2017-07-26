
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

bool csShaderGL4Loader::CanLoad(iFile *file, const csResourceLocator &locator, iObject *) const
{
  csString ext = file->GetExtension();
  return
    ext == csString("vert") ||
    ext == csString("ctrl") ||
    ext == csString("eval") ||
    ext == csString("geom") ||
    ext == csString("frag") ||
    ext == csString("comp");
}

const csClass *csShaderGL4Loader::EvalClass(iFile *file, const csResourceLocator &locator, iObject *) const
{
  return csShaderGL4::GetStaticClass();
}

iObject *csShaderGL4Loader::Load(iFile *file, const csResourceLocator &locator, iObject *) const
{
  csString ext = file->GetExtension();

  csShaderType type;
  if (ext == csString("vert"))
  {
    type = eST_Vertex;
  }
  else if (ext == csString("ctrl"))
  {
    type = eST_TessCtrl;
  }
  else if (ext == csString("eval"))
  {
    type = eST_TessEval;
  }
  else if (ext == csString("geom"))
  {
    type = eST_Geometry;
  }
  else if (ext == csString("frag"))
  {
    type = eST_Fragment;
  }
  else if (ext == csString("comp"))
  {
    type = eST_Compute;
  }
  else
  {
    return 0;
  }



  csSize length = file->GetLength();

  // create a buffer with an appropriet size and read all
  char *buffer = new char[length + 1];
  file->Read(buffer, length);
  buffer[length] = '\0';


  csString source(buffer);
  delete[] buffer;

  csShaderGL4 *shader = new csShaderGL4();
  shader->SetSource(source);
  shader->SetShaderType(type);

  if (!shader->Compile())
  {
    csString log = shader->GetCompileErrorLog();
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
    return 0;
  }

  return shader;
}


