#include "Connection.hpp"

#include <boost/algorithm/string.hpp>
#include <chrono>
#include <fstream>
#include <thread>
#include <utility>

namespace tcpdae {

constexpr char Connection::kClassName[];

// ==================== PUBLIC ====================

void Connection::start() {
    syslog(LOG_DEBUG, "%s::%s", kClassName, __func__);
    processRequest();
}

// ==================== PRIVATE ====================

void Connection::processRequest() {
    auto self = shared_from_this();
    boost::asio::async_read_until(
        mSocket, mRequest, "\n", [this, self](const boost::system::error_code ec, const size_t) {
            if (!ec || ec == boost::asio::error::eof) {
                // convert request stream to std string
                std::string inp((std::istreambuf_iterator<char>(&mRequest)), std::istreambuf_iterator<char>());
                // remove "\n" from string
                if (inp.back() == '\n') {
                    inp = inp.substr(0, inp.size() - 1);
                }
                const std::string ret = doCommand(inp);
                syslog(LOG_DEBUG, "Sending message %s to request %s", ret.c_str(), inp.c_str());
                // send data
                const std::string resp = ret + "\n";
                boost::asio::write(mSocket, boost::asio::buffer(resp));
                syslog(LOG_DEBUG, "Data sent");
            }
        });
}

std::string Connection::doCommand(const std::string& cmd) const {
    syslog(LOG_INFO, "Executing command : %s", cmd.c_str());
    if (cmd == "cpu") {
        return getCpu();
    } else if (cmd == "mem") {
        return getMem();
    } else {
        return "Invalid command";
    }
}

/// @brief returns jiffies - first total, second work
static inline std::pair<size_t, size_t> getJiffies() {
    // path to statfile and total jiffy count
    static constexpr char kStatPath[]{"/proc/stat"};
    static constexpr size_t kJiffyCnt{7};
    size_t jiffies[kJiffyCnt];
    std::ifstream f(kStatPath);
    // this should be "cpu, ignore it"
    std::string temp;
    f >> temp;
    for (size_t i = 0; i < kJiffyCnt; i++) {
        f >> jiffies[i];
    }
    // calculate work and total jiffies
    static constexpr size_t workJifCnt = 3;
    size_t workJif = 0;
    size_t totalJif = 0;
    for (size_t i = 0; i < kJiffyCnt; i++) {
        // first three are work jiffies
        if (i < workJifCnt) {
            workJif += jiffies[i];
        }
        totalJif += jiffies[i];
    }
    return {totalJif, workJif};
}

std::string Connection::getCpu() const {
    // delta between readings
    static constexpr std::chrono::milliseconds kDelta{1000};
    const auto jif1 = getJiffies();
    std::this_thread::sleep_for(kDelta);
    const auto jif2 = getJiffies();
    const float total = static_cast<float>(jif2.first - jif1.first);
    const float work = static_cast<float>(jif2.second - jif1.second);
    const float cpu = (work / total) * 100.0f;
    return std::to_string(cpu) + "%";
}

std::string Connection::getMem() const {
    // path to meminfo, how many measurements we need to calculate
    static constexpr char kMemPath[]{"/proc/meminfo"};
    static constexpr char kError[]{"Error while reading memory"};
    static constexpr size_t kMeasNeeded{4};
    size_t memTotal, memFree, buffers, cached;
    size_t meas{0};
    std::string tok;
    std::ifstream f(kMemPath);
    // get all four readings that we need to calculate
    while (f >> tok) {
        if (tok == "MemTotal:") {
            if (f >> memTotal)
                meas++;
            else
                return kError;
        } else if (tok == "MemFree:") {
            if (f >> memFree)
                meas++;
            else
                return kError;
        } else if (tok == "Buffers:") {
            if (f >> buffers)
                meas++;
            else
                return kError;
        } else if (tok == "Cached:") {
            if (f >> cached)
                meas++;
            else
                return kError;
        }
        if (meas == kMeasNeeded) break;
    }
    const size_t ret{memTotal - memFree - buffers - cached};
    return std::to_string(ret) + " kB";
}

}  // namespace tcpdae
