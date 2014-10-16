//
//  cardinal.h
//  micromouse2014
//
//  Created by Lyle Moffitt on 10/13/14.
//  Copyright (c) 2014 Lyle Moffitt. All rights reserved.
//

#ifndef micromouse2014_cardinal_h
#define micromouse2014_cardinal_h

#include <tuple>
#include <array>
#include <utility>
#include <vector>
#include <functional>

enum DIR 
{
	_N_=0,
	_S_=1,
	_E_=2,
	_W_=3
};


template <class _type>
struct rose 
{
//    typedef typename std::conditional<std::is_reference<_type>::value,std::reference_wrapper<_type>,_type>::type val_type;
    typedef std::reference_wrapper<_type> ref_type;
    std::tuple<_type,_type,_type,_type> _tup;
//    _type   north, south, east, west;
    
    _type&      N(){    return std::get<DIR::_N_>(_tup);  }
    _type&      E(){    return std::get<DIR::_E_>(_tup);   }
    _type&      S(){    return std::get<DIR::_S_>(_tup);  }
    _type&      W(){    return std::get<DIR::_W_>(_tup);   }
    
    /// Get using enum direction
    _type& operator[](DIR d)
    {   
        switch(d){
            case DIR::_N_: return  std::get<DIR::_N_>(_tup); 
            case DIR::_E_: return  std::get<DIR::_E_>(_tup); 
            case DIR::_S_: return  std::get<DIR::_S_>(_tup);
            case DIR::_W_: return  std::get<DIR::_W_>(_tup);
        } 
    }
    
    /// Get using char for direction
    _type& operator[](const char d)
    { 
        switch(d){
            case 'N':case 'n': return  std::get<DIR::_N_>(_tup); 
            case 'S':case 'e': return  std::get<DIR::_E_>(_tup); 
            case 'E':case 's': return  std::get<DIR::_S_>(_tup);
            case 'W':case 'w': return  std::get<DIR::_W_>(_tup);
            default: throw std::invalid_argument
                ("Argument must be one of {N,E,S,W,} or {n,e,s,w}");
        }
    }
    

    /// Default constructor
    
    rose():_tup()
    {
        std::vector<int> v;
    }
    
//    /// Tuple constructor
//    template<typename arg_t>
//    rose(arg_t a):
//        std::tuple<_type,_type,_type,_type>(a)
//    {
//    }
//    /// Tuple constructor
//    template<typename arg_t>
//    rose(const arg_t& a...):
//    std::tuple<_type,_type,_type,_type>(std::forward_as_tuple(a))
//    {
//    }
//    /// Tuple constructor
//    template<typename arg_t>
//    rose(arg_t&& a...):
//    std::tuple<_type,_type,_type,_type>(std::make_tuple(a))
//    {
//    }
    
//    /// move constructor
//    rose(rose&& _r):
//        _tup(std::forward_as_tuple(_r._tup))
//    {}
    
    /// Copy constructor
    rose(const _type &_n, const _type &_e, const _type &_s, const _type &_w):
        _tup( _n, _e, _s, _w )
    {}
    
//    /// Copy constructor
//    rose(_type &&_n, _type &&_e, _type &&_s, _type &&_w):
//    std::tuple<_type,_type,_type,_type>
//    (  std::forward<_type>(_n), 
//        std::forward<_type>(_e), 
//        std::forward<_type>(_s), 
//        std::forward<_type>(_w)      )
//    {}
//    /// Copy constructor
//    rose(std::initializer_list<_type> _r):
//    std::tuple<_type,_type,_type,_type>
//    ({  *(_r.begin()+DIR::_N_),*(_r.begin()+DIR::_E_),
//        *(_r.begin()+DIR::_S_),*(_r.begin()+DIR::_W_)
//    })
//    {}
    /// Copy constructor
    explicit 
    rose(std::array<_type,4> _r):
        _tup( _r[DIR::_N_],_r[DIR::_E_],_r[DIR::_S_],_r[DIR::_W_] )
    {}
    
    /// Copy constructor
    rose(rose &_r):
        _tup( _r[DIR::_N_],_r[DIR::_E_],_r[DIR::_S_],_r[DIR::_W_] )
    {}
    

//    /// Assignment operator
//    rose& operator=(std::array<_type,4> _r)
//    {
//        std::get<DIR::_N_>(*this) = std::get<DIR::_N_>(_r);
//        std::get<DIR::_E_>(*this) = std::get<DIR::_E_>(_r);
//        std::get<DIR::_S_>(*this) = std::get<DIR::_S_>(_r);
//        std::get<DIR::_W_>(*this) = std::get<DIR::_W_>(_r);
//        return *this;
//    }
//    
    /// Assignment operator
    rose& operator=(std::tuple<_type,_type,_type,_type> _r)
    {
        std::get<DIR::_N_>(_tup) = std::get<DIR::_N_>(_r);
        std::get<DIR::_E_>(_tup) = std::get<DIR::_E_>(_r);
        std::get<DIR::_S_>(_tup) = std::get<DIR::_S_>(_r);
        std::get<DIR::_W_>(_tup) = std::get<DIR::_W_>(_r);
        return *this;
    }
    
    /// Assignment operator
    rose& operator=(const rose& _r)
    {
        std::get<DIR::_N_>(_tup) = std::get<DIR::_N_>(_r._tup);
        std::get<DIR::_E_>(_tup) = std::get<DIR::_E_>(_r._tup);
        std::get<DIR::_S_>(_tup) = std::get<DIR::_S_>(_r._tup);
        std::get<DIR::_W_>(_tup) = std::get<DIR::_W_>(_r._tup);
        return *this;
    }
//    /// Assignment operator
//    rose& operator=(rose&& _r)
//    {
////        (*this) = std::forward_as_tuple(_r);
//        std::get<DIR::_N_>(_tup) = std::forward<_type>(std::get<DIR::_N_>(_r));
//        std::get<DIR::_E_>(_tup) = std::forward<_type>(std::get<DIR::_E_>(_r));
//        std::get<DIR::_S_>(_tup) = std::forward<_type>(std::get<DIR::_S_>(_r));
//        std::get<DIR::_W_>(_tup) = std::forward<_type>(std::get<DIR::_W_>(_r));
//        return *this;
//    }
    

    
    
};

typedef std::integral_constant<char, 'N'> north_char;
typedef std::integral_constant<char, 'S'> south_char;
typedef std::integral_constant<char, 'E'> east_char;
typedef std::integral_constant<char, 'W'> west_char;

typedef std::integral_constant<int, 0> north_int;
typedef std::integral_constant<int, 1> south_int;
typedef std::integral_constant<int, 2> east_int;
typedef std::integral_constant<int, 3> west_int;

typedef std::integral_constant<DIR, DIR::_N_> north_dir;
typedef std::integral_constant<DIR, DIR::_S_> south_dir;
typedef std::integral_constant<DIR, DIR::_E_> east_dir;
typedef std::integral_constant<DIR, DIR::_W_> west_dir;

#include <string>
void sdfsdf()
{
    rose<std::string> rose_str, str2;

    rose<std::string> rose_st = {"","","",""};
    
    str2 = rose_str;
    
    int i1,i2,i3,i4;
    rose<int&>  rose_ref_int {i1,i2,i3,i4};
    rose<int> rose_int, ints{0,0,0,0};
    
//    rose<int&> refs (std::move(rose_ref_int));
    
    rose_int = ints;
    
//    rose_int = {0,0,0,0};
    ints = rose_int;
    
    
}
#endif





