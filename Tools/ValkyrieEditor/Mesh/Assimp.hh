#pragma once


#include <assimp/IOSystem.hpp>
#include <assimp/IOStream.hpp>
#include <assimp/Importer.hpp>

namespace mesh
{


class AssimpIOStream : public Assimp::IOStream
{
public:
  AssimpIOStream();
  virtual ~AssimpIOStream();

  bool Open(const char *filename, const char *openMode);
  void Close();

  virtual size_t Read(void* pvBuffer, size_t pSize, size_t pCount);
  virtual size_t Write(const void* pvBuffer, size_t pSize, size_t pCount);

  virtual aiReturn Seek(size_t pOffset, aiOrigin pOrigin);
  virtual size_t Tell() const;
  virtual size_t FileSize() const;
  virtual void Flush();

private:
  FILE *m_file;

};

class AssimpIOSystem : public Assimp::IOSystem
{
public:
  AssimpIOSystem();
  virtual ~AssimpIOSystem();


  virtual bool Exists(const char* pFile) const;

  virtual char getOsSeparator() const;

  virtual Assimp::IOStream* Open(const char* pFile, const char* pMode = "rb");
  virtual void Close(Assimp::IOStream* pFile);

};


class AssimpImporterFactory
{
public:
  static Assimp::Importer *CreateImporter();

private:
  AssimpImporterFactory();


};


}