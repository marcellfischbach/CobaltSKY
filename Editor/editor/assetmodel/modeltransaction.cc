
#include <editor/assetmodel/modeltransaction.hh>


namespace asset::model
{

	ModelTransaction::ModelTransaction()
	{

	}

	ModelTransaction::~ModelTransaction()
	{
    for (Callback &callback : m_callbacks)
    {
      if (callback.callback)
      {
        delete callback.callback;
      }
		}
		m_callbacks.clear();
	}

	void ModelTransaction::Attach(ModelTransaction::iCallback *callback)
	{
    Callback cb;
    cb.callback = callback;
    cb.commit = nullptr;
    cb.rollback = nullptr;
    m_callbacks.push_back(cb);
	}

  void ModelTransaction::OnCommit(std::function<void()> commit)
  {
    Callback cb;
    cb.callback = nullptr;
    cb.commit = commit;
    cb.rollback = nullptr;
    m_callbacks.push_back(cb);
  }

  void ModelTransaction::OnRollback(std::function<void()> rollback)
  {
    Callback cb;
    cb.callback = nullptr;
    cb.commit = nullptr;
    cb.rollback = rollback;
    m_callbacks.push_back(cb);
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
    for (Callback &callback : m_callbacks)
    {
      if (callback.commit)
      {
        callback.commit();
      }
      if (callback.callback)
      {
        callback.callback->OnCommit();
      }
    }

	}

	void ModelTransaction::Rollback()
	{
    for (auto it = m_callbacks.rbegin(); it != m_callbacks.rend(); ++it)
    {
      if (it->rollback)
      {
        it->rollback();
      }
      if (it->callback)
      {
        it->callback->OnRollback();
      }
    }

	}


}
