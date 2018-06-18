

#pragma once


class csResourceLocator;
namespace asset::model
{


	class ModelSync
	{
		friend class Model;
	public:
		void Move(const csResourceLocator &oldLocator, const csResourceLocator &newLocator);
		void Delete(const csResourceLocator &locator);
	private:
		ModelSync();
	};

}