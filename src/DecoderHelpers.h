//
// Created by gnilk on 17.12.2025.
//

#ifndef GNILK_DECODERHELPERS_H
#define GNILK_DECODERHELPERS_H

#include <charconv>
#include <string_view>
#include <string>
#include <optional>

#if defined(__cpp_lib_to_chars) && __cpp_lib_to_chars >= 201611L
    #define HAS_FROM_CHARS 1
#else
    #define HAS_FROM_CHARS 0
#endif

namespace gnilk {
    template<typename T>
    [[nodiscard]]
    static std::optional<T> convert_to(std::string_view sv) {

        if constexpr (std::is_same_v<T, std::string>) {
            return std::string{sv};
        }
        else if constexpr (std::is_same_v<T, std::string_view>) {
            return sv;
        }
        else if constexpr (std::is_same_v<T, bool>) {
            if (sv == "1" || sv == "true"  || sv == "TRUE")  return true;
            if (sv == "0" || sv == "false" || sv == "FALSE") return false;
            return std::nullopt;
        }
        else if constexpr (std::is_arithmetic_v<T>) {
#if HAS_FROM_CHARS
            T out{};
            auto first = sv.data();
            auto last  = sv.data() + sv.size();

            // std::from_chars handles all integers and floating point (C++17+)
            auto [ptr, ec] = std::from_chars(first, last, out);
            if (ec == std::errc{} && ptr == last)
                return out;

            return std::nullopt;
#else
// ---- fallback path (macOS) ----
            const char* begin = sv.data();
            char* end = nullptr;
            errno = 0;

            if constexpr (std::is_integral_v<T>) {
                long long value = std::strtoll(begin, &end, 10);

                if (errno != 0 || end != begin + sv.size())
                    return std::nullopt;

                if constexpr (std::is_signed_v<T>) {
                    if (value < std::numeric_limits<T>::min() ||
                        value > std::numeric_limits<T>::max())
                        return std::nullopt;
                } else {
                    if (value < 0 ||
                        static_cast<unsigned long long>(value) >
                        std::numeric_limits<T>::max())
                        return std::nullopt;
                }

                return static_cast<T>(value);

            } else if constexpr (std::is_floating_point_v<T>) {
                long double value = std::strtold(begin, &end);

                if (errno != 0 || end != begin + sv.size())
                    return std::nullopt;

                return static_cast<T>(value);
            }
#endif
        }
        else {
            // Unsupported type; static assert provides helpful compile-time error
            static_assert(!sizeof(T*), "convert_to<T>: Unsupported type");
        }
    }

}
#endif //ENCDEC_DECODERHELPERS_H
