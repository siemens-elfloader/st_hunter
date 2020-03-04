#include <alib/io.h>
#include <alib/io_ui.h>

///////////Simple Parser////////////
int
SimpleParser::Open (char *fname){
    count_=GetFileSize(fname);
    if (count_>0){
        buf_=new  char [count_];
        FileReadToBuffer (fname, buf_, count_);

        return count_;
    }
    else return -1;
}

void
SimpleParser::Close (){
    if (buf_) delete [] buf_;
}

int
SimpleParser::GoToSymb (char s){
    while (cur_<count_){
        if (buf_[cur_]!=s) cur_++;
        else return ++cur_;
    }
    return -1;
}

int
SimpleParser::CopyToSymb ( char *str,  char s){
    int i=0;
    while (cur_<count_){
        if (buf_[cur_]!=s){ str[i]=buf_[cur_]; cur_++; i++;}
        else{ str[i]='\0'; return ++cur_;}
    }
    str[i]='\0';
    return -1;
}


int str2int (char *str){
  int n = 0;
  sscanf(str, "%d", &n);
  return(n);
}

int GetFileDir (char *fname, char *buf){
	int ii;
	int len = strlen(fname);

	for(ii = len-2; ii > 0; ii--)
		if (fname[ii] == '\\' || fname[ii] == '/') break;
	len = ii;

	if (buf)
	{
		for(ii=0; ii<len; ii++) buf[ii] = fname[ii];
		buf[len] = 0;
	}
	return len;
}

#ifndef WIN
void RunAction (char *s)
{
  if((s)&&strlen(s))
  {
    if (s[0]=='M'){
      //WSHDR  *ws=AllocWS(128);
      int len=strlen(s);
      char *m=new char [len-1];
      for (int i=0; i+1<len; i++) m[i]=s[i+1];
      m[len-1]=0;
      //str_2ws(ws, wayToMC, 128);

      //ExecuteFile(ws, 0, m);
      //FreeWS(ws);
      //OpenInMC (m);
      mfree(m);
    }
    else{
      if (s[0]=='J'){
        int len=strlen(s);
        //LogInt (len+100);
        char *m=new char (len-1);
        for (int i=0; i+1<len; i++) m[i]=s[i+1];
        //m[len-1]='\0';
        //Log (m);
        int mID=str2int (m);
        //LogInt (mID);
        //SetCurPosition (mID);
        mfree(m);
      }
      else{

      if ((s[1]==':')&&(s[2]=='\\')){
        WSHDR  *ws=AllocWS(128);
        str_2ws(ws,s,strlen(s)+1);
        ExecuteFile(ws,0,NULL);
        FreeWS(ws);
   }else
   {
    if ((s[0]!='a')&&(s[0]!='A')&&(s[1]!='0'))
    {unsigned int* addr = (unsigned int*)GetFunctionPointer(s);
      if (addr){
        typedef void (*voidfunc)();
        //voidfunc pp=(voidfunc)*(addr+4);
#ifdef NEWSGOLD
        voidfunc pp=(voidfunc)*(addr+4);
#else
        voidfunc pp=(voidfunc)addr;
#endif
        SUBPROC((void*)pp);
      }
    }

    if (((s[0]=='a')||(s[0]=='A'))&&(s[1]=='0'))
    { int entry; sscanf(s,"%08X",&entry); SUBPROC((void*)entry);}
   }
  }
}
  }
}

#endif
