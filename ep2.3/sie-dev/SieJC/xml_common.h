#ifndef _XML_COMMON_
#define _XML_COMMON_

typedef struct XMLAttr
{
	void* next;
	char* name;
	char* param;
} XMLAttr;


typedef struct XMLNode
{
	void* next;
	void* subnode;
	char* name;
	XMLAttr* attr;
	char* value;
} XMLNode;

#endif
