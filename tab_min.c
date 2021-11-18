#include "m_pd.h"  
 
#define MIN(a,b) (((a)<(b))?(a):(b))
#define MAX(a,b) (((a)>(b))?(a):(b))

static t_class *tab_min_class;  
 
typedef struct _tab_min {  
    t_object  x_obj;  
    t_symbol *x_arrayname_src1;
    t_symbol *x_arrayname_src2;
    t_symbol *x_arrayname_dst;
    int offset_src1, offset_src2, offset_dst, nb_max;
    t_outlet *b_out;
} t_tab_min;  
 
void *tab_min_new(t_symbol *s_src1, t_symbol *s_src2, t_symbol *s_dst)  
{  
    t_tab_min *x = (t_tab_min *)pd_new(tab_min_class);  
 
    x->x_arrayname_src1 = s_src1;
    x->x_arrayname_src2 = s_src2;
    x->x_arrayname_dst = s_dst;
    x->offset_src1 = 0;
    x->offset_src2 = 0;
    x->offset_dst = 0;
    
    x->b_out = outlet_new(&x->x_obj, &s_bang);  

    return (void *)x;  
}  

void tab_min_compute(t_tab_min *x)  
{  
    t_garray *a_src1, *a_src2, *a_dst;
    int npoints_src1, npoints_src2, npoints_dst;
    t_word *vec_src1, *vec_src2, *vec_dst;
        
    int nb_max, i;
    
    if (!(a_src1 = (t_garray *)pd_findbyclass(x->x_arrayname_src1, garray_class)))
        pd_error(x, "%s: no such array", x->x_arrayname_src1->s_name);
    else if (!garray_getfloatwords(a_src1, &npoints_src1, &vec_src1))
        pd_error(x, "%s: bad template for tab_min", x->x_arrayname_src1->s_name);
    else if (!(a_src2 = (t_garray *)pd_findbyclass(x->x_arrayname_src2, garray_class)))
        pd_error(x, "%s: no such array", x->x_arrayname_src2->s_name);
    else if (!garray_getfloatwords(a_src2, &npoints_src2, &vec_src2))
        pd_error(x, "%s: bad template for tab_min", x->x_arrayname_src2->s_name);
    else if (!(a_dst = (t_garray *)pd_findbyclass(x->x_arrayname_dst, garray_class)))
        pd_error(x, "%s: no such array", x->x_arrayname_dst->s_name);
    else if (!garray_getfloatwords(a_dst, &npoints_dst, &vec_dst))
        pd_error(x, "%s: bad template for tab_min", x->x_arrayname_dst->s_name);
    else
    {
        nb_max = MIN(npoints_src1 - x->offset_src1, npoints_src2 - x->offset_src2);
        nb_max = MIN(npoints_dst, nb_max);
        if (x->nb_max >0)
            nb_max = MIN(nb_max, x->nb_max);
        // post("nb_max = %d",nb_max);

        for (i=0; i<nb_max;i++) {
            vec_dst[i + x->offset_dst].w_float = MIN(vec_src1[i + x->offset_src1].w_float, vec_src2[i + x->offset_src2].w_float);
        }
        garray_redraw(a_dst);
    }
    outlet_bang(x->b_out);  
}  
 
void tab_min_bang(t_tab_min *x)  
{
    x->offset_src1 = 0;
    x->offset_src2 = 0;
    x->offset_dst = 0;
    x->nb_max = -1;
    tab_min_compute(x) ;
}

void tab_min_list(t_tab_min *x, t_symbol *s, int argc, t_atom *argv)
{
    x->offset_src1 = 0;
    x->offset_src2 = 0;
    x->offset_dst = 0;
    x->nb_max = -1;
    
    if ((argc>=1) && (argv[0].a_type == A_FLOAT))
        x->offset_src1 = atom_getfloatarg(0, argc, argv);
    if ((argc>=2) && (argv[1].a_type == A_FLOAT))
        x->offset_src2 = atom_getfloatarg(1, argc, argv);
    if ((argc>=3) && (argv[2].a_type == A_FLOAT))
        x->offset_dst = atom_getfloatarg(2, argc, argv);
    if ((argc>=4) && (argv[3].a_type == A_FLOAT))
        x->nb_max = atom_getfloatarg(3, argc, argv);
        
    tab_min_compute(x) ;
}

void tab_min_src1(t_tab_min *x, t_symbol *s_src) {
    x->x_arrayname_src1 = s_src;
    
}void tab_min_src2(t_tab_min *x, t_symbol *s_src) {
    x->x_arrayname_src2 = s_src;
}

void tab_min_dst(t_tab_min *x, t_symbol *s_dst) {
    x->x_arrayname_dst = s_dst;
}

void tab_min_setup(void) {  
  tab_min_class = class_new(gensym("tab_min"), (t_newmethod)tab_min_new,  
        0, sizeof(t_tab_min), CLASS_DEFAULT, A_DEFSYM, A_DEFSYM, A_DEFSYM, 0);  
  class_addbang(tab_min_class, tab_min_bang);  
  class_addlist(tab_min_class, (t_method)tab_min_list);
  class_addmethod(tab_min_class, (t_method)tab_min_src1, gensym("src1"),  A_DEFSYM, 0);  
  class_addmethod(tab_min_class, (t_method)tab_min_src2, gensym("src2"),  A_DEFSYM, 0);  
  class_addmethod(tab_min_class, (t_method)tab_min_dst, gensym("dst"),  A_DEFSYM, 0);  
} 

