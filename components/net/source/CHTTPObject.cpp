/**
 *  @file CHTTPObject.cpp
 *  @brief Source file implementing the base HTTP functionality.
 *
 *  This software is licensed under the Draconic Free License version 1. Please refer
 *  to LICENSE.txt for more information.
 *
 *  @author Robert MacGregor
 *  @copyright (c) 2016 Draconic Entity
 */

#include <iostream>
#include <cstring>

#include <net/CHTTPObject.hpp>

namespace Kiaro
{
    namespace Net
    {
        size_t CHTTPObject::curlReadCallback(void* outputPointer, size_t blockSize, size_t blockCount, void* userPointer)
        {
            HTTPTransferContext* context = reinterpret_cast<HTTPTransferContext*>(userPointer);

            const size_t bytesToWrite = blockSize * blockCount;

            // No bytes left to write to or no bytes left to read
            if (!bytesToWrite || !context->mPOSTPayloadLengthRemaining)
                return 0;

            const size_t bytesWritten = context->mPOSTPayloadLengthRemaining < bytesToWrite ? context->mPOSTPayloadLengthRemaining : bytesToWrite;

            memcpy(outputPointer, context->mPOSTPayload, bytesWritten);
            context->mPOSTPayloadLengthRemaining -= bytesWritten;

            return bytesWritten;
        }

        size_t CHTTPObject::curlWriteCallback(void* inputPointer, size_t blockSize, size_t blockCount, void* userPointer)
        {
            static_assert(sizeof(size_t) == sizeof(void*), "sizeof(size_t) must be sizeof(void*) for this code to work correctly! (This is a bug.)");

            HTTPTransferContext* context = reinterpret_cast<HTTPTransferContext*>(userPointer);

            const size_t bytesWritten = blockSize * blockCount;
            const size_t newSize = context->mResponseLength + bytesWritten;

            context->mResponse = realloc(context->mResponse, newSize);
            void* outputPointer = reinterpret_cast<void*>(reinterpret_cast<size_t>(context->mResponse) + context->mResponseLength);

            memcpy(outputPointer, inputPointer, bytesWritten);
            context->mResponseLength = newSize;
        }

        CHTTPObject::CHTTPObject(void)
        {
            // Zero out all of the context memory
            memset(&mHTTPTransferContext, 0x00, sizeof(HTTPTransferContext));
        }

        CHTTPObject::~CHTTPObject(void)
        {
            if (mHTTPTransferContext.mResponse)
            {
                free(mHTTPTransferContext.mResponse);
                mHTTPTransferContext.mResponse = nullptr;
            }
        }

        void CHTTPObject::setURL(const Support::String& url)
        {
            mURL = url;
        }

        const Support::String& CHTTPObject::getURL(void)
        {
            return mURL;
        }

        void CHTTPObject::clearRequestHeaders()
        {
            mRequestHeaders.clear();
        }

        void CHTTPObject::setUserAgent(const Support::String& agent)
        {
            mUserAgent = agent;
        }

        void CHTTPObject::setRequestHeader(const Support::String& headerName, const Support::String& value)
        {
            mRequestHeaders[headerName] = value;
        }

        void CHTTPObject::removeRequestHeader(const Support::String& headerName)
        {
            auto search = mRequestHeaders.find(headerName);

            if (search != mRequestHeaders.end())
                mRequestHeaders.erase(search);
        }

        void CHTTPObject::dispatchRequest(void)
        {
            CURL* curl = curl_easy_init();
            curl_easy_setopt(curl, CURLOPT_URL, mURL.data());

            // Initialize the response section
            if (mHTTPTransferContext.mResponse)
                free(mHTTPTransferContext.mResponse);

            // We just use realloc on this anyway
            mHTTPTransferContext.mResponse = malloc(1);

            // Set the request type
            switch (mRequestType)
            {
                case HTTP_REQUEST_GET:
                {
                    curl_easy_setopt(curl, CURLOPT_POST, 0);
                    break;
                }

                case HTTP_REQUEST_POST:
                {
                    curl_easy_setopt(curl, CURLOPT_POST, 1);
                    break;
                }

                default:
                {
                    throw std::out_of_range("Unexpected request type!");
                }
            }

            // Assemble headers
            curl_slist* requestHeaders = nullptr;
            for (auto it = mRequestHeaders.begin(); it != mRequestHeaders.end(); ++it)
            {
                Support::String header = (*it).first + ": ";
                header += (*it).second;

                requestHeaders = curl_slist_append(requestHeaders, header.data());
            }

            if (requestHeaders)
                curl_easy_setopt(curl, CURLOPT_HTTPHEADER, requestHeaders);

            // Setup the data transfer if necessary
            if (mHTTPTransferContext.mPOSTPayload && mRequestType == HTTP_REQUEST_POST)
            {
                mHTTPTransferContext.mPOSTPayloadLengthRemaining = mHTTPTransferContext.mPOSTPayloadLength;

                curl_easy_setopt(curl, CURLOPT_READDATA, &mHTTPTransferContext);
                curl_easy_setopt(curl, CURLOPT_READFUNCTION, curlReadCallback);
            }

            // Setup the data receive
            curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, curlWriteCallback);
            curl_easy_setopt(curl, CURLOPT_WRITEDATA, &mHTTPTransferContext);

            // If we're using SSL protocols, verify everything
            curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, 1);
            curl_easy_setopt(curl, CURLOPT_SSL_VERIFYHOST, 1);

            // Set the user agent if we have one set
            if (mUserAgent != "")
                curl_easy_setopt(curl, CURLOPT_USERAGENT, mUserAgent.data());

            // Finally do something
            curl_easy_perform(curl);

            curl_easy_cleanup(curl);
            curl_slist_free_all(requestHeaders);
        }

        void CHTTPObject::setPOSTPayload(void* payload, size_t payloadLength)
        {
            mHTTPTransferContext.mPOSTPayload = payload;
            mHTTPTransferContext.mPOSTPayloadLength = payloadLength;
        }

        void* CHTTPObject::getResponseBody(void)
        {
            return mHTTPTransferContext.mResponse;
        }

        size_t CHTTPObject::getResponseBodyLength(void)
        {
            return mHTTPTransferContext.mResponseLength;
        }
    } // End NameSpace Net
} // End NameSpace Kiaro

