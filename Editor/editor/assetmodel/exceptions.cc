

#include <editor/assetmodel/exceptions.hh>

namespace asset::model
{
	
	NoSuchLocatorException::NoSuchLocatorException (const csResourceLocator &locator)
		: std::exception()
		, m_locator(locator)
	{

	}

	




	NoTransactionException::NoTransactionException()
		: std::exception()
	{

	}

	NoTransactionException::NoTransactionException(const char *message)
		: std::exception(message)
	{

	}

	NoTransactionException::NoTransactionException(const std::string &message)
		: std::exception(message.c_str())
	{

	}





	NotAttachedException::NotAttachedException(const Entry *entry)
		: std::exception()
		, m_entry(entry)
	{

	}

	NotAttachedException::NotAttachedException(const Entry *entry, const char *message)
		: std::exception(message)
		, m_entry(entry)
	{

	}

	NotAttachedException::NotAttachedException(const Entry *entry, const std::string &message)
		: std::exception(message.c_str())
		, m_entry(entry)
	{

	}



	ModelStateException::ModelStateException()
		: std::exception()
	{

	}

	ModelStateException::ModelStateException(const char *message)
		: std::exception(message)
	{

	}

	ModelStateException::ModelStateException(const std::string &message)
		: std::exception(message.c_str())
	{

	}

}
