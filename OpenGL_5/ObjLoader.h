#ifndef OBJLOADER_H_
#define OBJLOADER_H_

#include <vector>
#include "header/Angel.h"

bool LoadObj(const char*  path, std::vector<float> *vertices, std::vector<float> *normals, char type);

#endif