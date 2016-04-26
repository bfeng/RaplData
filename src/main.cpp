#include <iostream>
#include"RaplData.h"

using namespace POWERAPI;

int main()
{
	char rapl_domain_names[4][16] =
	{
		"energy-cores",
		"energy-gpu",
		"energy-pkg",
		"energy-ram",
	};
	RaplData *d;
	for(int i = 0; i < 4; ++i)
	{
		d = new RaplData(rapl_domain_names[i]);
		d->Init();
		for(int j = 0; j < 10; ++j)
		{
			if(d->Open() == 0)
			{
				sleep(2);
				d->Read();
				d->Println();
			}
		}
		delete d;
	}

	return 0;
}
