#include <boost/date_time.hpp>

#include <xercesc/util/PlatformUtils.hpp>
#include <xercesc/util/TransService.hpp>
#include <xercesc/sax2/SAX2XMLReader.hpp>
#include <xercesc/sax2/XMLReaderFactory.hpp>
#include <xercesc/util/OutOfMemoryException.hpp>

#include <conio.h>

#include <iostream>
#include <fstream>
#include <string>
#include <locale>
#include <codecvt>
#include <fcntl.h>
#include <io.h>

#include <windows.h>
#include <Stringapiset.h>

#include "TradosSAX2Handler.h"

XERCES_CPP_NAMESPACE_USE

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

int runParser(const std::string& filename)
{
	// Initialize the XML4C2 system
	try
	{
		XMLPlatformUtils::Initialize();
	}

	catch (const XMLException& toCatch)
	{
		XERCES_STD_QUALIFIER cerr << "Error during initialization! :\n"
			<< toCatch.getMessage() << XERCES_STD_QUALIFIER endl;
		return 1;
	}

	//
	//  Create a SAX parser object. Then, according to what we were told on
	//  the command line, set it to validate or not.
	//
	SAX2XMLReader* parser = XMLReaderFactory::createXMLReader();
	SAX2XMLReader* filter = NULL;


	//
	//  Create the handler object and install it as the document and error
	//  handler for the parser. Then parse the file and catch any exceptions
	//  that propogate out
	//

	int errorCount = 0;
	int errorCode = 0;
	try
	{
		TradosSAX2Handler handler;

		parser->setFeature(XMLUni::fgXercesSchemaFullChecking, true);
		parser->setFeature(XMLUni::fgSAX2CoreValidation, true);
		parser->setFeature(XMLUni::fgXercesDynamic, false);

		parser->setContentHandler(&handler);
		parser->setErrorHandler(&handler);
		parser->parse(filename.c_str());
		errorCount = parser->getErrorCount();
	}
	catch (std::exception &e)
	{
		std::wcerr << L"Error: " << e.what() << "\n";
	}
	catch (const OutOfMemoryException&)
	{
		XERCES_STD_QUALIFIER cerr << "OutOfMemoryException" << XERCES_STD_QUALIFIER endl;
		errorCode = 5;
	}
	catch (const XMLException& toCatch)
	{
		XERCES_STD_QUALIFIER cerr << "\nAn error occurred\n  Error: "
			<< toCatch.getMessage()
			<< "\n" << XERCES_STD_QUALIFIER endl;
		errorCode = 4;
	}

	if (errorCode) {
		XMLPlatformUtils::Terminate();
		return errorCode;
	}

	//
	//  Delete the parser itself.  Must be done prior to calling Terminate, below.
	//
	delete parser;

	// And call the termination method
	XMLPlatformUtils::Terminate();

	if (errorCount > 0)
		return 4;
	else
		return 0;
}

int main(int argc, char* argv[])
{
	// Console output will be UTF-16 characters
	//_setmode(_fileno(stdout), _O_U16TEXT);
	_setmode(_fileno(stdout), _O_WTEXT);

	if (argc == 1)
	{
		std::wcerr << L"Use: TradosConverter.exe <filename>" << std::endl;
		return -1;
	}

	std::string filename(argv[1]);

	//runTest();

	boost::posix_time::ptime t1 = boost::posix_time::microsec_clock::local_time();

	boost::posix_time::ptime todayUtc(boost::gregorian::day_clock::local_day(), boost::posix_time::second_clock::local_time().time_of_day());
	std::wcout << boost::posix_time::to_simple_wstring(todayUtc) << std::endl;

	runParser(filename);

	boost::posix_time::ptime t2 = boost::posix_time::microsec_clock::local_time();
	boost::posix_time::time_duration diff = t2 - t1;
	std::wcout << L"Success! Run time (in sec): " << diff.total_milliseconds() / 1000.0 << std::endl;

	_getch();
}

