#pragma once

#include <Valkyrie/Export.hh>
#include <Valkyrie/Core/IFile.hh>
#include <stdio.h>
#include <string>
#include <Valkyrie/Core/FileStd.refl.hh>

/**
 * \addtogroup engine
 * @{
 */
/**
 * @brief Reading and writing standard files within the filesystem.
 */
VK_CLASS();
class VKE_API vkFileStd : public IFile
{
  VK_CLASS_GEN_OBJECT;
public:
	vkFileStd();
	virtual ~vkFileStd ();

	bool Open (const char* filename, vkOpenMode mode, vkTextMode textMode);

	virtual bool IsOpen() const;

	virtual bool Close();

	virtual bool IsRandomAccess () const;

	virtual bool IsReadable () const;

	virtual bool IsWritable () const;

  virtual bool IsEof () const;

	virtual bool Seek (vkSeekPos pos, long num);

	virtual long Tell ();

	virtual long GetLength();

	virtual long Read (void* buffer, long size);
  virtual long ReadLine (char* buffer, long maxSize);

	virtual long Write (const void* buffer, long size);

  virtual const vkString& GetLocation () const;

  virtual const vkString& GetName () const;

  virtual const vkString& GetExtension () const;
private:
  FILE*		m_file;

  vkString m_name;
  vkString m_extension;
  vkString m_location;

  bool		m_read;
  bool		m_write;


};

/** @} */

