#pragma once

#include <Valkyrie/Export.hh>
#include <Valkyrie/Core/String.hh>

class VKE_API vkFileInfo
{


public:
	vkFileInfo(const char* filename);

	const char* GetName () const;
	const char* GetExtension () const;
	const char* GetLocation () const;

private:
  vkString m_name;
  vkString m_extension;
  vkString m_location;

};

