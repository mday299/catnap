#include <iostream>

#include <nlohmann/json.hpp>

#include <CatNap.h>

using json = nlohmann::json;

catnap::Response ep_index(catnap::Request req)
{
    json ret_data = {
        {"routes", {"/"}}
    };
    return catnap::Response(ret_data);
}

int main(int argc, char *argv[]) {
    (void) argc; // UNUSED
    (void) argv; // UNUSED

    catnap::CatNap app;
    app.add_route("/", ep_index);

    json jobj = {
        {"key", "value"},
    };
    std::cout << "Blah" << " " << jobj.dump() << std::endl;
    return 0;
}
