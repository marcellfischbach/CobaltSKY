

#pragma once

#include <list>

namespace asset::model
{

	class ModelTransaction
	{
	public:
		struct iCallback
		{
			virtual ~iCallback() { }

			virtual void OnCommit() = 0;
			virtual void OnRollback() = 0;
		};

	public:
		ModelTransaction();
		~ModelTransaction();

		void Attach(iCallback *callbak);

		void Finalize(bool commit);
		void Commit();
		void Rollback();

	private:
		std::list<iCallback*> m_callbacks;

	};
}