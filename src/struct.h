#ifndef MYSTRUCT_H
#define MYSTRUCT_H

#include <assert.h>
#include <cmath>
#include <stdio.h>
#include <stdlib.h>
#include <utility>
#include <malloc.h>
#include <iconv.h>
#include <iostream>
#include <fstream>
#include <string>
#include <set>
#include <time.h>
#include <algorithm>
#include <cstring>
#include <functional>
#include <vector>
#include <map>
#include <dlfcn.h>

#include <sys/types.h>
#include <sys/stat.h>
#include <sys/time.h>
#include <sys/mman.h>

#include "../lib/Config.h"
#include "../lib/Timer.h"
#include "../lib/my_hash_map.h"
#include "../lib/WinnerTree_Linux.h" //ÍâÅÅ
#include "../lib/ReadStructFile.h"   // µ¹ÅÅË÷Òý
#include "../lib/MyGloble.h"
#include "../lib/Logger.h"
#include "../lib/stringUtils.h"

using namespace std;

#ifdef __GNUC__
#define OTL_ODBC_UNIX
#endif

#define OTL_ODBC  //  Compile OTL 4.0/ODBC
#define OTL_STL
#define OTL_ODBC_SELECT_STM_EXECUTE_BEFORE_DESCRIBE

typedef unsigned int UINT;

struct DataNode {
    int user_id;
    int item_id;
    float score;
};

// ¾ØÕóµÄµ¹ÅÅË÷Òý
struct MatrixIndex {
    int count;
    long long offset;
};
struct MatrixInvert {
    int id;
    double score;
};

struct tNode {
    int id;
    float score;
    bool operator < (const struct tNode& b) const {
        return (this->score > b.score);
    }
};
#endif

