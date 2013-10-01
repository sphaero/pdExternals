#include "pd/m_pd.h"

static t_class* counter_class;

typedef struct _counter
{
    //type t_object is necessary for graphical representation in pd
    t_object x_obj;
    // this holds the counter
    t_int i_count;
} t_counter;

void counter_bang(t_counter* x)
{
    t_float f=x->i_count;
    x->i_count++;
    outlet_float(x->x_obj.ob_outlet, f);
}

void* counter_new(t_floatarg f)
{
    //generate a new instance of t_counter struct
    t_counter* x = (t_counter*)pd_new(counter_class);
    //store the value of f in our counter;
    //Puredata only knows the float number type (no integers)
    x->i_count=f;
    //create a new outlet in PD where the value of the counter is outputted
    outlet_new(&x->x_obj, &s_float);
    //return a pointer to the instantiated data space
    return (void*)x;
}

//called by PD on load
void pdCounter_setup(void)
{
    counter_class = class_new(gensym(
        "pdCounter"),
        (t_newmethod)counter_new,
        0,
        sizeof(t_counter),
        CLASS_DEFAULT,
        A_DEFFLOAT, 0
    );
    class_addbang(counter_class, counter_bang);
}
