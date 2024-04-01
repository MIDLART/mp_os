#include <not_implemented.h>

#include "../include/server_logger_builder.h"
#include <server_logger.h>

#include <fstream>
#include <filesystem>
#include <queue>


logger_builder *server_logger_builder::add_file_stream(
    std::string const &stream_file_path,
    logger::severity severity)
{
    std::string absolute_path = std::filesystem::weakly_canonical(stream_file_path).string();
    _data[absolute_path].insert(severity);

    return this;
}

logger_builder *server_logger_builder::add_console_stream(
    logger::severity severity)
{
    _data[""].insert(severity);

    return this;
}

logger_builder* server_logger_builder::transform_with_configuration(
    std::string const &configuration_file_path,
    std::string const &configuration_path)
{
    std::ifstream configuration_file(configuration_file_path);

    if (!configuration_file.is_open())
    {
        throw std::runtime_error("File cannot be opened");
    }

    nlohmann::json config = nlohmann::json::parse(configuration_file);

    clear();

    get_config_info(config, configuration_path);

    configuration_file.close();

    return this;
}

logger_builder *server_logger_builder::clear()
{
    _data.clear();

    return this;
}

logger *server_logger_builder::build() const
{
    return new server_logger(_data);
}

void server_logger_builder::get_config_info(
        nlohmann::json &config,
        std::string const &configuration_path)
{
    std::string buf;
    std::string severity_str;
    logger::severity severity;

    std::queue<std::string> queue;

    get_substr_queue(configuration_path, queue, '/');

    while (!queue.empty())
    {
        buf = queue.front();
        queue.pop();

        if (config.find(buf) != config.end())
        {
            config = config[buf];
        }
        else
        {
            throw std::runtime_error("Configuration was not found");
        }
    }

    config = config["files"];

    for (auto& file : config.items())
    {
        buf = file.key();

        for (auto& severity_iter : file.value())
        {
            severity_str = severity_iter;
            severity = get_severity(severity_str);

            if (buf[0] == '\0')
            {
                add_console_stream(severity);
            }
            else
            {
                add_file_stream(buf, severity);
            }
        }
    }
}