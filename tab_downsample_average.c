#include "m_pd.h"  
 
#define MIN(a,b) (((a)<(b))?(a):(b))
#define MAX(a,b) (((a)>(b))?(a):(b))

static t_class *tab_downsample_average_class;  
 
typedef struct _tab_downsample_average {  
    t_object  x_obj;  
    t_symbol *x_arrayname_src;
    t_symbol *x_arrayname_dst;
    int factor;
    t_outlet *b_out;
} t_tab_downsample_average;  
 
void *tab_downsample_average_new(t_symbol *s_src,t_symbol *s_dst, t_float factor)  
{  
    t_tab_downsample_average *x = (t_tab_downsample_average *)pd_new(tab_downsample_average_class);  
 
    x->x_arrayname_src = s_src;
    x->x_arrayname_dst = s_dst;
    x->factor = MAX(1, factor);

    x->b_out = outlet_new(&x->x_obj, &s_bang);  

    return (void *)x;  
}  

void tab_downsample_average_bang(t_tab_downsample_average *x)  
{  
  
    t_garray *a_src, *a_dst;
    int npoints_src, npoints_dst;
    t_word *vec_src, *vec_dst;
        
    int nb_max, i, j, index_src;
    t_float tmp;
    
    if (!(a_src = (t_garray *)pd_findbyclass(x->x_arrayname_src, garray_class)))
        pd_error(x, "%s: no such array", x->x_arrayname_src->s_name);
    else if (!garray_getfloatwords(a_src, &npoints_src, &vec_src))
        pd_error(x, "%s: bad template for tab_downsample", x->x_arrayname_src->s_name);
    else if (!(a_dst = (t_garray *)pd_findbyclass(x->x_arrayname_dst, garray_class)))
        pd_error(x, "%s: no such array", x->x_arrayname_dst->s_name);
    else if (!garray_getfloatwords(a_dst, &npoints_dst, &vec_dst))
        pd_error(x, "%s: bad template for tab_downsample", x->x_arrayname_dst->s_name);
    else
    {
        nb_max = MIN(npoints_src/x->factor, npoints_dst);

        // post("nb_max = %d",nb_max);
        index_src = 0;
        for (i=0; i<nb_max;i++) {
            tmp=0;
            for (j=0; j<x->factor;j++) {
                tmp += vec_src[index_src].w_float;
                // post("read : %d, %f",index_src, vec_src[index_src].w_float);
                // post("tmp : %f",tmp);
                index_src++;
            }
            vec_dst[i].w_float = tmp / x->factor;
            // post("write : %d, %f",i, tmp);
        }
        garray_redraw(a_dst);
    }
    outlet_bang(x->b_out);  
}  
 
void tab_downsample_average_src(t_tab_downsample_average *x, t_symbol *s_src) {
    x->x_arrayname_src = s_src;
}

void tab_downsample_average_dst(t_tab_downsample_average *x, t_symbol *s_dst) {
    x->x_arrayname_dst = s_dst;
}

void tab_downsample_average_factor(t_tab_downsample_average *x, t_float factor) {
    x->factor = MAX(1, factor);    
}
 
void tab_downsample_average_setup(void) {  
  tab_downsample_average_class = class_new(gensym("tab_downsample_average"), (t_newmethod)tab_downsample_average_new,  
        0, sizeof(t_tab_downsample_average), CLASS_DEFAULT, A_DEFSYM, A_DEFSYM, A_DEFFLOAT, 0);  
  class_addbang(tab_downsample_average_class, tab_downsample_average_bang);  
  class_addmethod(tab_downsample_average_class, (t_method)tab_downsample_average_src, gensym("src"),  A_DEFSYM, 0);  
  class_addmethod(tab_downsample_average_class, (t_method)tab_downsample_average_dst, gensym("dst"),  A_DEFSYM, 0);  
  class_addmethod(tab_downsample_average_class, (t_method)tab_downsample_average_factor, gensym("factor"),  A_DEFFLOAT, 0);  
} 

