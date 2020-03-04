
#define TESTLIB_NAME    "TestLib"
#define TESTLIB_VERSION 0x0001
#define TESTLIB_FIRST   &testlib


typedef struct {
void (*message0)();
void (*message1)();
void (*message2)();
}TESTLIB_FUNC;

void message0(){} // "Show Message 1"
void message1(){} // "Show Message 2"
void message2(){} // "Show Message 3"

TESTLIB_FUNC testlib =
{
 message0,
 message1,
 message2
};

/* ----------------- Description  ----------------- */

/*

void message0(); // "Show Message 1"
void message1(); // "Show Message 2"
void message2(); // "Show Message 3"

*/







