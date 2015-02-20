#include <iostream>

#include "TradosStorage.h"
#include "Util.h"


CTradosStorage::CTradosStorage()
{
}


CTradosStorage::~CTradosStorage()
{
	if (m_pDB != NULL){
		sqlite3_close(m_pDB);
		m_pDB = NULL;
	}
}

CTradosStorage* CTradosStorage::Create(void)
{
	CTradosStorage* pTradosStorage = new CTradosStorage();
	if (pTradosStorage != NULL)
	{
		if (!pTradosStorage->Init())
		{
			delete pTradosStorage;
			pTradosStorage = NULL;
		}
	}
	return pTradosStorage;
}

bool CTradosStorage::Init(void)
{
	int rc = 0;
	char *zErrMsg = 0;

	rc = sqlite3_open(DB_FILENAME, &m_pDB);

	if (rc)
	{
		std::wcerr << L"Error opening database: " << sqlite3_errmsg(m_pDB) << std::endl;
		return false;
	}

	bool result = CreateTables();

	return result;
}

long long CTradosStorage::InsertIntoTableTU(const TradosUnit& tu)
{
	if (m_pDB == NULL)
	{
		return 0;
	}

	std::string cd = CUtil::utf8_encode(tu.creationdate());
	std::string ci = CUtil::utf8_encode(tu.creationid());

	std::string Query;
	FormatQuery(Query, m_szInsertIntoTableTU, m_szTableNameTU, cd.c_str(), ci.c_str());

	sqlite3_stmt* pStatement = NULL;

	long long llId = 0;

	if (sqlite3_prepare_v2(m_pDB, Query.c_str(), Query.length(), &pStatement, NULL) == SQLITE_OK)
	{
		if (sqlite3_step(pStatement) == SQLITE_DONE)
		{
			llId = sqlite3_last_insert_rowid(m_pDB);
		}
		else
		{
			std::cerr << "Error inserting into table: " << sqlite3_errmsg(m_pDB) << std::endl;
		}
		sqlite3_finalize(pStatement);
		pStatement = NULL;
	}
	else
	{
		std::cerr << "Error inserting into table: " << sqlite3_errmsg(m_pDB) << std::endl;
	}

	return llId;
}

long long CTradosStorage::InsertIntoTablePairs(const std::wstring& xmlLang, const std::wstring& text, long long tuId)
{
	if (m_pDB == NULL)
	{
		return 0;
	}

	std::string xmlLang8 = CUtil::utf8_encode(xmlLang);
	std::string text8 = CUtil::utf8_encode(text);

	std::string Query;
	FormatQuery(Query, m_szInsertIntoTablePairs, m_szTableNamePairs, xmlLang8.c_str(), text8.c_str(), tuId);

	sqlite3_stmt* pStatement = NULL;

	long long llId = 0;

	if (sqlite3_prepare_v2(m_pDB, Query.c_str(), Query.length(), &pStatement, NULL) == SQLITE_OK)
	{
		if (sqlite3_step(pStatement) == SQLITE_DONE)
		{
			llId = sqlite3_last_insert_rowid(m_pDB);
		}
		else
		{
			std::cerr << "Error inserting into table: " << sqlite3_errmsg(m_pDB) << std::endl;
		}
		sqlite3_finalize(pStatement);
		pStatement = NULL;
	}
	else
	{
		std::cerr << "Error inserting into table: " << sqlite3_errmsg(m_pDB) << std::endl;
	}

	return llId;
}

int CTradosStorage::FormatQuery(std::string& Query, const char* pszFormat, ...)
{
	va_list ap;

	va_start(ap, pszFormat);

	char* zSQL = sqlite3_vmprintf(pszFormat, ap);

	va_end(ap);

	Query = zSQL;

	sqlite3_free(zSQL);
	zSQL = NULL;

	return Query.length();
}

bool CTradosStorage::CreateTables(void)
{
	return CreateTable(m_szCreateTableTU, m_szTableNameTU)
		&& CreateTable(m_szCreateTablePairs, m_szTableNamePairs);
}


bool CTradosStorage::CreateTable(const char* m_szCreateTable, const char* m_szTableName)
{
	bool result = true;
	sqlite3_stmt* pStatement = NULL;
	std::string Query;
	FormatQuery(Query, m_szCreateTable, m_szTableName);

	if (sqlite3_prepare_v2(m_pDB, Query.c_str(), Query.length(), &pStatement, NULL) != SQLITE_OK)
	{
		std::wcerr << L"Error creating table '" << m_szTableName << "'" << sqlite3_errmsg(m_pDB) << std::endl;
		result = false;
	}
	else
		if (sqlite3_step(pStatement) != SQLITE_DONE)
		{
			std::wcerr << L"Error creating table '" << m_szTableName << "'" << sqlite3_errmsg(m_pDB) << std::endl;
			result = false;
		}

	sqlite3_finalize(pStatement);
	pStatement = NULL;

	return result;
}

const char* CTradosStorage::m_szInsertIntoTableTU = "INSERT INTO %q (CreationDate, CreationId) VALUES ('%q', '%q')";
const char* CTradosStorage::m_szCreateTableTU = "CREATE TABLE IF NOT EXISTS %Q ("  \
												"Id INTEGER PRIMARY KEY AUTOINCREMENT NOT NULL, " \
												"CreationDate TEXT NOT NULL, "	\
												"CreationId TEXT NOT NULL, "	\
												"Timestamp TIMESTAMP DEFAULT CURRENT_TIMESTAMP);";
const char* CTradosStorage::m_szTableNameTU = "TU";

const char* CTradosStorage::m_szInsertIntoTablePairs = "INSERT INTO %q (XmlLang, Message, TuId) VALUES ('%q', '%q', %d)";
const char* CTradosStorage::m_szCreateTablePairs = "CREATE TABLE IF NOT EXISTS %Q ("  \
												"Id INTEGER PRIMARY KEY AUTOINCREMENT NOT NULL, " \
												"XmlLang TEXT NOT NULL, "	\
												"Message TEXT NOT NULL, "	\
												"TuId INTEGER, " \
												"FOREIGN KEY(TuId) REFERENCES TU(Id));";
const char* CTradosStorage::m_szTableNamePairs = "Pairs";
