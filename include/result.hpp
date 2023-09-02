#pragma once

#include <string>

class Result
{
public:
    std::string msg;
    void *data;

private:
    bool valid;

public:
    Result() : Result(true, "") {}
    Result(bool _valid, void *_data = NULL) : Result(_valid, "", _data) {}
    Result(const std::string &_msg, void *_data = NULL) : Result(false, _msg, _data) {}
    Result(bool _valid, const std::string &_msg, void *_data = NULL) : valid(_valid), msg(_msg), data(_data) {}

    bool &operator==(const Result &) = delete;
    bool &operator!=(const Result &) = delete;

    operator bool() const
    {
        return valid;
    }
};