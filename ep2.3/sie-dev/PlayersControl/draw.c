void InitTags()
{
  if(player0&&IsPlayerOn()){}
  if(player1&&IsIvoOn)strcpy(Name,IvoTag);
  if(player2&&IsMaplayerOn)strcpy(Name,Info->trackName);
  if(player3&&IsSPlayerOn){}
  wsprintf(ws,Name);
}

void DrawTags()
{
  ws=AllocWS(256);
  if(TAGSType!=2)
  {
    if(TAGSType==1||(TAGSType==0&&ShowInfo))
    InitTags();
    DrawString(ws,TAGSPOS.x,TAGSPOS.y,TAGSPOS.x2,TAGSPOS.y2,FONT,TEXT_ALIGNMIDDLE,COLOR,NULL);
  }
  FreeWS(ws);
  if(TAGSType==0)
  {
    if(ShowInfo&&ShowTimer<15)
    ShowTimer++;
    else {ShowTimer=0;ShowInfo=0;}
  }
  GBS_StartTimerProc(&RedrawTimer,20,DrawTags);
}
