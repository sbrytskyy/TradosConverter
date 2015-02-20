#pragma once

#include <map>
#include <string>

#include "Tags.h"

class TradosUnit
{
public:
	TradosUnit();
	virtual ~TradosUnit();

	std::map<std::wstring, std::wstring>& pairs(void) { return m_pairs; }
	const std::map<std::wstring, std::wstring>& pairs(void) const { return m_pairs; }

	const std::wstring& creationdate(void) const { return m_creationdate; }
	void creationdate(const std::wstring& _creationdate) { this->m_creationdate = _creationdate; }
	void creationdate(wchar_t* _creationdate) { this->m_creationdate.assign(_creationdate); }
	
	const std::wstring& creationid(void) const { return m_creationid; }
	void creationid(const std::wstring& _creationid) { this->m_creationid = _creationid; }
	void creationid(wchar_t* _creationid) { this->m_creationid.assign(_creationid); }

	const long long id(void) const { return llId; }
	const void id(long long id) { llId = id; }

private:
	std::map<std::wstring, std::wstring> m_pairs;

	long long llId;
	std::wstring m_creationdate;
	std::wstring m_creationid;
};

std::wostream& operator<<(std::wostream& s, const TradosUnit& tu);
