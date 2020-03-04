#include <swilib.h>
#include <alib/ui.h>

extern "C" void __attribute__((constructor)) __constr_ui_so__()
{
    //ShowMSG(1, (int)"oO_constr_UI");
    init_ui ();

}


extern "C" void __attribute__((destructor)) __destr_ui_so__()
{
    ShowMSG(1, (int)"ui_destr");

}
