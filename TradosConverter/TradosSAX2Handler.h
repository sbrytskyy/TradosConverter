#pragma once

#include <boost/date_time.hpp>

#include <xercesc/sax2/DefaultHandler.hpp>
#include <xercesc/sax2/Attributes.hpp>

#include <stack>
#include <iostream>

#include "TradosStorage.h"

#include "TradosUnit.h"
#include "Tags.h"

#define DLOG false

XERCES_CPP_NAMESPACE_USE

class TradosSAX2Handler : public DefaultHandler
{
public:
	void startElement(
		const   XMLCh* const    uri,
		const   XMLCh* const    localname,
		const   XMLCh* const    qname,
		const   Attributes&     attrs
		);

	void printTabs();

	void endElement
		(
		const XMLCh* const uri,
		const XMLCh* const localname,
		const XMLCh* const qname
		);

	void Store();
	void characters
		(
		const   XMLCh* const    chars
		, const XMLSize_t       length
		);
	void fatalError(const SAXParseException&);
	virtual void ignorableWhitespace
		(
		const   XMLCh* const    chars
		, const XMLSize_t       length
		);

private:
	std::stack<int> sTags;
	TradosUnit* tu;

	std::wstring* xmllang;

	CTradosStorage* pTradosStorage = CTradosStorage::Create();

	enum {
		E_TAG_TU = 0,
		E_TAG_TUV,
		E_TAG_SEG
	};

	int tabCounter = 0;
	int counter = 0;
};

