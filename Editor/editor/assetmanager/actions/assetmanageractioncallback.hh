

#pragma once

#include <editor/editorexport.hh>

namespace asset::actions
{



	struct EDITOR_API ActionCallback
	{
		virtual ~ActionCallback() {}

		virtual void Callback() = 0;

	};



}