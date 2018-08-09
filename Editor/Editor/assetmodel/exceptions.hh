

#pragma once

#include <exception>
#include <string>
#include <cobalt/core/csresourcelocator.hh>

namespace asset::model
{

	class Entry;
	class NoSuchLocatorException : public std::exception 
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


	class NoTransactionException : public std::exception
	{

	public:
		NoTransactionException();
		NoTransactionException(const char *message);
		NoTransactionException(const std::string &message);

	};



	class NotAttachedException : public std::exception
	{

	public:
		NotAttachedException(const Entry *entry);
		NotAttachedException(const Entry *entry, const char *message);
		NotAttachedException(const Entry *entry, const std::string &message);

		const Entry *GetEntry() const;

	private:
		const Entry * m_entry;
	};


	class ModelStateException : public std::exception
	{

	public:
		ModelStateException();
		ModelStateException(const char *message);
		ModelStateException(const std::string &message);

	};


}

