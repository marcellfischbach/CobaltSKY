

#pragma once


namespace asset
{



	struct ActionCallback
	{
		virtual ~ActionCallback() {}

		virtual void Callback() = 0;

	};



}