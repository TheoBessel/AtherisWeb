//
// Created by Theo Bessel on 2019-03-25.
//

#ifndef WEBRENDER_LOGGER_HPP
#define WEBRENDER_LOGGER_HPP

#include <iostream>

namespace usual {
    inline void plog(){};
    template <typename T, typename... Args> inline void plog(T&& t, Args&& ...args) {
        std::cout << t;
        plog(std::forward<Args>(args)...);
        std::cout << std::endl;
    }
    inline void clog(){};
    template <typename T, typename... Args> inline void clog(T&& t, Args&& ...args) {
        std::cerr << t;
        clog(std::forward<Args>(args)...);
        std::cerr << std::endl;
    }
}

#endif //WEBRENDER_LOGGER_HPP