

#pragma once

#include <editor/editorexport.hh>
#include <string>
#include <cobalt/core/csresourcelocator.hh>
#include <cobalt/core/csexception.hh>

namespace asset::model
{

	class Entry;
    class NoSuchLocatorException : public cs::Exception
	{

	public:
		NoSuchLocatorException(const cs::ResourceLocator &locator);

		inline const cs::ResourceLocator &GetLocator() const
		{
			return m_locator;
		}

	private:
		cs::ResourceLocator m_locator;
	};


    class NoTransactionException : public cs::Exception
	{

	public:
		NoTransactionException();
		NoTransactionException(const char *message);
		NoTransactionException(const std::string &message);

	};



    class NotAttachedException : public cs::Exception
	{

	public:
		NotAttachedException(const Entry *entry);
		NotAttachedException(const Entry *entry, const char *message);
		NotAttachedException(const Entry *entry, const std::string &message);

		const Entry *GetEntry() const;

	private:
		const Entry * m_entry;
	};


    class ModelStateException : public cs::Exception
	{

	public:
		ModelStateException();
		ModelStateException(const char *message);
		ModelStateException(const std::string &message);

	};


}

