

#pragma once

#include <editor/editorexport.hh>

class csResourceLocator;
namespace asset::model
{


	class EDITOR_API ModelSync
	{
		friend class Model;
	public:
		bool Move(const csResourceLocator &oldLocator, const csResourceLocator &newLocator);
		bool Delete(const csResourceLocator &locator);
	private:
		ModelSync();
	};

}