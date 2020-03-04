/*
 * JUDAS V2.0 DPMI memory locking functions. Includes also functions to
 * allocate/free locked memory. Feel free to use in your own proggys.
 */


int judas_memlock(void *start, unsigned size);
int judas_memunlock(void *start, unsigned size);
void *locked_malloc(int size);
void locked_free(void *address);

/*
 * We allocate 4 bytes more and store the block size before the actual block
 * passed to user. Afterwards, when freeing, we know the size (needed in
 * unlocking!)
 */
void *locked_malloc(int size)
{
        return malloc(size);
}

void locked_free(void *address)
{
if(address)mfree(address);
}

/*
int judas_memlock(void *start, unsigned size)
{
        union REGS glenregs;

        glenregs.w.ax = 0x600;
        glenregs.w.bx = (unsigned)start >> 16;
        glenregs.w.cx = (unsigned)start & 0xffff;
        glenregs.w.si = size >> 16;
        glenregs.w.di = size & 0xffff;
        int386(0x31, &glenregs, &glenregs);
        if (glenregs.w.cflag) return 0;
        return 1;
}

int judas_memunlock(void *start, unsigned size)
{
        union REGS glenregs;

        glenregs.w.ax = 0x601;
        glenregs.w.bx = (unsigned)start >> 16;
        glenregs.w.cx = (unsigned)start & 0xffff;
        glenregs.w.si = size >> 16;
        glenregs.w.di = size & 0xffff;
        int386(0x31, &glenregs, &glenregs);
        if (glenregs.w.cflag) return 0;
        return 1;
}
*/
