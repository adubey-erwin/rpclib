#include "callme/client.h"
#include "callme/server.h"

#include "msgpack.hpp"
#include <iostream>

struct point {
    int x;
    int y;
    MSGPACK_DEFINE(x, y)
};

std::ostream& operator<<(std::ostream& os, point const& p) {
    os << "point(" << p.x << ", " << p.y << ")";
    return os;
}

int main() {
    callme::server s("localhost", 8080);

    s.bind("transform", [](point &p) {
        std::cout << "Transforming " << p << std::endl;
        point t;
        t.x = p.x * 2;
        t.y = p.y * 3;
        return t;
    });

    s.async_run();

    point a { 1, 2 };
    callme::client c("localhost", 8080);

    auto result = c.call("transform", a).as<point>();

    std::cout << "Transformed: " << result << std::endl;
    return 0;
}