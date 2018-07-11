
#pragma once

#include <assetmanager/actions/assetmanageractioncallback.hh>
#include <string>

namespace asset
{


	class NewAssetAction : public ActionCallback
	{
	public:
		NewAssetAction(const std::string &name, const std::string &defaultName);
		virtual ~NewAssetAction();

		void Callback();


	private:
		std::string m_name;
		std::string m_defaultName;

	};

}
