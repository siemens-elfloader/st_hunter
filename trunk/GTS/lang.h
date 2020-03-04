
#include <string>
using namespace std;

extern "C" LForm *input_lang, *output_lang;
extern "C" int inputlang, outputlang;
extern "C" string lang[2][50];
extern "C" string inlang[2][50];

void Start();
void Close();


int ShowMenuInput();
int ShowMenuOutput();
