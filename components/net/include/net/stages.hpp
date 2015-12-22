/**
 *  @file stages.hpp
 */

#ifndef _INCLUDE_NET_STAGES_HPP_
#define _INCLUDE_NET_STAGES_HPP_

namespace Kiaro
{
    namespace Net
    {
        /**
         *  @brief The netcode implementation calls for client states called "stages" which primarily controls
         *  what types of messages can be sent by clients and received from servers.
         */
        enum STAGE_NAME
        {
            //! The authentication stage is used solely for messages regarding the initial connection and client/server authentication.
            STAGE_AUTHENTICATION = 0,
            //! The loading state is used for messages regarding the loading of game assets and transferring scene information.
            STAGE_LOADING = 1,
            //! The gameplay state is used for messages regarding the actual synchronization of game simulation state between client and server.
            STAGE_GAMEPLAY = 2,
        };
    } // End NameSpace Net
} // End NameSpace Kiaro

#endif // _INCLUDE_NET_STAGES_HPP_
