#if defined( _MSC_VER )
#if !defined( _CRT_SECURE_NO_WARNINGS )
#define _CRT_SECURE_NO_WARNINGS		// This test file is not intended to be secure.
#endif
#endif

#include "tinyxml2/tinyxml2.h"
#include <iostream>


#include <cstdlib>
#include <cstring>
#include <ctime>

#if defined( _MSC_VER )
#include <direct.h>		// _mkdir
#include <crtdbg.h>
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
_CrtMemState startMemState;
_CrtMemState endMemState;
#elif defined(MINGW32) || defined(__MINGW32__)
#include <io.h>  // mkdir
#else
#include <sys/stat.h>	// mkdir
#endif

using namespace tinyxml2;

#pragma  comment(lib, "tinyxml2")

int gPass = 0;
int gFail = 0;

bool XMLTest(const char* testString, const char* expected, const char* found, bool echo=true, bool extraNL=false)
{
	bool pass;
	if (!expected && !found)
		pass = true;
	else if (!expected || !found)
		pass = false;
	else
		pass = !strcmp(expected, found);
	if (pass)
		printf("[pass]");
	else
		printf("[fail]");

	if (!echo) {
		printf(" %s\n", testString);
	}
	else {
		if (extraNL) {
			printf(" %s\n", testString);
			printf("%s\n", expected);
			printf("%s\n", found);
		}
		else {
			printf(" %s [%s][%s]\n", testString, expected, found);
		}
	}

	if (pass)
		++gPass;
	else
		++gFail;
	return pass;
}


template< class T > bool XMLTest(const char* testString, T expected, T found, bool echo=true)
{
	bool pass = (expected == found);
	if (pass)
		printf("[pass]");
	else
		printf("[fail]");

	if (!echo)
		printf(" %s\n", testString);
	else
		printf(" %s [%d][%d]\n", testString, static_cast<int>(expected), static_cast<int>(found));

	if (pass)
		++gPass;
	else
		++gFail;
	return pass;
}


void NullLineEndings(char* p)
{
	while (p && *p) {
		if (*p == '\n' || *p == '\r') {
			*p = 0;
			return;
		}
		++p;
	}
}

int main()
{
	XMLDocument* doc = new XMLDocument();
	doc->LoadFile("test.xml");
	XMLElement *element = doc->FirstChildElement("document")->FirstChildElement("English");
	XMLText *text = element->FirstChild()->ToText();
	element->SetAttribute("name", "123");
	std::cout << element->Attribute("name") << std::endl;
	std::cout << text->Value() << std::endl;

	XMLElement* root = doc->FirstChildElement();
	XMLElement* newElement = doc->NewElement("Subelement");
	root->InsertEndChild(newElement);

	XMLElement* sub[3] ={ doc->NewElement("sub"), doc->NewElement("sub"), doc->NewElement("sub") };
	for (int i=0; i < 3; ++i) {
		sub[i]->SetAttribute("attrib", i);
		root->InsertEndChild(sub[i]);
	}
	int ival;
	int res = root->QueryIntAttribute("attrib", &ival);
	XMLTest("Query attribute: does not exist", res, (int)XML_NO_ATTRIBUTE);

	root->LastChildElement()->SetAttribute("attrib", 5);
	root->LastChildElement()->DeleteAttribute("attrib");

	const char* xmlAfterTwo = "<?xml version=\"1.0\" ?>"
		"<root>"
		"<one/>"
		"<two/>"
		"<subtree>"
		"<elem>element 1</elem>text<!-- comment -->"
		"</subtree>"
		"</root>";

	XMLElement* subtree = doc->RootElement()->FirstChildElement("sub");
	XMLElement* two = doc->RootElement()->FirstChildElement("sub");
	doc->RootElement()->InsertAfterChild(two, subtree);
	XMLPrinter printer2(0, true);
	doc->Accept(&printer2);
	XMLTest("Move node from within <one> after <two>", xmlAfterTwo, printer2.CStr(), false);
	
	doc->SaveFile("test1.xml");

	std::cin.get();

	return doc->ErrorID();
}