#ifndef __TINYXML_TEST_H
#define __TINYXML_TEST_H

#include "test.h"
#include <iostream>
#include "open_source/TinyXml/src/tinyxml.h"

class TinyXml_Test : public CTest
{
public:
	void test(int argc, char ** argv)
	{
		TiXmlDocument doc;
		doc.LoadFile("db_log_app_config.xml");
		TiXmlNode* rootNode = doc.FirstChild("root");
		if (rootNode != NULL)
		{
			TiXmlNode * childNode = rootNode->FirstChild("db");
			if (childNode != NULL)
			{
				TiXmlElement * firstElement = childNode->FirstChildElement();
				if (firstElement != NULL)
				{
					std::cout << firstElement->GetText() << std::endl;	//mysql://DBTwoHostIp:DBTwoHostPort/DBLog
					TiXmlElement * element = firstElement->NextSiblingElement("url");
					if (element != NULL)
					{

						std::cout << element->GetText() << std::endl;	//mysql ://DBTwoHostIp:DBTwoHostPort/DBLog2222
					}
				}
			}
		}

		doc.SaveFile("db_log_app_config2.xml");
	}
};


#endif