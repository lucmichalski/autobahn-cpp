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
     * Attaches a handler to the transport. Only one handler may
     * be attached at any given time.
     *
     * @param handler The handler to attach to this transport.
     */
    virtual void attach(const std::shared_ptr<wamp_transport_handler>& handler) = 0;

    /*!
     * Detaches the handler currently attached to the transport.
     */
    virtual void detach() = 0;

    /*!
     * Determines if the transport has a handler attached.
     *
     * @return Whether or not a handler is attached.
     */
    virtual bool has_handler() const = 0;

    /*!
     * Send the given WAMP message over the transport to the peer.
     *
     * This method does _not_ block. An implementation of this method will
     * usually serialize the WAMP message to a byte string, frame the bytes
     * according to the WAMP transport framing in place (e.g. WebSocket or
     * RawSocket) and then buffer and arrange for asynchronous sending of
     * the bytes, but return immediately.
     *
     * Note that when you first send message 1 and then message 2, _and_
     * message 2 was indeed received by the peer, it is guaranteed that
     * a) message 1 was also received, and b) that the order of receiving
     * first message 1 and then 2 is preserved. Hence a transport guarantees
     * strict message ordering and a slightly weaker form of reliability.
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

    /*!
     * Default virtual destructor.
     */
    virtual ~wamp_transport_handler() = default;
};

} // namespace autobahn

#endif // AUTOBAHN_WAMP_TRANSPORT_HPP
