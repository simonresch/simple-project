#include "json_api.h"
#include "json11.hpp"

void DoJSONStuff(const std::string &str) {
    std::string err;
    json11::Json json = json11::Json::parse(str, err);    

    if (err.empty()) {
        auto foo = json["foo"];
        if (foo.is_string()) {
            if (foo.string_value().find("deadbeef123456789") != std::string::npos){
                __builtin_trap();
            }
        }
    }
}