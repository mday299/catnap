#include <iostream>

#include <nlohmann/json.hpp>

#include <CatNap.h>

using json = nlohmann::json;

void * get_index(void *)
{
    return nullptr;
}

int main(int argc, char *argv[]) {
    (void) argc; // UNUSED
    (void) argv; // UNUSED

    catnap::CatNap app;
    //app.add_route("/", get_index);
    app.add_unix_listen("/tmp/kitten.sock");

    json jobj = {
        {"key", "value"},
    };
    std::cout << "Blah" << " " << jobj.dump() << std::endl;

    app.run();
    int code = app.get_status_code();
    return code;
}
