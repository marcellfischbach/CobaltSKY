
#include <assetmodel/modeltransaction.hh>


namespace asset::model
{

	ModelTransaction::ModelTransaction()
	{

	}

	ModelTransaction::~ModelTransaction()
	{
		for (iCallback *callback : m_callbacks)
		{
			delete callback;
		}
		m_callbacks.clear();
	}

	void ModelTransaction::Attach(ModelTransaction::iCallback *callback)
	{
		m_callbacks.push_front(callback);
	}

	void ModelTransaction::Finalize(bool commit)
	{
		if (commit)
		{
			Commit();
		}
		else
		{
			Rollback();
		}
	}

	void ModelTransaction::Commit()
	{
		for (iCallback *callback : m_callbacks)
		{
			callback->OnCommit();
		}
	}

	void ModelTransaction::Rollback()
	{
		for (iCallback *callback : m_callbacks)
		{
			callback->OnRollback();
		}
	}


}
