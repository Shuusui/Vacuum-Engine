#ifndef JSON_H
#define JSON_H



//TODO: ThePJson.hpp file has still some warnings and I might address them later on. 
#pragma warning(disable : 6011)
#pragma warning(disable : 28020)
#pragma warning(disable : 26495)
#include "..\Private\json.hpp"
#pragma warning(default : 6011)
#pragma warning(default : 28020)
#pragma warning(default : 26495)

using PJson = nlohmann::json;
#endif //JSON_H