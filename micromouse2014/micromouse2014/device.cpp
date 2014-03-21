

#include "device.h"

using namespace std;

/********************     Directory Style Device       *********************/
device_dir::device_dir(string _base, set<string>  _attr):
dir_path(_base), dev_attr(_attr)
{
}

string
device_dir::chk_name(string  _name)
{
    set<const string>::iterator it = dev_attr.find(_name);
    if(it != dev_attr.end()) return  *it;
    throw exception();
}

template <typename _type>
void
dev::st(_type val)
{
    ofstream _f ;
    _f.open(dev_path);
    if(_f.bad() || !_f.good())
    {   cerr << "Failed to open: "<<dev_path<<"\n";
        _f.close(); return ;
    }
    _f << val;
    _f.close();
}

template <typename _type>
_type
dev::gt()
{
    ifstream _f;
    _f.open(dev_path);
    if(_f.bad() || !_f.good())
    {   cerr << "Failed to open: "<<dev_path<<"\n";
        _f.close(); throw exception();
    }
    _type val;
    _f >> val;
    _f.close();
    return val;
}

dev
device_dir::operator[](std::string _attr)
{
    return dev(dir_path +"/"+ chk_name(attr));
}


/********************    Teletype Style Device        **********************/
device_tty::device_tty(const std::string _path):
tty_path (_path)
{
    fd = open (tty_path.c_str(), O_RDWR | O_NOCTTY | O_SYNC | O_NONBLOCK );
    if(fd == -1){   cerr << "Invalid: "<<_path<<"\n";   }
}

template <typename  _type>
_type
device_tty::rd_(size_t num)
{
    char* buf = new char[num+1];
    ssize_t _len = read( fd, buf, num );
    _type _ret;
    if (num<=1){return _ret = *buf;}
    
    try{    _ret.insert(_ret.end(),buf,buf+_len );      
    }catch(...){_ret = *buf;}
    
    return _ret;
}

template <typename _type>
void
device_tty::wr_(_type val)
{
    strstream _ss;
    _ss << val;
    string _s (_ss.str());
    write(fd,_s.c_str(),_s.length());
}

