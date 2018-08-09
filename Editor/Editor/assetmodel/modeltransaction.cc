
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
		m_callbacks.push_back(callback);
	}

  void ModelTransaction::OnCommit(std::function<void()> commit)
  {
    Callback cb;
    cb.commit = commit;
    cb.rollback = 0;
    m_callbacks_.push_back(cb);
  }

  void ModelTransaction::OnRollback(std::function<void()> rollback)
  {
    Callback cb;
    cb.commit = 0;
    cb.rollback = rollback;
    m_callbacks_.push_back(cb);
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
    for (Callback &callback : m_callbacks_)
    {
      if (callback.commit)
      {
        callback.commit();
      }
    }
		for (iCallback *callback : m_callbacks)
		{
			callback->OnCommit();
		}
	}

	void ModelTransaction::Rollback()
	{
    for (auto it = m_callbacks_.rbegin(); it != m_callbacks_.rend(); ++it)
    {
      if (it->rollback)
      {
        it->rollback();
      }
    }
    for (auto it = m_callbacks.rbegin(); it != m_callbacks.rend(); ++it)
    {
      (*it)->OnRollback();
    }
	}


}
