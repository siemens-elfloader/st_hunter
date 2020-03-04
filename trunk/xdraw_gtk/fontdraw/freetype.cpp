#include <alib/freetype.h>


ft_font *ft_open(const char *font, uint16_t size)
{
    ft_font *ftf = (ft_font *)malloc(sizeof(ft_font));
    if( !ftf ) return 0;

    ftf->fte = fte_open(font);
    if(!ftf->fte){
        goto release;
    }

    ftf->fti = fte_open_cache_by_metrics(ftf->fte, size);
    if(!ftf->fti)
    {
        fte_close(ftf->fte, 0);
        goto release;
    }

    ftf->fti->height=ftf->fte->face->height*size/ftf->fte->face->units_per_EM;
    ftf->fti->ascender=ftf->fte->face->ascender*size/ftf->fte->face->units_per_EM;

    fte_set_load_flags (ftf->fte, FT_LOAD_TARGET_LIGHT);
    fte_set_render_flags (ftf->fte, FT_RENDER_MODE_LIGHT);

    return ftf;
release:
    free(ftf);

    return 0;
}


void ft_close(ft_font *f)
{
    if(!f) return;
    if(!f->fte) return;
    if(!f->fti) return;

    fte_close_cache_metrics(f->fti);
    fte_close(f->fte, 0);
    free(f);
}


