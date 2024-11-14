#ifndef SAP_UTILS_H
#define SAP_UTILS_H

#include <string>
#include "nwrfcsdk/include/sapnwrfc.h"

// Convert UTF-8 string to SAP Unicode format
bool convertToSapUnicode(const std::string& input, SAP_UC** output, unsigned& outputLen, RFC_ERROR_INFO& errorInfo) {
    unsigned resultLen = 0;
    RFC_RC rc = RfcUTF8ToSAPUC((RFC_BYTE*)input.c_str(), input.length(), *output, &outputLen, &resultLen, &errorInfo);
    if (rc == RFC_BUFFER_TOO_SMALL) {
        *output = (SAP_UC*)reallocU(*output, outputLen);
        rc = RfcUTF8ToSAPUC((RFC_BYTE*)input.c_str(), input.length(), *output, &outputLen, &resultLen, &errorInfo);
    }
    if (rc != RFC_OK || *output == nullptr) {
        return false;
    }
    return true;
}

// Print SAP RFC error message
void printSapRfcError(const RFC_ERROR_INFO& errorInfo) {
    std::cout << "Login PROBLEM" << std::endl;
    std::cout << "key# " << errorInfo.key << ": " << errorInfo.code << std::endl;
    std::cout << "message# " << errorInfo.message << std::endl;
    std::cout << "code# " << errorInfo.code << std::endl;
}

#endif // SAP_UTILS_H



// Format system number (SYSNR)
std::string formatSystemNumber(const std::string& sysnr) {
    std::string formatted = sysnr;

    // Remove leading/trailing whitespace
    formatted.erase(0, formatted.find_first_not_of(" \t\r\n"));
    formatted.erase(formatted.find_last_not_of(" \t\r\n") + 1);

    // Ensure it's numeric
    if (!std::all_of(formatted.begin(), formatted.end(), ::isdigit)) {
        throw std::runtime_error("SYSNR must contain only digits");
    }

    // Pad with leading zero if necessary
    if (formatted.length() == 1) {
        formatted = "0" + formatted;
    }

    // Verify length is exactly 2
    if (formatted.length() != 2) {
        throw std::runtime_error("SYSNR must be exactly two digits");
    }

    return formatted;
}

#endif // SAP_UTILS_H