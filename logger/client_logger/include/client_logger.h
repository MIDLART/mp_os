#ifndef MATH_PRACTICE_AND_OPERATING_SYSTEMS_CLIENT_LOGGER_H
#define MATH_PRACTICE_AND_OPERATING_SYSTEMS_CLIENT_LOGGER_H

#include <logger.h>
#include "client_logger_builder.h"

#include <map>
#include <unordered_map>
#include <set>
#include <string>

class client_logger final:
    public logger
{

    friend class client_logger_builder;

private:
    std::map<std::string, std::pair<std::ostream *, std::set<logger::severity>>> _streams;

    static std::unordered_map<std::string, std::pair<std::ostream *, size_t>> _all_streams;

    std::string _log_struct;

private:
    client_logger(
            std::map<std::string, std::set<logger::severity>> const &data,
            std::string log_struct);

private:
    std::string log_string_parse(
            const std::string &text,
            logger::severity severity) const;

public:

    client_logger(
        client_logger const &other);

    client_logger &operator=(
        client_logger const &other);

    client_logger(
        client_logger &&other) noexcept;

    client_logger &operator=(
        client_logger &&other) noexcept;

    ~client_logger() noexcept final;

public:

    [[nodiscard]] logger const *log(
        const std::string &message,
        logger::severity severity) const noexcept override;

private:

    void decrement_stream(
            std::string const &file_path) const noexcept;

};

#endif //MATH_PRACTICE_AND_OPERATING_SYSTEMS_CLIENT_LOGGER_H