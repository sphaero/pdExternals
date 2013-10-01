#include "pd/m_pd.h"

static t_class* helloworld_class;

typedef struct _helloworld
{
    //type t_object is necessary for graphical representation in pd
    t_object x_obj;
} t_helloworld;

// BANG message handler
void helloworld_bang(t_helloworld *x)
{
    post("Hello World");
}

//constructor method
void* helloworld_new(void)
{
    t_helloworld* x = (t_helloworld*)pd_new(helloworld_class);
    //return a pointer to the instantiated data space
    return (void*) x;
}

//External initialisation is called when external is loaded
void helloworld_setup(void)
{
    helloworld_class = class_new(
        //external name
        gensym("helloworld"),
        //constructor
        (t_newmethod)helloworld_new,
        //destructor (none in this case
        0,
        //data size of the struct
        sizeof(t_helloworld),
        //this has influence on the graphical representation
        CLASS_DEFAULT,
        //remaining arguments of an object, 0 terminates arguments
        0
    );
    // map a method to a bang message
    class_addbang(helloworld_class, helloworld_bang);
}
