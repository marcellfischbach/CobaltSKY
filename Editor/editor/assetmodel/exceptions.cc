

#include <editor/assetmodel/exceptions.hh>

namespace asset::model
{
	
	NoSuchLocatorException::NoSuchLocatorException (const cs::ResourceLocator &locator)
        : cs::Exception()
		, m_locator(locator)
	{

	}

	




	NoTransactionException::NoTransactionException()
        : cs::Exception()
	{

	}

	NoTransactionException::NoTransactionException(const char *message)
        : cs::Exception(message)
	{

	}

	NoTransactionException::NoTransactionException(const std::string &message)
        : cs::Exception(message.c_str())
	{

	}





	NotAttachedException::NotAttachedException(const Entry *entry)
        : cs::Exception()
		, m_entry(entry)
	{

	}

	NotAttachedException::NotAttachedException(const Entry *entry, const char *message)
        : cs::Exception(message)
		, m_entry(entry)
	{

	}

	NotAttachedException::NotAttachedException(const Entry *entry, const std::string &message)
        : cs::Exception(message.c_str())
		, m_entry(entry)
	{

	}



	ModelStateException::ModelStateException()
        : cs::Exception()
	{

	}

	ModelStateException::ModelStateException(const char *message)
        : cs::Exception(message)
	{

	}

	ModelStateException::ModelStateException(const std::string &message)
        : cs::Exception(message.c_str())
	{

	}

}
