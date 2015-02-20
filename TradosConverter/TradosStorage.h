#pragma once

#include "sqlite3.h"
#include "TradosUnit.h"

#define DB_FILENAME "trados.db"

class CTradosStorage
{
public:
	static CTradosStorage* Create(void);
	~CTradosStorage(void);

	long long InsertIntoTableTU(const TradosUnit& tu);
	long long InsertIntoTablePairs(const std::wstring& xmlLang, const std::wstring& text, long long tuId);

protected:
	CTradosStorage(void);

	bool Init(void);

private:
	sqlite3* m_pDB;

	static const char* m_szTableNameTU;
	static const char* m_szCreateTableTU;
	static const char* m_szInsertIntoTableTU;

	static const char* m_szTableNamePairs;
	static const char* m_szCreateTablePairs;
	static const char* m_szInsertIntoTablePairs;

	int FormatQuery(std::string& Query, const char* pszFormat, ...);
	bool CreateTables(void);
	bool CreateTable(const char* m_szCreateTable, const char* m_szTableName);
};

