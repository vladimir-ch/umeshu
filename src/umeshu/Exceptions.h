#ifndef UMESHU_EXCEPTIONS_H
#define UMESHU_EXCEPTIONS_H 

#include <boost/exception/exception.hpp>
#include <boost/exception/diagnostic_information.hpp>

#include <string>

namespace umeshu {

typedef boost::error_info<struct tag_errinfo_desc, std::string> errinfo_desc;

struct umeshu_error : virtual std::exception, virtual boost::exception { };
struct triangulation_error : virtual umeshu_error { };
struct add_face_error : virtual triangulation_error { };
struct bad_topology_error : virtual triangulation_error { };

} // namespace umeshu

#endif // UMESHU_EXCEPTIONS_H
