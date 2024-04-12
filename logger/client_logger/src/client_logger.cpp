#include <fstream>

#include <not_implemented.h>

#include "../include/client_logger.h"

std::unordered_map<std::string, std::pair<std::ostream *, size_t>> client_logger::_all_streams;

client_logger::client_logger(
        std::map<std::string, std::set<logger::severity>> const &data,
        std::string log_struct):
        _log_struct(std::move(log_struct))
{

    for (auto iter = data.begin(); iter != data.end(); ++iter)
    {
        const auto &log_elem = *iter;

        if (log_elem.first[0] == '\0')
        {
            _streams[""] = std::make_pair(&std::cout, log_elem.second);
        }
        else
        {
            auto elem_pair = _all_streams[log_elem.first];

            if (!elem_pair.second)
            {
                std::ofstream *new_stream = nullptr;

                try
                {
                    new_stream = new std::ofstream(log_elem.first);

                    if (!new_stream->is_open())
                    {
                        throw std::runtime_error("File cannot be opened");
                    }
                }
                catch (std::exception const &)
                {
                    delete new_stream;

                    for (auto del_iter = data.begin(); del_iter != iter; ++del_iter)
                    {
                        decrement_stream(del_iter->first);
                    }

                    throw;
                }


                elem_pair = std::move(std::make_pair(new_stream, size_t(0)));
            }

            elem_pair.second++;
            _streams[log_elem.first] = std::make_pair(elem_pair.first, log_elem.second);
        }
    }
}

client_logger::client_logger(
        client_logger const &other):
        _log_struct(other._log_struct),
        _streams(other._streams)
{
    for (auto &stream : _streams)
    {
        _all_streams[stream.first].second++;
    }
}

client_logger &client_logger::operator=(
        client_logger const &other)
{
    if (this != &other)
    {
        for (auto &log_stream: _streams) {
            decrement_stream(log_stream.first);
        }

        _streams = other._streams;
        _log_struct = other._log_struct;

        for (auto &log_stream: _streams) {
            _all_streams[log_stream.first].second++;
        }
    }

    return *this;
}

client_logger::client_logger(
        client_logger &&other) noexcept:
        _log_struct(std::move(other._log_struct)),
        _streams(std::move(other._streams))
{ }

client_logger &client_logger::operator=(
        client_logger &&other) noexcept
{
    if (this != &other)
    {
        for (auto &log_stream : _streams)
        {
            decrement_stream(log_stream.first);
        }

        _log_struct = std::move(other._log_struct);
        _streams = std::move(other._streams);
    }

    return *this;
}

client_logger::~client_logger() noexcept
{
    for (auto &log_stream : _streams)
    {
        decrement_stream(log_stream.first);
    }
}

std::string client_logger::log_string_parse (
        const std::string &text,
        logger::severity severity) const
{
    std::string res;
    size_t size = _log_struct.size();

    for (size_t i = 0; i < size; ++i)
    {
        if (_log_struct[i] == '%' && i < size - 1)
        {
            ++i;
            switch (_log_struct[i])
            {
                case 'd':
                    res += '[' + current_date_to_string() + "] ";
                    break;
                case 't':
                    res += '[' + current_time_to_string() + "] ";
                    break;
                case 's':
                    res += '[' + severity_to_string(severity) + "] ";
                    break;
                case 'm':
                    res += text + ' ';
                    break;
                default:
                    res += '%' + _log_struct[i];
                    break;
            }
        }
        else
        {
            res += _log_struct[i];
        }
    }

    return res;
}

logger const *client_logger::log(
    const std::string &text,
    logger::severity severity) const noexcept
{
    std::string out_string = log_string_parse(text, severity);

    for (auto &cur_stream : _streams)
    {
        std::ostream &stream = *(cur_stream.second.first);
        auto &severities = cur_stream.second.second;

        if (severities.count(severity))
        {
            stream << out_string << std::endl;
        }
    }

    return this;
}

void client_logger::decrement_stream(std::string const &file_path) const noexcept
{
    if (file_path[0] != '\0')
    {
        auto iter = _all_streams[file_path];

        --(iter.second);

        if (iter.second == 0)
        {
            iter.first->flush();
            delete iter.first;

            _all_streams.erase(file_path);
        }
    }
}