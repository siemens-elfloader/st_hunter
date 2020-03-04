
#include <swilib.h>
#include "FormBuilder/EForm.h"
#include "trafic.h"
#include "LangPack.h"


extern LangPack *lgp;
int input_traf=0, output_traf=0;
EForm *traf;
extern char temp[];


void TraficBuldForm(EForm *ef)
{
   sprintf(temp, "%s:", lgp->lang[Send].c_str());
   ef->AddFormElement(temp, EForm::NormalText, 256);
   sprintf(temp, "%d b", output_traf);
   ef->AddFormElement(temp, EForm :: ReadOnly, 2*1024);

   sprintf(temp, "%s:", lgp->lang[Received].c_str());
   ef->AddFormElement(temp, EForm::NormalText, 256);
   sprintf(temp, "%d b", input_traf);
   ef->AddFormElement(temp, EForm :: ReadOnly, 2*1024);

   sprintf(temp, "%s:", lgp->lang[AllTrafic].c_str());
   ef->AddFormElement(temp, EForm::NormalText, 256);
   sprintf(temp, "%d b", output_traf+input_traf);
   ef->AddFormElement(temp, EForm :: ReadOnly, 2*1024);
}

void closeTrafGUI()
{
   traf->close();
}

void AllocTrafic()
{
   extern char dir[];
   traf = new EForm ();
   sprintf(temp, "%sRes\\traf.PNG", dir);
   traf->SetHeader(temp, (int)lgp->lang[Trafic].c_str());
   traf->SetHookFormBuilder(TraficBuldForm);
   traf->ReallocEws(2*1024);
   traf->SetLeftSoft((int)lgp->lang[Back].c_str(), (void*)closeTrafGUI);
   traf->SetRightSoft((int)lgp->lang[Back].c_str());
}


void FreeTrafic()
{
   delete traf;
}
