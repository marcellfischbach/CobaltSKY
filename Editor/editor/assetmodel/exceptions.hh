

#pragma once

#include <editor/editorexport.hh>
#include <string>
#include <cobalt/core/csresourcelocator.hh>
#include <cobalt/core/csexception.hh>

namespace asset::model
{

	class Entry;
    class NoSuchLocatorException : public csException
	{

	public:
		NoSuchLocatorException(const csResourceLocator &locator);

		inline const csResourceLocator &GetLocator() const
		{
			return m_locator;
		}

	private:
		csResourceLocator m_locator;
	};


    class NoTransactionException : public csException
	{

	public:
		NoTransactionException();
		NoTransactionException(const char *message);
		NoTransactionException(const std::string &message);

	};



    class NotAttachedException : public csException
	{

	public:
		NotAttachedException(const Entry *entry);
		NotAttachedException(const Entry *entry, const char *message);
		NotAttachedException(const Entry *entry, const std::string &message);

		const Entry *GetEntry() const;

	private:
		const Entry * m_entry;
	};


    class ModelStateException : public csException
	{

	public:
		ModelStateException();
		ModelStateException(const char *message);
		ModelStateException(const std::string &message);

	};


}

