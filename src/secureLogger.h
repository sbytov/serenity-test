#pragma once
#include "cryptlib.h"
#include "hex.h"
#include "sha.h"
#include "hmac.h"
#include "filesystem"
#include <fstream>
#include <iostream>
using namespace CryptoPP;

class SecureLogger sealed {
private:
    std::ofstream logOut;
    std::ofstream sessionsOut;
    HMAC<SHA1> hmac;
    static const size_t DIGEST_SIZE = HMAC<SHA1>::DIGESTSIZE;
    void validateLog(const std::string& logFile, const std::string& sessionFile)
    {
        std::ifstream log(logFile);
        std::ifstream sessions(sessionFile, std::ios::in | std::ios::binary);
        if (!log.good() | !sessions.good())
        {
            throw std::runtime_error("failed to open log or session file for input");
        }
        std::string line;
        size_t lineNum = 0;
        bool started = false;
        while (std::getline(log, line))
        {
            if (line == "[Start session]")
            {
                if (!started)
                {
                    started = true;
                    hmac.Restart();
                }
                else
                {
                    throw std::runtime_error("log is malformed");
                }
            }
            else if (line == "[Close session]")
            {
                if (started)
                {
                    started = false;
                    byte stored[DIGEST_SIZE];
                    sessions.read((char*)stored, DIGEST_SIZE);
                    byte current[DIGEST_SIZE];
                    hmac.Final(current);
                    if (memcmp(stored, current, DIGEST_SIZE) != 0)
                    {
                        throw std::runtime_error(std::string("digest mismatch at line ") + std::to_string(lineNum));
                    }
                }
                else
                {
                    throw std::runtime_error("log is malformed");
                }
            }
            else
            {
                hmac.Update((const byte*)line.data(), line.size());
            }
            lineNum++;
        }
        log.close();
        sessions.close();
    }

public:
    SecureLogger(const std::string& logFile, const std::string& password, const std::string& sessionFile)
        : hmac{ (const byte*)password.data(), password.size() }
    {
        if (std::experimental::filesystem::exists(logFile))
        {
            validateLog(logFile, sessionFile);
        }

        logOut.open(logFile, std::ios::out | std::ios::ate | std::ios::app);
        sessionsOut.open(sessionFile, std::ios::out | std::ios::binary | std::ios::ate | std::ios::app);

        if (!logOut.good() || !sessionsOut.good())
        {
            throw std::runtime_error("failed to open log or session file for output");
        }
        logOut << "[Start session]\n";
        hmac.Restart();
    }

    void Log(const std::string& message) {
        logOut << message << "\n";
        hmac.Update((const byte*)message.data(), message.size());
    }

    ~SecureLogger() {
        byte current[DIGEST_SIZE];
        hmac.Final(current);
        sessionsOut.write((const char*)current, DIGEST_SIZE);
        logOut << "[Close session]\n";
    }

};