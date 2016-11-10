#ifndef __CONFIG_H
#define  __CONFIG_H


class CConfig
{
public:
	static CConfig * instance();
	int getTestObjectType();

private:
	CConfig() {}
};

#endif