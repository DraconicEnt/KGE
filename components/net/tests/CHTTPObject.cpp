/**
 *  @file CHTTPObject.cpp
 *  @brief Testing code for the CHTTPObject class.
 */

#include <gtest/gtest.h>

#include <support/Console.hpp>
#include <net/CHTTPObject.hpp>

namespace Kiaro
{
    namespace Net
    {
        TEST(CHTTPObject, SimpleGET)
        {
            CHTTPObject connection;
            connection.mRequestType = CHTTPObject::HTTP_REQUEST_GET;
            connection.setURL("https://google.com");
            connection.dispatchRequest();

            const size_t responseLength = connection.getResponseBodyLength();
            Common::C8* response = reinterpret_cast<Common::C8*>(connection.getResponseBody());
            response[responseLength - 1] = 0x00;

            CONSOLE_INFOF("Response Length: %u bytes", responseLength);
            std::cout << response << std::endl;
        }
    } // End NameSpace Net
} // End NameSpace Kiaro
