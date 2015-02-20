//#include <boost/locale.hpp>
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/xml_parser.hpp>
#include <boost/date_time.hpp>
#include <boost/foreach.hpp>
#include <string>
#include <set>
#include <exception>

#include <clocale>
#include <iostream>
#include <fstream>
#include <string>
#include <locale>
#include <codecvt>
#include <fcntl.h>
#include <io.h>

#include <conio.h>

void load(const std::string &filename)
{

	// Create empty property tree object
	using boost::property_tree::wptree;
	wptree wpt;

	//boost::locale::generator gen;
	//std::locale loc;// = gen.generate("en_US.UTF-8");

	// Load XML file and put its contents in property tree. 
	// No namespace qualification is needed, because of Koenig 
	// lookup on the second argument. If reading fails, exception
	// is thrown.
	std::wifstream wifs;
	read_xml(filename, wpt, 0, std::locale(wifs.getloc(), new std::codecvt_utf8<wchar_t, 0x10ffff, std::consume_header>()));

	BOOST_FOREACH(const wptree::value_type& node, wpt.get_child(L"tmx.body"))
	{
		if (node.first == L"tu")
		{
			std::wstring creationdate = node.second.get<std::wstring>(L"<xmlattr>.creationdate");
			std::wstring creationid = node.second.get<std::wstring>(L"<xmlattr>.creationid");

			std::wcout << L" creationdate:  " << creationdate << std::endl;
			std::wcout << L" creationid:  " << creationid << std::endl;

			BOOST_FOREACH(const wptree::value_type& tuv, node.second.get_child(L""))
			{
				if (tuv.first == L"tuv")
				{
					std::wstring xmllang = tuv.second.get<std::wstring>(L"<xmlattr>.xml:lang");
					std::wcout << L"\t xmllang: " << xmllang;
					std::wstring m_seg = tuv.second.get<std::wstring>(L"seg");
					std::wcout << L", seg: \"" << m_seg << L"\"" << std::endl;
				}
			}

			std::wcout << std::endl;
		}
	}
}


void read_all_lines(const char *filename)
{
	std::wifstream wifs;
	std::wstring txtline;
	int c = 0;

	wifs.open(filename);
	if (!wifs.is_open())
	{
		std::wcerr << L"Unable to open file" << std::endl;
		return;
	}
	// We are going to read an UTF-8 file
	wifs.imbue(std::locale(wifs.getloc(), new std::codecvt_utf8<wchar_t, 0x10ffff, std::consume_header>()));
	while (getline(wifs, txtline))
		std::wcout << ++c << L'\t' << txtline << L'\n';
	std::wcout << std::endl;
}


int runTest()
{
	std::string filename = "trados.xml";

	// Console output will be UTF-16 characters
	//_setmode(_fileno(stdout), _O_U16TEXT);
	read_all_lines(filename.c_str());

	return 0;
}

void runParser(const std::string& filename)
{
	try
	{
		boost::posix_time::ptime t1 = boost::posix_time::microsec_clock::local_time();

		load(filename);

		boost::posix_time::ptime t2 = boost::posix_time::microsec_clock::local_time();
		boost::posix_time::time_duration diff = t2 - t1;
		std::wcout << L"Success! Run time (in sec): " << diff.total_milliseconds() / 1000.0 << std::endl;
	}
	catch (std::exception &e)
	{
		std::wcerr << L"Error: " << e.what() << "\n";
	}
}

int main(int argc, char* argv[])
{
	_setmode(_fileno(stdout), _O_U16TEXT);

	if (argc == 1)
	{
		std::wcerr << L"Use: BoostXML.exe <filename.xml>" << std::endl;
		return -1;
	}

	std::string filename(argv[1]);

	//std::setlocale(LC_ALL, "uk_UA.UTF-8");

	//std::wcout << L"Запуск тестового читання файла..." << std::endl;
	//runTest();

	std::wcout << L"Запуск тестового парсера..." << std::endl;
	runParser(filename);

	_getch();

	return 0;
}
