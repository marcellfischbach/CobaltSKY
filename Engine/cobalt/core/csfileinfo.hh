#pragma once

#include <cobalt/csexport.hh>
#include <string>

class CSE_API csFileInfo
{


public:
	csFileInfo(const std::string &filename);

	const std::string &GetName () const;
	const std::string &GetExtension () const;
	const std::string &GetLocation () const;

  void Lower();
  void Upper();

  bool IsFile() const;
  bool IsFolder() const;
  bool Exists() const;
  static bool IsFile(const std::string &filename);
  static bool IsFolder(const std::string &filename);
  static bool Exists(const std::string &filename);
private:
  std::string m_name;
  std::string m_extension;
  std::string m_location;
  std::string m_fileName;
};

