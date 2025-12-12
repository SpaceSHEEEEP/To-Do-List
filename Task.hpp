#pragma once

struct Task
{
    long id_num;
    std::string title;
    int completed;

    Task(long i, std::string s, int c)
        : id_num(i), title(s), completed(c) 
    {
    }
};