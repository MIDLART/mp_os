#include "../include/logger_builder.h"

logger::severity logger_builder::string_to_severity(
    std::string const &severity_string)
{
    if (severity_string == "trace")
    {
        return logger::severity::trace;
    }
    if (severity_string == "debug")
    {
        return logger::severity::debug;
    }
    if (severity_string == "information")
    {
        return logger::severity::information;
    }
    if (severity_string == "warning")
    {
        return logger::severity::warning;
    }
    if (severity_string == "error")
    {
        return logger::severity::error;
    }
    if (severity_string == "critical")
    {
        return logger::severity::critical;
    }

    throw std::out_of_range("invalid severity string value");
}

logger::severity logger_builder::get_severity(
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
        case 'D':
            return logger::severity::debug;
        case 'T':
            return logger::severity::trace;
        case 'I':
            return logger::severity::information;
        case 'W':
            return logger::severity::warning;
        case 'C':
            return logger::severity::critical;
        case 'E':
            return logger::severity::error;
        default:
            return logger::severity::information;
    }
}

void logger_builder::get_substr_queue(
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