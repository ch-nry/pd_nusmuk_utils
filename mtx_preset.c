/*
mtx_preset : store and interpol presets that are in a matrix form
Copyright (C) 2014 Cyrile Henry

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/
 
 
#include "m_pd.h" 
#include "math.h"

#define sizemaxX 1000
#define sizemaxY 1000

static t_class *mtx_preset_class;  
 
typedef struct _mtx_preset {  
    t_object  x_obj;  
    t_outlet *main_outlet;
    t_int sizeX;
    t_int sizeY;
    t_float matrix[sizemaxX][sizemaxY+1];
} t_mtx_preset; 

t_float mix(t_float X, t_float Y, t_float mix)
{
    return (1-mix)*X + mix*Y ;
}

#define min(X, Y) (((X) < (Y)) ? (X) : (Y))
#define max(X, Y) (((X) > (Y)) ? (X) : (Y))

void *mtx_preset_new(t_symbol *s, int argc, t_atom *argv)  
{  
    int i, j;
    t_mtx_preset *x = (t_mtx_preset *)pd_new(mtx_preset_class);  
    
    x->sizeX = 2;
    if ((argc>0) && (argv[0].a_type == A_FLOAT))
        x->sizeX = atom_getfloatarg(0, argc, argv);
    if (x->sizeX > sizemaxX)
        x->sizeX = sizemaxX;
    if (x->sizeX < 1)
        x->sizeX = 1;
        
    x->sizeY = 2;
    if ((argc>1) && (argv[1].a_type == A_FLOAT))
        x->sizeY = atom_getfloatarg(1, argc, argv);
    if (x->sizeY > sizemaxX)
        x->sizeY = sizemaxX;
    if (x->sizeY < 1)
        x->sizeY = 1;

    for (i=0; i < sizemaxX; i++)
        for (j=0; j < sizemaxY; j++)
            x->matrix[i][j] = 0;
            
    x->main_outlet = outlet_new(&x->x_obj, 0);  
    return (void *)x;  
}  

 void mtx_preset_resize(t_mtx_preset *x, t_float sizeX, t_float sizeY)  
{ 
    x->sizeX = sizeX;
    x->sizeY = sizeY;
    
    if (x->sizeX > sizemaxX)
        x->sizeX = sizemaxX;
    if (x->sizeX < 1)
        x->sizeX = 1;
        
    if (x->sizeY > sizemaxX)
        x->sizeY = sizemaxX;
    if (x->sizeY < 1)
        x->sizeY = 1;
}


void mtx_preset_matrix(t_mtx_preset *x, t_symbol *s, int argc, t_atom *argv)  
{ 
    int X,Y, mtxX, mtxY, i, j,k;
    
    if ((argc > 3) && (argv[0].a_type == A_FLOAT) && (argv[1].a_type == A_FLOAT) && (argv[2].a_type == A_FLOAT)) {
    // reception de matrix X Y values
    // copy data, carfull if matrix and mtx_interpol have different size
        mtxX = atom_getfloatarg(0,argc,argv);
        mtxY = atom_getfloatarg(1,argc,argv);
        
        X = min(x->sizeX, mtxX);
        Y = min(x->sizeY, mtxY);
    
        for (i=0; i < X; i++) {
            for (j=0; j < Y; j++) {
                k= j*mtxX + i + 2;
                if ((argc > k) && (argv[k].a_type == A_FLOAT))
                    x->matrix[i][j] = atom_getfloatarg(k,argc,argv);
            }
        }
    }
    else if ((argc > 0) && (argv[0].a_type == A_FLOAT)) {
    // reception de matrix value
        for (i=0; i < x->sizeX; i++) {
            for (j=0; j < x->sizeY; j++) {
                x->matrix[i][j] = atom_getfloatarg(0,argc,argv);
            }
        }
    }
    else {
        pd_error(x,"bad matrix");
        return;
    }
}

void mtx_preset_row(t_mtx_preset *x, t_symbol *s, int argc, t_atom *argv)  
{ 
// reception de row Y values...
    int  row, i;
    
    if ((argc > 0) && (argv[0].a_type == A_FLOAT)) 
        row = atom_getfloatarg(0,argc,argv);
    else {
        pd_error(x,"bad row");
        return;
    }
    row = min(x->sizeY-1, row);
    row = max(0, row);
    for (i=0; i < x->sizeX; i++)
        if ((argc > i+1) && (argv[i+1].a_type == A_FLOAT))
            x->matrix[i][row] = atom_getfloatarg(i+1,argc,argv);
}

void mtx_preset_col(t_mtx_preset *x, t_symbol *s, int argc, t_atom *argv)  
{ 
// reception de col X values...
    int  col, i;
    
    if ((argc > 0) && (argv[0].a_type == A_FLOAT)) 
        col = atom_getfloatarg(0,argc,argv);
    else {
        pd_error(x,"bad col");
        return;
    }
    col = min(x->sizeX-1, col);
    col = max(0, col);
    for (i=0; i < x->sizeY; i++)
        if ((argc > i+1) && (argv[i+1].a_type == A_FLOAT))
            x->matrix[col][i] = atom_getfloatarg(i+1,argc,argv);
}

void mtx_preset_setrow(t_mtx_preset *x, t_float rowtoset, t_float value)  
{ 
// reception de row Y value
    int  row, i;
    
    row = min(x->sizeY-1, rowtoset);
    row = max(0, row);
    
    for (i=0; i < x->sizeX; i++)
        x->matrix[i][row] = value;
}

void mtx_preset_setcol(t_mtx_preset *x, t_float coltoset, t_float value) 
{ 
// reception de col X value
    int  col, i;
    
    col = min(x->sizeX-1, coltoset);
    col = max(0, col);
    
    for (i=0; i < x->sizeY; i++)
        x->matrix[col][i] = value;
}

void mtx_preset_element(t_mtx_preset *x, t_float posX, t_float posY, t_float value)  
{ 
// reception de element X Y value
    if ((posX < x->sizeX) && (posY < x->sizeY) && (posX >= 0) && (posY >= 0)) {
        x->matrix[(int)posX][(int)posY] = value;
    }
}

void mtx_preset_getMatrix(t_mtx_preset *x)  
{ 
    int i, j;
// dump matrix tailleX tailleY datas...
	t_atom std_out[x->sizeX*x->sizeY+2];
	SETFLOAT(&(std_out[0]),x->sizeX);
	SETFLOAT(&(std_out[1]),x->sizeY);
    for (i=0; i < x->sizeX; i++)
        for (j=0; j < x->sizeY; j++)
            SETFLOAT(&(std_out[i+x->sizeX*j+2]),x->matrix[i][j]);

    outlet_anything(x->main_outlet, gensym("matrix"),x->sizeX*x->sizeY+2,std_out);
}

void mtx_preset_getRows(t_mtx_preset *x)  
{ 
    int i,j;
	t_atom std_out[x->sizeX+1];
    
    for (i=0; i< x->sizeY; i++) {
        SETFLOAT(&(std_out[0]),i);
        for (j=0; j < x->sizeX; j++) {
            SETFLOAT(&(std_out[j+1]),x->matrix[j][i]);
        }
        outlet_anything(x->main_outlet, gensym("rows"),x->sizeX+1,std_out);
    }
}

void mtx_preset_getRow(t_mtx_preset *x, t_float row)  
{ 
    int i,a,b;
    t_float c;
	t_atom std_out[x->sizeX+1];
    
    a = floor(row);
    b = a+1;
    c = row-a;

    a = min(a, x->sizeY-1);
    a = max(a,0);
    b = min(b, x->sizeY-1);
    b = max(b,0);
    
    for (i=0; i < x->sizeX; i++) {
        SETFLOAT(&(std_out[i]),mix(x->matrix[i][a], x->matrix[i][b], c));
    }
    outlet_anything(x->main_outlet, gensym("row"), x->sizeX, std_out);
}

void mtx_preset_getElementss(t_mtx_preset *x)
{ 
    int i, j;
	t_atom std_out[3];

    for (j=0; j < x->sizeY; j++) {
        SETFLOAT(&(std_out[1]),j);
        for (i=0; i < x->sizeX; i++) {
            SETFLOAT(&(std_out[0]),i);
            SETFLOAT(&(std_out[2]),x->matrix[i][j]);
            outlet_anything(x->main_outlet, gensym("elementss"), 3, std_out);
        }
    }
}

void mtx_preset_getElements(t_mtx_preset *x, t_float row)
{ 
    int i, a, b;
    t_float c;
	t_atom std_out[2];

    a = floor(row);
    b = a+1;
    c = row-a;
    
    a = min(a, x->sizeY-1);
    a = max(a,0);
    b = min(b, x->sizeY-1);
    b = max(b,0);
    
    for (i=0; i < x->sizeX; i++) {
        SETFLOAT(&(std_out[0]),i);
        SETFLOAT(&(std_out[1]),mix(x->matrix[i][a], x->matrix[i][b], c));
        outlet_anything(x->main_outlet, gensym("elements"), 2, std_out);
    }
}

void mtx_preset_getElement(t_mtx_preset *x, t_float col, t_float row)
{ 
    int a, b, d;
    t_float c;
	t_atom std_out[1];

    a = floor(row);
    b = a+1;
    c = row-a;
    
    a = min(a, x->sizeY-1);
    a = max(a,0);
    b = min(b, x->sizeY-1);
    b = max(b,0);
    
    d = min(col, x->sizeX-1);
    d = max(d,0);

    SETFLOAT(&(std_out[0]),mix(x->matrix[d][a], x->matrix[d][b], c));
    outlet_anything(x->main_outlet, gensym("element"), 1, std_out);
}

void mtx_preset_mixRows(t_mtx_preset *x, t_symbol *s, int argc, t_atom *argv)
{ 
    int i, j;
	t_atom std_out[x->sizeX];
    t_float row[x->sizeX];
    
    for (i=0; i < x->sizeX; i++)
        row[i] = 0;

    for (j=0; j<min(argc, x->sizeY); j++)
        if ((argv[j].a_type == A_FLOAT) && (atom_getfloatarg(j, argc, argv) != 0))
            for (i=0; i < x->sizeX; i++)
                row[i] += x->matrix[i][j] * atom_getfloatarg(j, argc, argv);

    for (i=0; i < x->sizeX; i++)
        SETFLOAT(&(std_out[i]),row[i]);
    outlet_anything(x->main_outlet, gensym("mixRows"), x->sizeX, std_out);
}

void mtx_preset_mixElements(t_mtx_preset *x, t_symbol *s, int argc, t_atom *argv)
{ 
    int i, j;
	t_atom std_out[x->sizeX];
    t_float row[2];
    
    for (i=0; i < x->sizeX; i++)
        row[i] = 0;

    for (j=0; j<min(argc, x->sizeY); j++)
        if ((argv[j].a_type == A_FLOAT) && (atom_getfloatarg(j, argc, argv) != 0))
            for (i=0; i < x->sizeX; i++)
                row[i] += x->matrix[i][j] * atom_getfloatarg(j, argc, argv);

    for (i=0; i < x->sizeX; i++){
        SETFLOAT(&(std_out[0]),i);
        SETFLOAT(&(std_out[1]),row[i]);
        outlet_anything(x->main_outlet, gensym("mixElements"), 2, std_out);
    }
}

void mtx_preset_copyRow(t_mtx_preset *x, t_float row1, t_float row2) 
{ 
    int  i, rowA, rowB;
   
    rowA = min(x->sizeY-1, row1);
    rowA = max(0, rowA);

    rowB = min(x->sizeY-1, row2);
    rowB = max(0, rowB);
         
    for (i=0; i < x->sizeX; i++)
        x->matrix[i][rowB] = x->matrix[i][rowA];
}

void mtx_preset_setup(void) {  
    mtx_preset_class = class_new(gensym("mtx_preset"),
        (t_newmethod)mtx_preset_new,  
        0, sizeof(t_mtx_preset),  
        CLASS_DEFAULT, A_GIMME, 0);  
    class_addmethod(mtx_preset_class, (t_method)mtx_preset_matrix,    gensym("matrix"),A_GIMME, 0);                         // put Matrix 
                                                                                                                                //      matrix value -> all matrix will be at the float value
                                                                                                                                //      matrix sizeX sizeY values ... -> put the matrix in 0 0
    class_addmethod(mtx_preset_class, (t_method)mtx_preset_row,       gensym("row"),A_GIMME, 0);                            // put Row
                                                                                                                                //      row float float ... -> put all value to a row
    class_addmethod(mtx_preset_class, (t_method)mtx_preset_col,       gensym("col"),A_GIMME, 0);                            // put Col
                                                                                                                                //      coll float float ... -> put all value to a col
    class_addmethod(mtx_preset_class, (t_method)mtx_preset_setrow,    gensym("setRow"),A_FLOAT, A_FLOAT, 0);                // set Row
                                                                                                                                //      row float float -> put value to a row
    class_addmethod(mtx_preset_class, (t_method)mtx_preset_setcol,    gensym("setCol"),A_FLOAT, A_FLOAT, 0);                // set Col
                                                                                                                                //      coll float float -> put value to a col
    class_addmethod(mtx_preset_class, (t_method)mtx_preset_element,   gensym("element"),A_FLOAT, A_FLOAT, A_FLOAT, 0);      // put 1 element
                                                                                                                                //      element posX posY value
    class_addmethod(mtx_preset_class, (t_method)mtx_preset_getMatrix, gensym("getMatrix"), 0);                              // get matrix  (matrix sizeX sizeY value)
    
    class_addmethod(mtx_preset_class, (t_method)mtx_preset_getRows,   gensym("getRows"), 0);                                // get row
                                                                                                                                //      getRow -> dump all row (row now_number row values)
    class_addmethod(mtx_preset_class, (t_method)mtx_preset_getRow,    gensym("getRow"), A_FLOAT, 0);                        // get row
                                                                                                                                //      getRow row_number -> dump 1 row (values) 
                                                                                                                                //          row_number can be float : row will be interpolated
    class_addmethod(mtx_preset_class, (t_method)mtx_preset_getElementss,gensym("getElementss"), 0);                         // get elements

    class_addmethod(mtx_preset_class, (t_method)mtx_preset_getElements,gensym("getElements"), A_FLOAT, 0);                  // get elements
                                                                                                                                //      get posX -> dump elements from 1 row
                                                                                                                                //          row_number can be float : row will be interpolated
    class_addmethod(mtx_preset_class, (t_method)mtx_preset_getElement,gensym("getElement"), A_FLOAT, A_FLOAT, 0);           // get element
                                                                                                                                //      get posX posY -> dump 1 element
                                                                                                                                //          row_number can be float : row will be interpolated
    class_addmethod(mtx_preset_class, (t_method)mtx_preset_mixRows,   gensym("mixRows"), A_GIMME, 0);                       // interpol between all row
                                                                                                                                //      mixRow value0 value1 value2 : (value0*row0 + value1*row1 + value2*row2)
    class_addmethod(mtx_preset_class, (t_method)mtx_preset_mixElements,gensym("mixElements"), A_GIMME, 0);                  // interpol between all row
                                                                                                                                //      mixELments value0 value1 value2 : (value0*row0 + value1*row1 + value2*row2)
    class_addmethod(mtx_preset_class, (t_method)mtx_preset_copyRow,   gensym("copyRow"), A_FLOAT, A_FLOAT, 0);              // copy row 1 to row 2                                                                                                                                
    class_addmethod(mtx_preset_class, (t_method)mtx_preset_resize,    gensym("resize"),A_FLOAT, A_FLOAT, 0);                // resize matrix
} 
