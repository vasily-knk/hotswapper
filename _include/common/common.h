#pragma once

#include <cstdint>

#include <string>
using std::string;

#include <iostream>
using std::cout;
using std::endl;

#include <memory>
using std::shared_ptr;
using std::make_shared;
using std::weak_ptr;


using std::unique_ptr;
using std::make_unique;

#include <atomic>


#define WIN32_LEAN_AND_MEAN
#include <windows.h>

#include <boost/optional.hpp>
using boost::optional;
using boost::none;

#include <boost/thread.hpp>