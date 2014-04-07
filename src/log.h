//  Copyright Eelko van Breda, Delft University of Technology, 2014. 
//  Use, modification and distribution is subject to the Boost Software License,
//  Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)

#ifndef __LOG_H__
#define __LOG_H__
// #define DEBUG
#define INFO
// #define BOOSTLOG
// #ifdef BOOSTLOG
// 
// #include <boost/log/trivial.hpp>
// 
// #define logInit //logging::core::get()->set_filter (logging::trivial::severity >= logging::trivial::info);
// 
// #ifdef DEBUG
//   #define logDeb(str)       BOOST_LOG_TRIVIAL(debug) << str;
// #else
//   #define logDeb(str)     do {} while (0)
// #endif
// 
// #ifdef INFO
//   #define logInf(str)       BOOST_LOG_TRIVIAL(info) << str;
// #else
//   #define logInf(str)     do {} while (0)
// #endif
// #define logWar(str)       BOOST_LOG_TRIVIAL(warning) << str;
// #define logErr(str)       BOOST_LOG_TRIVIAL(error) << str;
// #define logFat(str)       BOOST_LOG_TRIVIAL(fatal) << str;
// 
// #else 

#include <iostream>
#define logInit //

#ifdef DEBUG
  #define logDeb(str)     std::cout << "[debug]  " << str << std::endl;
#else
  #define logDeb(str)     do {} while (0)
#endif

#ifdef INFO
  #define logInf(str)     std::cout << "[info]   " << str << std::endl;
#else
  #define logInf(str)     do {} while (0)
#endif
#define logWar(str)       std::cout << "[warning]" << str << std::endl;
#define logErr(str)       std::cout << "[error]  " << str << std::endl;
#define logFat(str)       std::cout << "[fatal]  " << str << std::endl;

//#endif

#endif //__LOG_H__