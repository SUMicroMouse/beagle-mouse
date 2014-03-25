

#include "device.h"

using namespace std;

/********************     Directory Style Device       *********************/
device_dir::device_dir(string _base, 
                       initializer_list<string>  ls_attr):
dir_path(*new string(_base))
{
    for( auto it = ls_attr.begin(); it != ls_attr.end(); it++)
    {
        auto _atrib = *new string(*it);
        dev_attr.emplace(pair<string,dev&>
                         (_atrib, *new dev(dir_path+_atrib)) );
    }
}
device_dir::device_dir(const char* _base, 
                       initializer_list<const char*>  ls_attr):
dir_path(*new string(_base))
{
    for( auto it = ls_attr.begin(); it != ls_attr.end(); it++)
    {
        auto _atrib = *new string(*it);
        dev_attr.emplace(pair<string,dev&>
                         (_atrib, *new dev(dir_path+_atrib)) );
    }
}


dev&
device_dir::operator[](string _attr)
{
    return dev_attr.at(_attr);
}

dev&
device_dir::operator[](const char* _attr)
{
    return (*this)[ string(_attr) ];
}


/* *******************   Device Attribute Get/Set    ******************** */
template <class _type>
void
dev::st(_type val)
{
    ofstream _f ;
    _f.open(dev_path.c_str());
    if(_f.bad() || !_f.good())
    {   
        cerr << "Failed to open: "<<dev_path<<"\n";
        _f.close(); return ;
    }
    _f << val;
    _f.close();
    cout<<"[DBG] "<< dev_path<<" << == "<<val <<endl;
    return;
}

template <class _type>
_type
dev::gt()
{
    ifstream _f;
    _f.open(dev_path.c_str());
    if(_f.bad() || !_f.good())
    {   
        cerr << "Failed to open: "<<dev_path<<"\n";
        _f.close(); throw exception();
    }
    _type val;
    _f >> val;
    _f.close();
    cout<<"[DBG] "<< dev_path<<" == >> "<<val <<endl;
    return val;
}


