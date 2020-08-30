#include <iostream>

#include <nlohmann/json.hpp>

#include <CatNap.h>

using json = nlohmann::json;

int main(int argc, char *argv[]) {
    json jobj = {
        {"key", "value"},
    };
    std::cout << "Blah" << " " << jobj.dump() << std::endl;
    return 0;
}
