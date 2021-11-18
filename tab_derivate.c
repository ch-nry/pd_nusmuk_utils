#include "m_pd.h"  

#define MIN(a,b) (((a)<(b))?(a):(b))

static t_class *tab_derivate_class;  
 
typedef struct _tab_derivate {  
    t_object  x_obj;  
    t_symbol *x_arrayname_src1;
    t_symbol *x_arrayname_dst;
    t_outlet *b_out;
} t_tab_derivate;  
 
void *tab_derivate_new(t_symbol *s_src1, t_symbol *s_dst)  {  
    t_tab_derivate *x = (t_tab_derivate *)pd_new(tab_derivate_class);  
 
    x->x_arrayname_src1 = s_src1;
    x->x_arrayname_dst = s_dst;
    
    x->b_out = outlet_new(&x->x_obj, &s_bang);  

    return (void *)x;  
}  

void tab_derivate_compute(t_tab_derivate *x, t_float nb)  {  
    t_garray *a_src1, *a_dst;
    int npoints_src1, npoints_dst;
    t_word *vec_src1, *vec_dst;
    t_float tmp2, tmp1;
    
    int nb_derivate, i;
        
    if (!(a_src1 = (t_garray *)pd_findbyclass(x->x_arrayname_src1, garray_class)))
        pd_error(x, "%s: no such array", x->x_arrayname_src1->s_name);
    else if (!garray_getfloatwords(a_src1, &npoints_src1, &vec_src1))
        pd_error(x, "%s: bad template for tab_derivate", x->x_arrayname_src1->s_name);
    else if (!(a_dst = (t_garray *)pd_findbyclass(x->x_arrayname_dst, garray_class)))
        pd_error(x, "%s: no such array", x->x_arrayname_dst->s_name);
    else if (!garray_getfloatwords(a_dst, &npoints_dst, &vec_dst))
        pd_error(x, "%s: bad template for tab_derivate", x->x_arrayname_dst->s_name);
    else
    {
        nb_derivate = MIN(npoints_src1, npoints_dst);
        //nb_derivate = MAX(1, x->factor);

        // post("nb_derivate = %d",nb_derivate);

        if (nb > 0)
            nb_derivate = MIN(nb_derivate, nb);
            
        tmp1 = 0;
        for (i=0; i<nb_derivate;i++) {
            tmp2 = vec_src1[i].w_float;
            vec_dst[i].w_float = tmp2 - tmp1;
            tmp1 = tmp2;
        }
        garray_redraw(a_dst);
    }
    outlet_bang(x->b_out);  
}  
 
void tab_derivate_bang(t_tab_derivate *x)  {
    tab_derivate_compute(x, -1) ;
}

void tab_derivate_float(t_tab_derivate *x, t_float nb)  {
    tab_derivate_compute(x, nb) ;
}

void tab_derivate_src1(t_tab_derivate *x, t_symbol *s_src) {
    x->x_arrayname_src1 = s_src;
}

void tab_derivate_dst(t_tab_derivate *x, t_symbol *s_dst) {
    x->x_arrayname_dst = s_dst;
}

void tab_derivate_setup(void) {  
  tab_derivate_class = class_new(gensym("tab_derivate"), (t_newmethod)tab_derivate_new,  
        0, sizeof(t_tab_derivate), CLASS_DEFAULT, A_DEFSYM, A_DEFSYM, A_DEFSYM, 0);  
  class_addbang(tab_derivate_class, tab_derivate_bang);  
  class_addfloat(tab_derivate_class, tab_derivate_float);  
  class_addmethod(tab_derivate_class, (t_method)tab_derivate_src1, gensym("src1"),  A_DEFSYM, 0);  
  class_addmethod(tab_derivate_class, (t_method)tab_derivate_dst, gensym("dst"),  A_DEFSYM, 0);  
} 

