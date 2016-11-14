#ifndef __JSON_TEST_H
#define __JSON_TEST_H
#include "test.h"
#include "open_source/Json/src/json.h"
#include <fstream>
class Json_Test : public CTest
{
public:
	void test(int argc, char ** argv)
	{
		Json::Value json;
		Json::Reader reader;
		std::ifstream ifs("./t_question.json");
		if (!ifs.is_open())
		{
			std::cout << "file not open" << std::endl;
		}
		reader.parse(ifs, json);
		for (Json::Value::UInt i = 0; i < json.size(); i++)
		{
			std::cout << "answer: " << json[Json::Value::UInt(i)]["answer"].asString() << std::endl;
			std::cout << "option1: " << json[Json::Value::UInt(i)]["option1"].asString() << std::endl;
			std::cout << "option2: " << json[Json::Value::UInt(i)]["option2"].asString() << std::endl;
			std::cout << "question: " << json[Json::Value::UInt(i)]["question"].asString() << std::endl;
			std::cout << "questionId: " << json[Json::Value::UInt(i)]["questionId"].asInt() << std::endl;
			std::cout << "type: " << json[Json::Value::UInt(i)]["type"].asInt() << std::endl;
			std::cout << std::endl;
		}

		ifs.close();
	}
};


#endif