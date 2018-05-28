/**************************************************************************
   Copyright (c) 2017 sewenew

   Licensed under the Apache License, Version 2.0 (the "License");
   you may not use this file except in compliance with the License.
   You may obtain a copy of the License at

       http://www.apache.org/licenses/LICENSE-2.0

   Unless required by applicable law or agreed to in writing, software
   distributed under the License is distributed on an "AS IS" BASIS,
   WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
   See the License for the specific language governing permissions and
   limitations under the License.
 *************************************************************************/

#ifndef SEWENEW_REDISPLUSPLUS_TEST_UTILS_H
#define SEWENEW_REDISPLUSPLUS_TEST_UTILS_H

#include <string>
#include <vector>
#include <sw/redis++/redis++.h>

#define REDIS_ASSERT(condition, msg) \
    sw::redis::test::redis_assert((condition), (msg), __FILE__, __LINE__)

namespace sw {

namespace redis {

namespace test {

inline void redis_assert(bool condition,
                            const std::string &msg,
                            const std::string &file,
                            int line) {
    if (!condition) {
        auto err_msg = "ASSERT: " + msg + ". " + file + ":" + std::to_string(line);
        throw Error(err_msg);
    }
}

inline std::string test_key(const std::string &k) {
    return "sw::redis::test::" + k;
}

class KeyDeleter {
public:
    template <typename Input>
    KeyDeleter(Redis &redis, Input first, Input last) : _redis(redis), _keys(first, last) {
        _delete();
    }

    KeyDeleter(Redis &redis, std::initializer_list<std::string> il) :
                KeyDeleter(redis, il.begin(), il.end()) {}

    KeyDeleter(Redis &redis, const std::string &key) : KeyDeleter(redis, {key}) {}

    ~KeyDeleter() {
        _delete();
    }

private:
    void _delete() {
        if (!_keys.empty()) {
            _redis.del(_keys.begin(), _keys.end());
        }
    }

    Redis &_redis;
    std::vector<std::string> _keys;
};

}

}

}

#endif // end SEWENEW_REDISPLUSPLUS_TEST_UTILS_H
