

#pragma once

#include <editor/editorexport.hh>

namespace cs
{
class ResourceLocator;
}

namespace asset::model
{


	class EDITOR_API ModelSync
	{
		friend class Model;
	public:
		bool Move(const cs::ResourceLocator &oldLocator, const cs::ResourceLocator &newLocator);
		bool Delete(const cs::ResourceLocator &locator);
	private:
		ModelSync();
	};

}