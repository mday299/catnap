#include <iostream>

#include <nlohmann/json.hpp>

#include <CatNap.h>

using json = nlohmann::json;

int main(int argc, char *argv[]) {
    (void) argc; // UNUSED
    (void) argv; // UNUSED

    catnap::CatNap app;

    json jobj = {
        {"key", "value"},
    };
    std::cout << "Blah" << " " << jobj.dump() << std::endl;
    return 0;
}
