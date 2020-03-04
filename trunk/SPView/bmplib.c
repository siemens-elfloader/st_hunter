#include <swilib.h>
#include "bmplib.h"

static unsigned int ExtraBytes(const int bit_count, const int width, const unsigned int extrabytes);

int SaveBMP(const char *fname, char *bitmap_in, const int width, const int height, const unsigned int bit_count,
                        const unsigned int compression)
{
    if(bitmap_in==NULL || !width || !height || bit_count>32)
        return -1;

    unsigned int err;
    int fp=_open(fname, A_ReadWrite+A_Create, P_WRITE, &err);

    if(fp==-1) return -1;

    unsigned long bitsize=width*height*bit_count/8;
    const unsigned int headsize=sizeof(BITMAPFILEHEADER);
    const unsigned int infosize=sizeof(BITMAPINFOHEADER);
    const char palette[]={0x00, 0xf8, 0x00, 0x00, 0xe0, 0x07, 0x00, 0x00, 0x1F, 0x00, 0x00, 0x00};

    int palette_size=(compression==BI_BITFIELDS) ? 12 : 0;
    //head
    BITMAPFILEHEADER header;
    zeromem(&header, headsize);
    header.bfType=BF_TYPE;
    //header.bfReserved1=0;
    //header.bfReserved2=0;
    header.bfOffBits=headsize+infosize+palette_size;
    header.bfSize=header.bfOffBits+bitsize;

    //headinfo
    BITMAPINFOHEADER info;
    zeromem(&info, infosize);
    info.biSize=infosize;
    info.biWidth=width;
    info.biHeight=-height;
    info.biPlanes=1;
    info.biBitCount=bit_count;
    info.biCompression=compression;
    /*info.biSizeImage=0;//if bi.compression==0
    info.biXPelsPerMeter=0xB40;
    info.biYPelsPerMeter=0xB40;
    info.biClrUsed=0;
    info.biClrImportant=0;*/

    char *bitmap=NULL;

    if(ExtraBytes(bit_count, width, 0)) //кратность длины параграфа, для 32 bits не актуально
    {
        unsigned int extrabytes=0; //количество добавочных байт
        while(1)
        {
            extrabytes++;
            if(!ExtraBytes(bit_count, width, extrabytes)) break;
        }
        bitsize+=height*extrabytes;
        bitmap=malloc(bitsize+1);
        int i=0;
        unsigned int psize=bit_count/8;
        while(i<height)
        {
            memcpy(bitmap+width*psize*i+i*extrabytes, bitmap_in+width*psize*i, width*psize);
            zeromem(bitmap+width*psize*(i+1)+i*extrabytes, extrabytes);
            i++;
        }
    };
    _write(fp, &header, headsize, &err);
    _write(fp, &info, infosize, &err);
    if(palette_size) _write(fp, palette, palette_size, &err);
    if(bitmap==NULL)
        _write(fp, bitmap_in, bitsize, &err);
    else
    {
        _write(fp, bitmap, bitsize, &err);
        mfree(bitmap);
    }
    _close(fp, &err);
    return 1;
}

static unsigned int ExtraBytes(const int bit_count,const int width, const unsigned int extrabytes)
{
    unsigned int factor;
    switch(bit_count)
    {
        case 8:
            factor=3;
        break;
        case 16:
            factor=2;
        break;
        default:
            factor=0;
    }
    return ((width*factor+extrabytes)%4);
}

char *SieRastrDecoder(const char *bitmap_in, const int permission, const int bit_count)
{
    char *bitmap=NULL;
    if(bitmap_in!=NULL)
    {
        unsigned int rlen=0, wlen=0, offset=0;
        const int psize=bit_count/8; //байт на пиксель
        const unsigned int mlen=permission*psize;
        bitmap=malloc(mlen+1);
        while(wlen<mlen)
        {
            if(bitmap_in[rlen]<=0x7F)             //0x7F  (0xFF,0xFF) число повторений следующих psize
            {
                for(int i=0; i<bitmap_in[rlen]; i++, wlen+=psize)
                {
                    //bitmap=realloc(bitmap, wlen+3); //на глаз заметно, что скорость обработки уменьшается
                    memcpy(bitmap+wlen, bitmap_in+rlen+1, psize);
                }
                rlen+=psize+1;
            }
            else                                            //0x80 0xFF,0xFF,0xFF...количество несжатых последующих байт
            {
                switch(bit_count)
                {
                    case 8: case 32:
                        offset=(0xFF-bitmap_in[rlen]+0x01)*psize; break;
                    case 16:
                        offset=(0xFF-bitmap_in[rlen])*psize+0x02;
                    break;
                }
                //bitmap=realloc(bitmap, wlen+offset+1); //на глаз заметно, что скорость обработки уменьшается
                memcpy(bitmap+wlen, bitmap_in+rlen+1, offset);
                wlen+=offset;
                rlen+=offset+1;
            }
        }
        /*char s[128];
        sprintf(s, "rlen: %d, wlen: %d\nmaxlen: %d", rlen, wlen, mlen);
        ShowMSG(1, (int)s);*/
    }
    return bitmap;
}
