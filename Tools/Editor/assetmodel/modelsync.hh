

#pragma once


class csResourceLocator;
namespace asset::model
{


	class ModelSync
	{
		friend class Model;
	public:
		bool Move(const csResourceLocator &oldLocator, const csResourceLocator &newLocator);
		bool Delete(const csResourceLocator &locator);
	private:
		ModelSync();
	};

}