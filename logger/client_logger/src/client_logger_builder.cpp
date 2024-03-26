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
    _data["console"].insert(severity);

    return this;
}

logger_builder* client_logger_builder::transform_with_configuration(
    std::string const &configuration_file_path,
    std::string const &configuration_path)
{
    std::ifstream configuration_file(configuration_file_path);
    //configuration_file.open(configuration_file_path);

    if (!configuration_file.is_open())
    {
        throw std::runtime_error("File cannot be opened");
    }

    std::istringstream input_str_stream(configuration_path);
    std::string block;
    std::string log;
    getline(input_str_stream, block, '/');
    getline(input_str_stream, log);

    get_config_info(configuration_file, block, log);

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
        std::ifstream &configuration_file,
        std::string const &block,
        std::string const &log)
{
    std::string buf;
    std::string severity_str;
    logger::severity severity;
    int flag = 1;

    while (!configuration_file.eof())
    {
        getline(configuration_file, buf, '{');
        if (buf == block)
        {
            getline(configuration_file, buf);
            getline(configuration_file, _log_struct);

            while (!configuration_file.eof())
            {
                getline(configuration_file, buf, '[');
                if (buf == log)
                {
                    getline(configuration_file, buf);

                    flag = 0;
                    break;
                }

                getline(configuration_file, buf, ']');
                getline(configuration_file, buf);
            }

            break;
        }

        getline(configuration_file, buf, '}');
        getline(configuration_file, buf);
    }

    if (!flag)
    {
        while (!configuration_file.eof() && buf != "]")
        {
            getline(configuration_file, buf, '"');
            if (buf[0] == ']')
            {
                break;
            }
            getline(configuration_file, buf, '"');
            getline(configuration_file, severity_str);

            severity = get_severity(severity_str);

            if (buf == "console")
            {
                add_console_stream(severity);
            }
            else
            {
                add_file_stream(buf, severity);
            }
        }
    }
    else
    {
        throw std::runtime_error("Invalid configuration path");
    }
}

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