/**
 *  @file CHTTPObject.hpp
 *  @brief Include file declaring shared HTTP object components.
 *
 *  This software is licensed under the Draconic Free License version 1. Please refer
 *  to LICENSE.txt for more information.
 *
 *  @author Robert MacGregor
 *  @copyright (c) 2016 Draconic Entity
 */

#include <curl/curl.h>

#include <support/String.hpp>
#include <support/UnorderedMap.hpp>

namespace Kiaro
{
    namespace Net
    {
        /**
         *  @brief A base class for the HTTP and HTTPS classes to derive shared attributes and functionality from.
         */
        class CHTTPObject
        {
            // Public Members
            public:
                //! An enumeration representing the supported HTTP request types.
                enum HTTP_REQUEST_TYPE
                {
                    //! A regular HTTP GET.
                    HTTP_REQUEST_GET,

                    //! An HTTP POST request.
                    HTTP_REQUEST_POST,
                };

                //! The type of request that should be issued.
                HTTP_REQUEST_TYPE mRequestType;

            // Private Members
            private:
                //! The URI we will be operating against.
                Support::String mURL;

                //! The user agent to use.
                Support::String mUserAgent;

                //! A map mapping HTTP request header names to their values.
                Support::UnorderedMap<Support::String, Support::String> mRequestHeaders;

                //! A map mapping POST key, value pairs for a URL encoded POST.
                Support::UnorderedMap<Support::String, Support::String> mURLEncodedPOSTValues;

                /**
                 *  @brief The HTTP transfer context to be used when uploading arbitrary data to the web server.
                 */
                struct HTTPTransferContext
                {
                    //! The payload data to submit.
                    void* mPOSTPayload;

                    //! How many bytes are remaining to write.
                    size_t mPOSTPayloadLengthRemaining;

                    //! The total length of the payload.
                    size_t mPOSTPayloadLength;

                    //! The total length of the web server response.
                    size_t mResponseLength;

                    //! The response data from the HTTP server.
                    void* mResponse;
                };

                //! A pointer to the HTTP transfer context in use or to be used.
                HTTPTransferContext mHTTPTransferContext;

            // Public Methods
            public:
                //! Parameter-less constructor to initialize base systems.
                CHTTPObject(void);

                //! Standard destructor.
                ~CHTTPObject(void);

                /**
                 *  @brief Sets the URI that the HTTP object will operate against.
                 *  @param uri The URI to use.
                 */
                void setURL(const Support::String& uri);

                /**
                 *  @brief Retrieves the current URI the HTTP object will operate against.
                 *  @return The currently set URI.
                 */
                const Support::String& getURL(void);

                /**
                 *  @brief Sets the value of a header to be used in the request. If the header already exists, the value is overwritten.
                 *  @param headerName The name of the header to set.
                 *  @param value The value to use.
                 *  @warning Certain header values may be interpreted differently be the internal HTTP implementation (Ie: CURL has special ways to designate
                 *  empty headers and removed headers) but do not rely on this as backing can change at anytime. Use the explicit set/remove methods instead.
                 */
                void setRequestHeader(const Support::String& headerName, const Support::String& value);

                /**
                 *  @brief Removes a header from the request. If the header does not exist, this simly no-ops.
                 *  @param headerName The name of the header to remove.
                 */
                void removeRequestHeader(const Support::String& headerName);

                /**
                 *  @brief Clears all custom headers.
                 */
                void clearRequestHeaders();

                /**
                 *  @brief Sets a new user agent.
                 *  @param agent The new agent to advertise as.
                 */
                void setUserAgent(const Support::String& agent);

                /**
                 *  @brief Sets the POST payload data to send to the server.
                 *  @param payload The payload data to send. If nullptr, then no raw data is uploaded. The CHTTPObject does not claim ownership of this payload data
                 *  when called, it is up to the programming making use of the CHTTPObject programming to manage this memory.
                 *  @param payloadLength The length of the payload. It is absolutely imperative to ensure that this value is correct. If payload is nullptr,
                 *  then this is unused.
                 */
                void setPOSTPayload(void* payload, size_t payloadLength);

                /**
                 *  @brief Dispatches the HTTP request with the current request payload within the context of the current thread. In other words, if you want the
                 *  engine to continue to perform with any semblance of speed, ensure that this is only ever called from within the context of some auxilliary thread
                 *  and not main.
                 */
                void dispatchRequest(void);

                /**
                 *  @brief Gets the server raw response body.
                 *  @return A pointer to the server raw response data.
                 */
                void* getResponseBody(void);

                /**
                 *  @brief Gets the server raw repsonse length.
                 *  @return The server raw response length in bytes.
                 */
                size_t getResponseBodyLength(void);

            // Private Methods
            private:
                /**
                 *  @brief The CURL data read callback to use when sending arbitrary data payloads to the HTTP server.
                 *  @param The output buffer to write to.
                 *  @return The amount of bytes written.
                 */
                static size_t curlReadCallback(void* outputPointer, size_t size, size_t nmemb, void *userPointer);

                /**
                 *  @brief The CURL data write callback to use when receiving arbitrary data payloads from an HTTP server.
                 *  @param inputPointer The memory we are receiving from the web server.
                 */
                static size_t curlWriteCallback(void* inputPointer, size_t size, size_t nmemb, void* userPointer);
        };
    } // End NameSpace Net
} // End NameSpace Kiaro
