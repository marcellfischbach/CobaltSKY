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
VK_CLASS()
class VKE_API vkFileStd : public VK_SUPER(IFile)
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

	virtual vkSize Tell ();

	virtual vkSize GetLength();

	virtual vkSize Read (void* buffer, vkSize size);
  virtual vkSize ReadLine (char* buffer, vkSize maxSize);

	virtual vkSize Write (const void* buffer, vkSize size);

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

