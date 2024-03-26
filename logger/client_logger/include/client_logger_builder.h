#ifndef MATH_PRACTICE_AND_OPERATING_SYSTEMS_CLIENT_LOGGER_BUILDER_H
#define MATH_PRACTICE_AND_OPERATING_SYSTEMS_CLIENT_LOGGER_BUILDER_H

#include <logger_builder.h>
#include <map>
#include <set>
#include <exception>

class client_logger_builder final:
    public logger_builder
{

private:

    std::map<std::string, std::set<logger::severity>> _data;

    std::string _log_struct;

public:

    client_logger_builder();

    client_logger_builder(
            std::string const &log_struct);


    client_logger_builder(
        client_logger_builder const &other) = default;

    client_logger_builder &operator=(
        client_logger_builder const &other) = default;

    client_logger_builder(
        client_logger_builder &&other) noexcept = default;

    client_logger_builder &operator=(
        client_logger_builder &&other) noexcept = default;

    ~client_logger_builder() noexcept override = default;

public:

    logger_builder *add_file_stream(
        std::string const &stream_file_path,
        logger::severity severity) override;

    logger_builder *add_console_stream(
        logger::severity severity) override;

    logger_builder *transform_with_configuration(
        std::string const &configuration_file_path,
        std::string const &configuration_path) override;

    logger_builder *clear() override;

    [[nodiscard]] logger *build() const override;

    logger_builder *change_log_struct(
            std::string const &log_struct);

private:
    logger::severity get_severity(
            std::string const &severity_str);

    void get_config_info(
            std::ifstream &configuration_file,
            std::string const &block,
            std::string const &log);
};

#endif //MATH_PRACTICE_AND_OPERATING_SYSTEMS_CLIENT_LOGGER_BUILDER_H