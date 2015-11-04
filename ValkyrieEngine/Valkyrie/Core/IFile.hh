#pragma once

#include <Valkyrie/Export.hh>
#include <Valkyrie/Core/Object.hh>
#include <Valkyrie/Core/String.hh>
#include <Valkyrie/Core/IFile.refl.hh>

/**
 * \addtogroup engine
 * @{
 */

enum vkSeekPos
{
	eSP_Set,
	eSP_Current,
	eSP_End
};

enum vkOpenMode
{
	eOM_Read,
	eOM_Write,
	eOM_ReadWrite
};

enum vkTextMode
{
	eTM_Text,
	eTM_Binary
};

/**
 * @brief Generic interface for handling IO operations on file-like
 *        datasources.
 */
VK_INTERFACE();
struct VKE_API IFile : public virtual IObject
{
  VK_CLASS_GEN;

  /**
   * @brief Returns whether or not the file is opened.
   *
   * @return \a True if the file is currently opened else \a false.
   */
	virtual bool IsOpen () const = 0;

  /**
   * @brief Closes the current file.
   *
   * If the file is not currently opened this method has no effect.
   *
   * @return \a True if the file could be closed else \a false. Returns \a false
   *         if the was was not opened.
   */
	virtual bool Close () = 0;

  /**
   * @brief Return whether or not the file has random access.
   *
   * An standard file within the filesystem usualy has random access, a
   * network connection usualy does not.
   *
   * @return \a True if the file has random access else \a false.
   */
	virtual bool IsRandomAccess () const = 0;

  /**
   * @brief Returns whether or not the user can read from the file.
   *
   * It depends on the type of file and the mode it is opened in.
   *
   * @return \a True if the user can read from the file else \a false.
   */
	virtual bool IsReadable () const = 0;

  /**
   * @brief Returns whether or not the user can write to the file.
   *
   * It depends on the type of file and the mode it is opened in.
   *
   * @return \a True if the user can write to the file else \a false.
   */
  virtual bool IsWritable () const = 0;

  /**
   * @brief Returns whether or not the current read pointer points on
   *        the end of the file.
   *
   * @return \a True if the read pointer points on the end of the file
   *         else \a false.
   */
  virtual bool IsEof () const = 0;

  /**
   * @brief Move the current position within the file.
   *
   * If the file has no random access this method does nothing.
   *
   * @param pos The absolute position from where the number of \
   *            bytes should be interpreted.
   * @param num The number of bytes the current location should be moved.
   */
	virtual bool Seek (vkSeekPos pos, long num) = 0;

  /**
   * @brief Returns the current absolute position within the file.
   *
   * If the file has no random access it will return \a 0 always.
   *
   * @return The current absolute position within the file.
   */
	virtual vkSize Tell () = 0;

	/**
	 * @brief Returns the length of the opened file.
	 *
	 * If this file has no random access (a network stream or any other stream)
	 * this methid will return -1;
	 *
	 * @return The length of the opened file or -1 if the file has no random access.
	 */
	virtual vkSize GetLength () = 0;

  /**
   */
	virtual vkSize Read (void* buffer, vkSize size) = 0;

  /**
   * @brief Read a single line from the buffer
   *
   * @param buffer The output buffer where to put the line
   * @param maxSize The number of max bytes to read
   *
   * @return The number of characters in the output buffer
   */
  virtual vkSize ReadLine (char *buffer, vkSize maxSize) = 0;

	virtual vkSize Write (const void* buffer, vkSize size) = 0;

  /**
   * @brief Returns the location where the file was located.
   *
   * Streaming files without any location like network connections
   * should return an empty string.
   *
   * @return The location of the file
   */
  virtual const vkString& GetLocation () const = 0;

  /**
   * @brief Returns the name of the file witout an optional extension
   *
   * Streaming files without any name like network connections
   * should return an empty string.
   *
   * @return The name of the file without an optional extension
   */
  virtual const vkString& GetName () const = 0;

  /**
   * @brief Returns the optional extension of the file
   *
   * Streaming files without any name like network connections
   * should return an empty string.
   *
   * @return The optional extension of the file
   */
  virtual const vkString& GetExtension () const = 0;
};

/** @} */

