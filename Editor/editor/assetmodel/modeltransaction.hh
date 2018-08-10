

#pragma once

#include <editor/editorexport.hh>
#include <list>
#include <functional>

namespace asset::model
{

	class EDITOR_API ModelTransaction
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
    void OnCommit(std::function<void()> commit);
    void OnRollback(std::function<void()> rollback);

		void Finalize(bool commit);
		void Commit();
		void Rollback();

	private:
    struct Callback
    {
      std::function<void()> commit;
      std::function<void()> rollback;
    };


    std::list<Callback> m_callbacks_;
		std::list<iCallback*> m_callbacks;

	};
}