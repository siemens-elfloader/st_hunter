#include <swilib.h>
#include "LangConfig.h"



int ParseToMssivInLang(char *buf, string lang[2][50])
{
	int c=0;
	char *s = buf;

	while(*s!='[')s++;

	while(*s)
	{
		if(!memcmp(s, "inputlang", sizeof("inputlang")-1))
		{
			while(*s!=']')s++;
			break;
		}
		s++;
	}

	do
	{
		if(*s=='<')
		{
			while(*s++)
			{
				if(!memcmp(s, "lang", sizeof("lang")-1))
				{
					char *e;
					while(*s++!='"');
					e=s;
					while(*s++!='"'); s--;

					lang[0][c].assign(e, s-e);

					while(*s++!='=');
					while(*s++!='"');
					e=s;
					while(*s++!='"');s--;

					lang[1][c].assign(e, s-e);

					c++;

                                        while(*s++!='>');
                                        break;
				}
			}

		}

	}while(*s++!='[' && *s);

        return c;

}


int ParseToMassivOutLang(char *buf, string lang[2][50])
{
	int c=0;
	char *s=buf;

	while(*s)
	{
		if(!memcmp(s, "outputlang", sizeof("outputlang")-1))
		{
			while(*s!=']')s++;
			break;
		}
		s++;
	}


	do
	{
		if(*s=='<')
		{
			while(*s++)
			{
				if(!memcmp(s, "lang", sizeof("lang")-1))
				{
					char *e;
					while(*s++!='"');
					e=s;
					while(*s++!='"'); s--;

					lang[0][c].assign(e, s-e);


					while(*s++!='=');
					while(*s++!='"');
					e=s;
					while(*s++!='"');s--;

					lang[1][c].assign(e, s-e);

					c++;
                                        while(*s!='>')s++;
                                        break;
				}
			}

		}


	}while(*s++!='[' && *s);

        return c;
}

