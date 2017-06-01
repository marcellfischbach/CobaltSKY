#pragma once

#include <valkyrie/vkexport.hh>
#include <string>

class VKE_API vkFileInfo
{


public:
	vkFileInfo(const std::string &filename);

	const std::string &GetName () const;
	const std::string &GetExtension () const;
	const std::string &GetLocation () const;

  bool Exists() const;
  static bool Exists(const std::string &filename);
private:
  std::string m_name;
  std::string m_extension;
  std::string m_location;
  std::string m_filname;
};

