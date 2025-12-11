#pragma once

struct Task
{
    int id_num;
    std::string title;
    int completed;

    Task(int i, std::string s, int c)
        : id_num(i), title(s), completed(c) 
    {
    }
};