

#include <editor/assetmodel/exceptions.hh>

namespace asset::model
{
	
	NoSuchLocatorException::NoSuchLocatorException (const csResourceLocator &locator)
        : csException()
		, m_locator(locator)
	{

	}

	




	NoTransactionException::NoTransactionException()
        : csException()
	{

	}

	NoTransactionException::NoTransactionException(const char *message)
        : csException(message)
	{

	}

	NoTransactionException::NoTransactionException(const std::string &message)
        : csException(message.c_str())
	{

	}





	NotAttachedException::NotAttachedException(const Entry *entry)
        : csException()
		, m_entry(entry)
	{

	}

	NotAttachedException::NotAttachedException(const Entry *entry, const char *message)
        : csException(message)
		, m_entry(entry)
	{

	}

	NotAttachedException::NotAttachedException(const Entry *entry, const std::string &message)
        : csException(message.c_str())
		, m_entry(entry)
	{

	}



	ModelStateException::ModelStateException()
        : csException()
	{

	}

	ModelStateException::ModelStateException(const char *message)
        : csException(message)
	{

	}

	ModelStateException::ModelStateException(const std::string &message)
        : csException(message.c_str())
	{

	}

}
