///////////////////////////////////////////////////////////////////////////////
//
//  Copyright (C) 2014 Tavendo GmbH
//
//  Licensed under the Apache License, Version 2.0 (the "License");
//  you may not use this file except in compliance with the License.
//  You may obtain a copy of the License at
//
//      http://www.apache.org/licenses/LICENSE-2.0
//
//  Unless required by applicable law or agreed to in writing, software
//  distributed under the License is distributed on an "AS IS" BASIS,
//  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
//  See the License for the specific language governing permissions and
//  limitations under the License.
//
///////////////////////////////////////////////////////////////////////////////

#ifndef AUTOBAHN_WAMP_TRANSPORT_HPP
#define AUTOBAHN_WAMP_TRANSPORT_HPP

// http://stackoverflow.com/questions/22597948/using-boostfuture-with-then-continuations/
#define BOOST_THREAD_PROVIDES_FUTURE
#define BOOST_THREAD_PROVIDES_FUTURE_CONTINUATION
#define BOOST_THREAD_PROVIDES_FUTURE_WHEN_ALL_WHEN_ANY

#include <boost/thread/future.hpp>
#include <memory>
#include <string>

namespace autobahn {

class wamp_message;
class wamp_transport_handler;

/*!
 * Provides an abstraction for a transport to be used by the session. A wamp
 * transport is defined as being message based, bidirectional, reliable, and
 * ordered.
 */
class wamp_transport
{
public:
    /*!
     * Send the given WAMP message over the transport to the peer.
     *
     * @param message The message to be sent.
     */
    virtual void send(const std::shared_ptr<wamp_message>& message) = 0;

    /*!
     * Pause receiving messages and firing "on_message" events. This is
     * used to excert backpressure on the sending peer.
     */
    virtual void pause_receiving() = 0;

    /*!
     * Resume receiving messages and firing "on_message" events. This is
     * used to signal readiness to consume more messages and lift backpressure
     * from the sending peer.
     */
    virtual void resume_receiving() = 0;
};

} // namespace autobahn

#endif // AUTOBAHN_WAMP_TRANSPORT_HPP
