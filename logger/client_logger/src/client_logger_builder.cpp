#include <not_implemented.h>
#include <filesystem>
#include <fstream>

#include "../include/client_logger.h"
#include "../include/client_logger_builder.h"


client_logger_builder::client_logger_builder():
        _log_struct("%s %m")
{ }

client_logger_builder::client_logger_builder(
        std::string const &log_struct):
        _log_struct(log_struct)
{ }

logger_builder *client_logger_builder::add_file_stream(
    std::string const &stream_file_path,
    logger::severity severity)
{
    std::string absolute_path = std::filesystem::weakly_canonical(stream_file_path).string();
    // std::string absolute_path = std::filesystem::absolute(stream_file_path).string();
    _data[absolute_path].insert(severity);

    return this;
}

logger_builder *client_logger_builder::add_console_stream(
    logger::severity severity)
{
    _data[""].insert(severity);

    return this;
}

logger_builder* client_logger_builder::transform_with_configuration(
    std::string const &configuration_file_path,
    std::string const &configuration_path)
{
    std::ifstream configuration_file(configuration_file_path);

    if (!configuration_file.is_open())
    {
        throw std::runtime_error("File cannot be opened");
    }

//    std::istringstream input_str_stream(configuration_path);
//    std::string block;
//    std::string log;
//    getline(input_str_stream, block, '/');
//    getline(input_str_stream, log);
//
//    get_config_info(configuration_file, block, log);

    nlohmann::json config = nlohmann::json::parse(configuration_file);

    get_config_info(config, configuration_path);

    configuration_file.close();

    return this;
}

logger_builder *client_logger_builder::clear()
{
    _log_struct = "%s %m";
    _data.clear();

    return this;
}

logger *client_logger_builder::build() const
{
    return new client_logger(_data, _log_struct);
}

logger_builder *client_logger_builder::change_log_struct(
        std::string const &log_struct)
{
    _log_struct = log_struct;

    return this;
}

void client_logger_builder::get_config_info(
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
            config = config[buf]; // 1 pass??
        }
        else
        {
            throw std::runtime_error("Configuration was not found");
        }
    }

    _log_struct = config["_log_struct"];

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

void client_logger_builder::get_substr_queue(
        std::string const &str,
        std::queue<std::string> &queue,
        char separator)
{
    std::string substr;
    size_t i = 0, j = 0;

    while (str[i] != '\0')
    {
        if (str[i] == separator)
        {
            queue.push(substr);
            j = 0;
            substr.clear();
        }
        else
        {
            substr.replace(j, 1, 1, str[i]);
            ++j;
        }

        ++i;
    }

    queue.push(substr);
}

//void client_logger_builder::get_config_info(
//        std::ifstream &configuration_file,
//        std::string const &block,
//        std::string const &log)
//{
//    std::string buf;
//    std::string severity_str;
//    logger::severity severity;
//    int flag = 1;
//
//    while (!configuration_file.eof())
//    {
//        getline(configuration_file, buf, '{');
//        if (buf == block)
//        {
//            getline(configuration_file, buf);
//            getline(configuration_file, _log_struct);
//
//            while (!configuration_file.eof())
//            {
//                getline(configuration_file, buf, '[');
//                if (buf == log)
//                {
//                    getline(configuration_file, buf);
//
//                    flag = 0;
//                    break;
//                }
//
//                getline(configuration_file, buf, ']');
//                getline(configuration_file, buf);
//            }
//
//            break;
//        }
//
//        getline(configuration_file, buf, '}');
//        getline(configuration_file, buf);
//    }
//
//    if (!flag)
//    {
//        while (!configuration_file.eof() && buf != "]")
//        {
//            getline(configuration_file, buf, '"');
//            if (buf[0] == ']')
//            {
//                break;
//            }
//            getline(configuration_file, buf, '"');
//            getline(configuration_file, severity_str);
//
//            severity = get_severity(severity_str);
//
//            if (buf == "")
//            {
//                add_console_stream(severity);
//            }
//            else
//            {
//                add_file_stream(buf, severity);
//            }
//        }
//    }
//    else
//    {
//        throw std::runtime_error("Invalid configuration path");
//    }
//}

logger::severity client_logger_builder::get_severity(
        std::string const &severity_str)
{
    switch (severity_str[0])
    {
        case 'd':
            return logger::severity::debug;
        case 't':
            return logger::severity::trace;
        case 'i':
            return logger::severity::information;
        case 'w':
            return logger::severity::warning;
        case 'c':
            return logger::severity::critical;
        case 'e':
            return logger::severity::error;
        default:
            return logger::severity::information;
    }
}