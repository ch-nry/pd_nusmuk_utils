#include "m_pd.h"  
 
#define MIN(a,b) (((a)<(b))?(a):(b))
#define MAX(a,b) (((a)>(b))?(a):(b))

static t_class *tab_upsample_copy_class;  
 
typedef struct _tab_upsample_copy {  
    t_object  x_obj;  
    t_symbol *x_arrayname_src;
    t_symbol *x_arrayname_dst;
    int factor, offset;
    t_outlet *b_out;
} t_tab_upsample_copy;  
 
void *tab_upsample_copy_new(t_symbol *s_src,t_symbol *s_dst, t_float factor)  
{  
    t_tab_upsample_copy *x = (t_tab_upsample_copy *)pd_new(tab_upsample_copy_class);  
 
    x->x_arrayname_src = s_src;
    x->x_arrayname_dst = s_dst;
    x->factor = MAX(1, factor);
    x->offset = 0;
    
    x->b_out = outlet_new(&x->x_obj, &s_bang);  

    return (void *)x;  
}  

void tab_upsample_copy_compute(t_tab_upsample_copy *x)  
{  
  
    t_garray *a_src, *a_dst;
    int npoints_src, npoints_dst;
    t_word *vec_src, *vec_dst;
        
    int nb_max, i, j;
        
    if (!(a_src = (t_garray *)pd_findbyclass(x->x_arrayname_src, garray_class)))
        pd_error(x, "%s: no such array", x->x_arrayname_src->s_name);
    else if (!garray_getfloatwords(a_src, &npoints_src, &vec_src))
        pd_error(x, "%s: bad template for tab_upsample_copy", x->x_arrayname_src->s_name);
    else if (!(a_dst = (t_garray *)pd_findbyclass(x->x_arrayname_dst, garray_class)))
        pd_error(x, "%s: no such array", x->x_arrayname_dst->s_name);
    else if (!garray_getfloatwords(a_dst, &npoints_dst, &vec_dst))
        pd_error(x, "%s: bad template for tab_upsample_copy", x->x_arrayname_dst->s_name);
    else
    {
        nb_max = MIN(npoints_src*x->factor, npoints_dst-x->offset);
        
        // post("nb_max = %d",nb_max);
        for (i=0; i<nb_max;i++) {
            vec_dst[i+x->offset].w_float = vec_src[i/x->factor].w_float;
        }
        garray_redraw(a_dst);
    }
    outlet_bang(x->b_out);  
}  
 
 void tab_upsample_copy_bang(t_tab_upsample_copy *x)  
{
    x->offset = 0;
    tab_upsample_copy_compute(x) ;
}

void tab_upsample_copy_float(t_tab_upsample_copy *x, t_float offset)  
{
    x->offset = offset;
    tab_upsample_copy_compute(x) ;
}

void tab_upsample_copy_src(t_tab_upsample_copy *x, t_symbol *s_src) {
    x->x_arrayname_src = s_src;
}

void tab_upsample_copy_dst(t_tab_upsample_copy *x, t_symbol *s_dst) {
    x->x_arrayname_dst = s_dst;
}

void tab_upsample_copy_factor(t_tab_upsample_copy *x, t_float factor) {
    x->factor = MAX(1, factor);
}
 
void tab_upsample_copy_setup(void) {  
  tab_upsample_copy_class = class_new(gensym("tab_upsample_copy"), (t_newmethod)tab_upsample_copy_new,  
        0, sizeof(t_tab_upsample_copy), CLASS_DEFAULT, A_DEFSYM, A_DEFSYM, A_DEFFLOAT, 0);  
  class_addbang(tab_upsample_copy_class, tab_upsample_copy_bang);  
  class_addmethod(tab_upsample_copy_class, (t_method)tab_upsample_copy_src, gensym("src"),  A_DEFSYM, 0);  
  class_addmethod(tab_upsample_copy_class, (t_method)tab_upsample_copy_dst, gensym("dst"),  A_DEFSYM, 0);  
  class_addmethod(tab_upsample_copy_class, (t_method)tab_upsample_copy_factor, gensym("factor"),  A_DEFFLOAT, 0);  
  class_addfloat(tab_upsample_copy_class, (t_method)tab_upsample_copy_float);  
} 

