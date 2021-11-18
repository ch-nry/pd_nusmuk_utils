#include "m_pd.h"  

#define MIN(a,b) (((a)<(b))?(a):(b))

static t_class *tab_integrate_class;  
 
typedef struct _tab_integrate {  
    t_object  x_obj;  
    t_symbol *x_arrayname_src1;
    t_symbol *x_arrayname_dst;
    t_outlet *b_out;
} t_tab_integrate;  
 
void *tab_integrate_new(t_symbol *s_src1, t_symbol *s_dst)  {  
    t_tab_integrate *x = (t_tab_integrate *)pd_new(tab_integrate_class);  
 
    x->x_arrayname_src1 = s_src1;
    x->x_arrayname_dst = s_dst;
    
    x->b_out = outlet_new(&x->x_obj, &s_bang);  

    return (void *)x;  
}  

void tab_integrate_compute(t_tab_integrate *x, t_float nb)  {  
    t_garray *a_src1, *a_dst;
    int npoints_src1, npoints_dst;
    t_word *vec_src1, *vec_dst;
    t_float integral;
    
    int nb_integrate, i;
    
    integral = 0;
    
    if (!(a_src1 = (t_garray *)pd_findbyclass(x->x_arrayname_src1, garray_class)))
        pd_error(x, "%s: no such array", x->x_arrayname_src1->s_name);
    else if (!garray_getfloatwords(a_src1, &npoints_src1, &vec_src1))
        pd_error(x, "%s: bad template for tab_integrate", x->x_arrayname_src1->s_name);
    else if (!(a_dst = (t_garray *)pd_findbyclass(x->x_arrayname_dst, garray_class)))
        pd_error(x, "%s: no such array", x->x_arrayname_dst->s_name);
    else if (!garray_getfloatwords(a_dst, &npoints_dst, &vec_dst))
        pd_error(x, "%s: bad template for tab_integrate", x->x_arrayname_dst->s_name);
    else
    {
        nb_integrate = MIN(npoints_src1, npoints_dst);
        // post("nb_integrate = %d",nb_integrate);

        if (nb > 0)
            nb_integrate = MIN(nb_integrate, nb);

        for (i=0; i<nb_integrate;i++) {
            integral += vec_src1[i].w_float;
            vec_dst[i].w_float = integral;
        }
        garray_redraw(a_dst);
    }
    outlet_bang(x->b_out);  
}  
 
void tab_integrate_bang(t_tab_integrate *x)  {
    tab_integrate_compute(x, -1) ;
}

void tab_integrate_float(t_tab_integrate *x, t_float nb)  {
    tab_integrate_compute(x, nb) ;
}

void tab_integrate_src1(t_tab_integrate *x, t_symbol *s_src) {
    x->x_arrayname_src1 = s_src;
}

void tab_integrate_dst(t_tab_integrate *x, t_symbol *s_dst) {
    x->x_arrayname_dst = s_dst;
}

void tab_integrate_setup(void) {  
  tab_integrate_class = class_new(gensym("tab_integrate"), (t_newmethod)tab_integrate_new,  
        0, sizeof(t_tab_integrate), CLASS_DEFAULT, A_DEFSYM, A_DEFSYM, A_DEFSYM, 0);  
  class_addbang(tab_integrate_class, tab_integrate_bang);  
  class_addfloat(tab_integrate_class, tab_integrate_float);  
  class_addmethod(tab_integrate_class, (t_method)tab_integrate_src1, gensym("src1"),  A_DEFSYM, 0);  
  class_addmethod(tab_integrate_class, (t_method)tab_integrate_dst, gensym("dst"),  A_DEFSYM, 0);  
} 

