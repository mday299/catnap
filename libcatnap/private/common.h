#ifndef __LIBCATNAP_PRIVATE_COMMON_H__
#define __LIBCATNAP_PRIVATE_COMMON_H__

#ifdef _DEBUG
#  include <iostream>
#  define DBG(...)                                          \
    do {                                                    \
        std::cout << "DEBUG: " << __VA_ARGS__ << std::endl; \
    } while(0)
#else
#  define DBG(x)
#endif

#endif /* __LIBCATNAP_PRIVATE_COMMON_H__ */
