
#include "zlib.h"

typedef struct
{
  uLong signature; //bytes  (0x04034b50)
  unsigned short version;
  unsigned short gpbf;
  unsigned short copmmet;
  unsigned short lmfn;
  unsigned short lmfd;
  uLong crc32;
  uLong compsize;
  uLong uncompsize;
  unsigned short fnamelen;
  unsigned short extralen;
}
_ziphdr;


/*unzip (c)bn cbn@yandex.ru */
int unzip( Byte * compr, uLong comprLen, Byte * uncompr, uLong uncomprLen )
{
  int err;
  z_stream d_stream;
  d_stream.zalloc = ( alloc_func )0;
  d_stream.zfree = ( free_func )0;
  d_stream.opaque = ( voidpf )0;
  d_stream.next_in = compr;
  d_stream.avail_in = ( uInt )comprLen;
  err = inflateInit2( & d_stream, -MAX_WBITS );
  if ( err != Z_OK )
  {
  unerr:
    return err;
  }
  d_stream.next_out = uncompr;
  d_stream.avail_out = ( uInt )uncomprLen;
  err = inflate( & d_stream, 2 );
  if ( err < 0 ) goto unerr;
  err = inflateEnd( & d_stream );
  if ( err < 0 ) goto unerr;
  return 0;
}

unsigned int UnZipByFileName( char * zipfilename, char * unzfilename, char **buffer, uLong uncomprLen )
{
  int zip, err;
  _ziphdr ziphdr;
  char * compbuff;
  char *nameforcmp = malloc( 256 );;
  char * filename = malloc( 256 );
  strcpy(nameforcmp,unzfilename);
  uppercase( nameforcmp );
  zip = fopen( zipfilename, A_ReadOnly + A_BIN, P_READ, & err );
  if(zip==-1)return(0);
  zeromem( & ziphdr, sizeof( ziphdr ) );
  while ( fread( zip, & ziphdr, 14, & err ) == 14 )
  {   
    if(ziphdr.signature!=0x04034b50)break;
    fread( zip, & ziphdr.crc32, 16, & err );    
    fread( zip, filename, ziphdr.fnamelen, & err );
    //fread( zip, compbuff, ziphdr.extralen, & err );    
    filename[ziphdr.fnamelen] = 0;
    uppercase( filename );
    if(!strcmp(filename,nameforcmp)){
    compbuff = malloc( ziphdr.compsize );
    fread32( zip, compbuff, ziphdr.compsize, & err );  
    *buffer=malloc(ziphdr.uncompsize);
    unzip( compbuff, ziphdr.compsize, *buffer, uncomprLen);
    mfree( compbuff );
    mfree(nameforcmp);
    mfree( filename );
    fclose( zip, & err );    
    return(ziphdr.uncompsize);   
    }
    lseek(zip,ziphdr.extralen+ziphdr.compsize,S_CUR,&err,&err); 
    
  }

  mfree(nameforcmp);
  mfree( filename );
  fclose( zip, & err );
  return(0);
}


