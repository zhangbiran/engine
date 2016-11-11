#include "config/config.h"
#include <fstream>

CConfig * CConfig::instance()
{
	static CConfig obj;
	return &obj;
}

int CConfig::getTestObjectType()
{
	std::ifstream ifs;
	ifs.open("./config.txt", std::ostream::in);
	if (!ifs.is_open())
	{
		return -1;
	}

	char buf[10] = { 0 };
	ifs.read(buf, 10);
	ifs.close();

	return atoi(buf);

}