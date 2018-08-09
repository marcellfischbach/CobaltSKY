
#include <Mesh/Assimp.hh>

namespace mesh
{

AssimpIOStream::AssimpIOStream()
  : m_file(0)
{

}

AssimpIOStream::~AssimpIOStream()
{
  Close();
}

bool AssimpIOStream::Open(const char *filename, const char *openMode)
{
  Close();
  m_file = fopen(filename, openMode);
  return m_file != 0;
}

void AssimpIOStream::Close()
{
  if (m_file)
  {
    fclose(m_file);
    m_file = 0;
  }
}


size_t AssimpIOStream::Read(void* pvBuffer, size_t pSize, size_t pCount)
{
  if (!m_file)
  {
    return 0;
  }
  return fread(pvBuffer, pSize, pCount, m_file);
}


size_t AssimpIOStream::Write(const void* pvBuffer, size_t pSize, size_t pCount)
{
  if (!m_file)
  {
    return 0;
  }
  return fwrite(pvBuffer, pSize, pCount, m_file);
}


aiReturn AssimpIOStream::Seek(size_t pOffset, aiOrigin pOrigin)
{
  if (!m_file)
  {
    return aiReturn_FAILURE;
  }
  int ret = fseek(m_file, pOffset, pOrigin);
  return ret == 0 ? aiReturn_SUCCESS : aiReturn_FAILURE;
}

size_t AssimpIOStream::Tell() const
{
  if (!m_file)
  {
    return 0;
  }

  return ftell(m_file);
}

size_t AssimpIOStream::FileSize() const
{
  if (!m_file)
  {
    return 0;
  }

  long current = ftell(m_file);
  fseek(m_file, 0, SEEK_END);
  long size = ftell(m_file);
  fseek(m_file, current, SEEK_SET);
  return size;
}

void AssimpIOStream::Flush()
{
  if (!m_file)
  {
    return;
  }

  fflush(m_file);
}






AssimpIOSystem::AssimpIOSystem()
{

}

AssimpIOSystem::~AssimpIOSystem()
{

}

bool AssimpIOSystem::Exists(const char *pFile) const
{
  FILE *file = fopen(pFile, "r");
  if (file == 0)
  {
    return false;
  }
  fclose(file);
  return true;
}

char AssimpIOSystem::getOsSeparator() const
{
#ifdef VK_WIN32
  return '\\';
#else
  return '/';
#endif
}



Assimp::IOStream* AssimpIOSystem::Open(const char* pFile, const char* pMode)
{
  AssimpIOStream *stream = new AssimpIOStream();
  if (!stream->Open(pFile, pMode))
  {
    delete stream;
    stream = 0;
  }
  return stream;
}

void AssimpIOSystem::Close(Assimp::IOStream* pFile)
{
  if (!pFile)
  {
    return;
  }

  AssimpIOStream *stream = static_cast<AssimpIOStream*>(pFile);
  stream->Close();
}



AssimpImporterFactory::AssimpImporterFactory()
{
}



Assimp::Importer *AssimpImporterFactory::CreateImporter()
{
  AssimpIOSystem *ioSystem = new AssimpIOSystem();

  Assimp::Importer *importer = new Assimp::Importer();
  importer->SetIOHandler(ioSystem);

  return importer;
}

}
