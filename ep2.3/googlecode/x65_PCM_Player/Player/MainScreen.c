
unsigned char 
PosX=2,
PosY=39,
PosW=126,
PosH=5,
SliderW=14,
SliderH=5;

void DrawMainScreen(){
 char temptime[5];
 _rgb pen={0,0,0,255};
 _rgb redpen={255,0,0,255};
 _rgb brush={0,0,0,0};
 
  WSHDR * ws = AllocWS( 256 );
 
  DrawBitmap( 0, 0, 132, 176, ( short * ) MainFrame );
  if((playlist[plcurrent].songtime>0)&&(playlist[plcurrent].songtime>=(GetPlayingTime()*50))){
  unsigned char temppos=PosX+(PosW-SliderW)*GetPlayingTime()/(playlist[plcurrent].songtime/50);  
  DrawBitmap( temppos, PosY, SliderW, SliderH, ( short * ) PosSlider );  
  }
  
  DrawScreen();
  
    unsigned char min = GetPlayingTime() / 60;
    unsigned char sec = GetPlayingTime() - min * 60;
    if ( min > 9 )
    {
      if ( sec > 9 )
      {
        sprintf( temptime, "%d:%d", min, sec );
      }
      else
      {
        sprintf( temptime, "%d:0%d", min, sec );
      }
    }
    else
    {
      if ( sec > 9 )
      {
        sprintf( temptime, "0%d:%d", min, sec );
      }
      else
      {
        sprintf( temptime, "0%d:0%d", min, sec );
      }
    }
    
    wsprintf( ws,temptime);
    DrawString( ws, 3, 17, 45, 35, 0, 2, ( char * ) & pen, ( char * ) & brush );
    
        str_2ws(ws,playlist[plcurrent].filename2,strlen(playlist[plcurrent].filename2));
        DrawString( ws, 3, 4, 130, 14, 7, 3, ( char * ) & pen, ( char * ) & brush );
        
        if(channels==1)
        wsprintf(ws,"Bitrate %d kb/s %dHz\nVolume %d mono",bitrate/1000,samplerate,Volume);
        else
        wsprintf(ws,"Bitrate %d kb/s %dHz\nVolume %d stereo",bitrate/1000,samplerate,Volume);
        
        if(Volume<=22) 
        DrawString( ws, 3, 4+48, 130, 14+62, 7, 3, ( char * ) & pen, ( char * ) & brush );
        else
        DrawString( ws, 3, 4+48, 130, 14+62, 7, 3, ( char * ) & redpen, ( char * ) & brush );
        
#ifdef _profile
wsprintf(ws,"d %d %d %d",GetProfilePercent(0),
         GetProfilePercent(1),GetProfilePercent(2));
DrawString( ws, 2, 100, 130, 176, 7, 3, ( char * ) & pen, ( char * ) & brush );        
#endif
    
    FreeWS( ws );
  
}
