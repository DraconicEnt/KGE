/**
 *  @file InputListenerSingleton.hpp
 *  @brief Include file defining the Server and related functions.
 *
 *  This software is licensed under the GNU Lesser General Public License version 3.
 *  Please refer to gpl.txt and lgpl.txt for more information.
 *
 *  @author Draconic Entertainment
 *  @version 0.1.0
 *  @date 12/23/2013
 *  @copyright (c) 2013 Draconic Entertainment
 */

#ifndef _INCLUDE_KIARO_ENGINE_INPUTLISTENERSINGLETON_HPP_
#define _INCLUDE_KIARO_ENGINE_INPUTLISTENERSINGLETON_HPP_

#include <engine/Common.hpp>
#include <irrlicht.h>

#include <physfs.h>

namespace Kiaro
{
    namespace Engine
    {
        class InputListenerSingleton : public irr::IEventReceiver
        {
            // Public Methods
            public:
                static InputListenerSingleton *getPointer(void);
                static void destroy(void);

                bool OnEvent(const irr::SEvent& event);

            // Private Methods
            private:
                //! Standard constructor.
                InputListenerSingleton(void);
                //! Standard destructor.
                ~InputListenerSingleton(void);
        };
    } // End Namespace Game
} // End Namespace Kiaro
#endif // _INCLUDE_KIARO_ENGINE_INPUTLISTENERSINGLETON_HPP_
