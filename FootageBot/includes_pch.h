#ifndef INCLUDESPCH_H
#define INCLUDESPCH_H

// C++ headers
#include <algorithm>
#include <array>
#include <bit>
#include <charconv>
#include <chrono>
#include <compare>
#include <concepts>
#include <ctime>
#include <filesystem>
#include <format>
#include <fstream>
#include <functional>
#include <future>
#include <iostream>
#include <limits>
#include <map>
#include <memory>
#include <mutex>
#include <numeric>
#include <random>
#include <ranges>
#include <regex>
#include <source_location>
#include <string_view>
#include <string>
#include <thread>
#include <type_traits>
#include <unordered_map>
#include <utility>
#include <vector>

// Vcpkg headers
#pragma warning( push, 1 )  
#include <tgbot/tgbot.h>
#include <curl/curl.h>
#include <sqlite_orm/sqlite_orm.h>
#include <nlohmann/json.hpp>
#pragma warning( pop )

// Macros
#define U8(str) (char*)u8##str

#endif // !INCLUDESPCH_H