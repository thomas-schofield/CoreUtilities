#pragma once

#include <iostream>

class Header
{
public:
  int id;

  friend std::ostream& operator<<(std::ostream& os, const Header& header);
};

std::ostream& operator<<(std::ostream& os, const Header& header)
{
    os << "Header: " << header.id << "\n";
    return os;
}
