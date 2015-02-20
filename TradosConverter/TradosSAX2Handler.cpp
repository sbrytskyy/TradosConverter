#include "TradosSAX2Handler.h"

void TradosSAX2Handler::startElement(const   XMLCh* const    uri,
	const   XMLCh* const    localname,
	const   XMLCh* const    qname,
	const   Attributes&     attrs)
{
	if (wcscmp(localname, Tags::TAG_TU) == 0)
	{
		sTags.push(E_TAG_TU);

		tu = new TradosUnit();

		XMLSize_t len = attrs.getLength();
		for (XMLSize_t i = 0; i < len; i++)
		{
			if (XMLString::compareIString(attrs.getQName(i), Tags::ATTR_CREATION_DATE) == 0)
			{
				const XMLCh* value = attrs.getValue(i);
				tu->creationdate(value);
			}
			else if (XMLString::compareIString(attrs.getQName(i), Tags::ATTR_CREATION_ID) == 0)
			{
				const XMLCh* value = attrs.getValue(i);
				tu->creationid(value);
			}
		}
	}
	else if (wcscmp(localname, Tags::TAG_TUV) == 0)
	{
		sTags.push(E_TAG_TUV);

		XMLSize_t len = attrs.getLength();
		for (XMLSize_t i = 0; i < len; i++)
		{
			if (XMLString::compareIString(attrs.getQName(i), Tags::ATTR_XML_LANG) == 0)
			{
				const XMLCh* value = attrs.getValue(i);

				if (xmllang == nullptr)
				{
					xmllang = new std::wstring(value);
				}
			}
		}
	}
	else if (wcscmp(localname, Tags::TAG_SEG) == 0)
	{
		sTags.push(E_TAG_SEG);
	}

	if (DLOG)
	{
		printTabs();
		tabCounter++;
		std::wcout << L"startElement: " << localname;
		std::wcout << std::endl;
	}
}

void TradosSAX2Handler::characters(const XMLCh* const chars, const XMLSize_t length)
{
	if (!sTags.empty())
	{
		int itag = sTags.top();
		switch (itag)
		{
		case E_TAG_SEG:
			if (length > 0)
			{
				if (DLOG)
				{
					printTabs();
					std::wcout << L"characters: " << chars << L", length: " << length;
					std::wcout << std::endl;
				}

				if (xmllang != nullptr)
				{
					std::wstring message(chars);
					tu->pairs()[*xmllang] = message;

					delete xmllang;
					xmllang = nullptr;
				}
			}
			break;
		default:
			break;
		}
	}
}

void TradosSAX2Handler::Store()
{
	if (tu == nullptr)
	{
		return;
	}

	long long id = pTradosStorage->InsertIntoTableTU(*tu);
	if (id > 0)
	{
		tu->id(id);

		std::map<std::wstring, std::wstring>::const_iterator it = tu->pairs().begin();

		for (; it != tu->pairs().end(); it++)
		{
			long long pairId = pTradosStorage->InsertIntoTablePairs(it->first, it->second, id);
		}
	}

	if (DLOG)
	{
		std::wcout << L"Added TradosUnit to DB: " << *tu << std::endl;
	}
}

void TradosSAX2Handler::endElement(const XMLCh* const uri, const XMLCh* const localname, const XMLCh* const qname)
{
	if (DLOG)
	{
		tabCounter--;
		printTabs();
		std::wcout << L"endElement: " << localname;
		std::wcout << std::endl << std::endl;
	}

	if (wcscmp(localname, Tags::TAG_TU) == 0)
	{
		if (tu != nullptr)
		{
			Store();

			counter++;
			if (counter % 1000 == 0)
			{
				std::wcout << L"Added " << counter << L" TradosUnits to DB." << std::endl;
				boost::posix_time::ptime todayUtc(boost::gregorian::day_clock::local_day(), boost::posix_time::second_clock::local_time().time_of_day());
				std::wcout << boost::posix_time::to_simple_wstring(todayUtc) << std::endl;
			}

			delete tu;
			tu = nullptr;
		}
		sTags.pop();
	}
}

void TradosSAX2Handler::fatalError(const SAXParseException& exception)
{
	char* message = XMLString::transcode(exception.getMessage());
	std::wcerr << L"Fatal Error: " << message
		<< L" at line: " << exception.getLineNumber()
		<< std::endl;
	XMLString::release(&message);
}

void TradosSAX2Handler::ignorableWhitespace(const   XMLCh* const chars
	, const  XMLSize_t    length)
{
}

void TradosSAX2Handler::printTabs()
{
	for (int i = 0; i < tabCounter; i++)
	{
		std::wcout << "\t";
	}
}
