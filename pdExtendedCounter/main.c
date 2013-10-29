#include "m_pd.h"

//the new class
static t_class* extCounter_class;
//the datatype for the class
typedef struct _extCounter
{
    t_object  x_obj;
    t_int i_count;
    t_float step;
    t_int i_down, i_up;
    t_outlet* f_out;
    t_outlet* b_out;
} t_extCounter;

//the method when a bang is received
void counter_bang(t_extCounter* x)
{
    t_float f=x->i_count;
    t_int step = x->step;
    x->i_count+=step;

    if (x->i_down-x->i_up)
    {
        if ((step>0) && (x->i_count > x->i_up))
        {
            x->i_count = x->i_down;
            // so if we're reseting output a bang on the second outlet
            outlet_bang(x->b_out);
        }
        else if (x->i_count < x->i_down)
        {
            x->i_count = x->i_up;
          // so if we're reseting output a bang on the second outlet
            outlet_bang(x->b_out);
        }
    }
    outlet_float(x->f_out, f);
    post("extCounter banging");
}

//when the counter needs resetting
void counter_reset(t_extCounter* x)
{
    x->i_count = x->i_down;
    post("extCounter reseting");
}

//method to set the current value of the counter
void counter_set(t_extCounter* x, t_floatarg f)
{
    x->i_count = f;
    //no idea when this is called
    post("extCounter being set");
}

//method is called when a message is received on the second inlet
//print an error if wrong data is received. So how does it check data???
void counter_bound(t_extCounter* x, t_floatarg f1, t_floatarg f2)
{
    x->i_down = (f1<f2)?f1:f2;
    x->i_up   = (f1>f2)?f1:f2;
    post("extCounter bound method");
}

void* counter_new(t_symbol* s, int argc, t_atom* argv)
{
    t_extCounter* x = (t_extCounter*)pd_new(extCounter_class);
    t_float f1=0, f2=0;

    x->step=1;
    switch(argc)
    {
        default:
        case 3:
            x->step=atom_getfloat(argv+2);
        case 2:
            f2=atom_getfloat(argv+1);
        case 1:
            f1=atom_getfloat(argv);
            break;
        case 0:
            break;
    }
    if (argc<2) f2=f1;

    x->i_down = (f1<f2)?f1:f2;
    x->i_up   = (f1>f2)?f1:f2;

    x->i_count=x->i_down;

    inlet_new(&x->x_obj, &x->x_obj.ob_pd,
        gensym("list"),
        gensym("bound")
    );
    floatinlet_new(&x->x_obj, &x->step);

    x->f_out = outlet_new(&x->x_obj, &s_float);
    x->b_out = outlet_new(&x->x_obj, &s_bang);

    return (void*)x;
}

void pdExtCounter_setup(void)
{
    extCounter_class = class_new(gensym("pdExtCounter"),
        (t_newmethod)counter_new,
        0,
        sizeof(t_extCounter),
        CLASS_DEFAULT,
        A_GIMME,
        0
    );
    class_addbang  (extCounter_class, counter_bang);
    class_addmethod(extCounter_class,   
        (t_method)counter_reset,
        gensym("reset"),
        0);
    class_addmethod(extCounter_class,
        (t_method)counter_set,
        gensym("set"),
        A_DEFFLOAT,
        0);
    class_addmethod(extCounter_class,
        (t_method)counter_bound, gensym("bound"),
        A_DEFFLOAT, A_DEFFLOAT,
        0);
    //right click help menu helper
    class_sethelpsymbol(extCounter_class, gensym("help-pdExtCounter"));
}
