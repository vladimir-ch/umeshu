#ifndef __EXCEPTIONS_H_INCLUDED__
#define __EXCEPTIONS_H_INCLUDED__ 

#include <boost/exception/exception.hpp>
#include <boost/exception/diagnostic_information.hpp>

namespace umeshu {

struct umeshu_error : virtual std::exception, virtual boost::exception { };

} // namespace umeshu

#endif /* __EXCEPTIONS_H_INCLUDED__ */
