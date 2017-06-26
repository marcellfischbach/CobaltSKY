#pragma once

#include <cobalt/csexport.hh>
#include <cobalt/core/ifile.hh>
#include <stdio.h>
#include <string>
#include <cobalt/core/csfilestd.refl.hh>

/**
 * \addtogroup engine
 * @{
 */
/**
 * @brief Reading and writing standard files within the filesystem.
 */
CS_CLASS()
class CSE_API csFileStd : public CS_SUPER(iFile)
{
  CS_CLASS_GEN_OBJECT;
public:
	csFileStd();
	virtual ~csFileStd ();

	bool Open (const char* filename, csOpenMode mode, csTextMode textMode);

	virtual bool IsOpen() const;

	virtual bool Close();

	virtual bool IsRandomAccess () const;

	virtual bool IsReadable () const;

	virtual bool IsWritable () const;

  virtual bool IsEof () const;

	virtual bool Seek (csSeekPos pos, long num);

	virtual csSize Tell ();

	virtual csSize GetLength();

	virtual csSize Read (void* buffer, csSize size);
  virtual csSize ReadLine (char* buffer, csSize maxSize);

	virtual csSize Write (const void* buffer, csSize size);

  virtual const csString& GetLocation () const;

  virtual const csString& GetName () const;

  virtual const csString& GetExtension () const;
private:
  FILE*		m_file;

  csString m_name;
  csString m_extension;
  csString m_location;

  bool		m_read;
  bool		m_write;


};

/** @} */
