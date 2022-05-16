#ifndef CPP_PILOT_HSE_UNARCHIVER_H
#define CPP_PILOT_HSE_UNARCHIVER_H

#include <istream>
#include <string>
#include <vector>

#include "bit_istream.h"

void Unarchive(std::istream& in, std::vector<std::string>& files);

#endif  // CPP_PILOT_HSE_UNARCHIVER_H
