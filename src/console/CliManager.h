/* Copyright (c) 2018 - present, VE Software Inc. All rights reserved
 *
 * This source code is licensed under Apache 2.0 License
 *  (found in the LICENSE.Apache file in the root directory)
 */

#ifndef CONSOLE_CLIMANAGER_H_
#define CONSOLE_CLIMANAGER_H_

#include "base/Base.h"
#include "console/CmdProcessor.h"

namespace nebula {
namespace graph {

class CliManager final {
public:
    CliManager();
    ~CliManager() = default;

    bool connect(const std::string& addr,
                 uint16_t port,
                 const std::string& username,
                 const std::string& password);

    void batch(const std::string& filename);

    void loop();

    bool readLine(std::string &line, bool linebreak = false);

    void updateHistory(const char *line);

    void saveHistory();

    void loadHistory();

private:
    std::string addr_;
    uint16_t port_;
    std::string username_;

    std::unique_ptr<CmdProcessor> cmdProcessor_;
};

}  // namespace graph
}  // namespace nebula
#endif  // CONSOLE_CLIMANAGER_H_