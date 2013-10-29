/*
 * main.c
 *
 *  Created on: Oct 29, 2013
 *      Author: arnaud
 *
 *  This PD external accepts any input and gives the same output
 *  It's just to understand atoms and how to process them in C
 */

#include "m_pd.h"

// the new class
static t_class* atomsTest_class;
// its data structure
typedef struct _atomsTest
{
	t_object	x_obj;
	t_outlet*	a_out;
} t_atomsTest;

// method when a bang is received
void atomsTest_bang(t_atomsTest* x)
{
	t_atom a[2];
	SETFLOAT(&a[0], 10);
	SETFLOAT(&a[1], 5);
	outlet_list(x->a_out, gensym("vlaout"),2, a);
	//outlet_float(x->a_out, 12.0f);
}

void* atomsTest_new(t_symbol* s, int argc, t_atom* argv)
{
	t_atomsTest* x = (t_atomsTest*)pd_new(atomsTest_class);
	x->a_out = outlet_new(&x->x_obj, &s_float);
	post("number of args: %d", argc);
	return (void*)x;
}

void pdAtomsTest_setup(void)
{
	atomsTest_class = class_new(gensym("atomsTest"),
		(t_newmethod)atomsTest_new,
		0,
		sizeof(t_atomsTest),
		CLASS_DEFAULT,
		A_GIMME,
		0
	);
	class_addbang(atomsTest_class, atomsTest_bang);
}
