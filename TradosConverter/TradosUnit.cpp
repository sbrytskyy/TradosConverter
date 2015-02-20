#include "TradosUnit.h"


TradosUnit::TradosUnit()
{
}


TradosUnit::~TradosUnit()
{
}

std::wostream& operator<<(std::wostream& s, const TradosUnit& tu) {
	s << L"TradosUnit [";
	s << L"id=";
	s << tu.id();
	s << L", creationdate='";
	s << tu.creationdate();
	s << L"', creationid='";
	s << tu.creationid();
	s << L"']" << std::endl;
	
	std::map<std::wstring, std::wstring>::const_iterator it = tu.pairs().begin();
	
	for (; it != tu.pairs().end(); it++)
	{
		s << L"\t" << Tags::ATTR_XML_LANG << L"='" << it->first << L"', Text='" << it->second << '\'' << std::endl;
	}

	s << std::flush;
	return(s);
}

