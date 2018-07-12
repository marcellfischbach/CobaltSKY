

#pragma once


namespace asset::actions
{



	struct ActionCallback
	{
		virtual ~ActionCallback() {}

		virtual void Callback() = 0;

	};



}