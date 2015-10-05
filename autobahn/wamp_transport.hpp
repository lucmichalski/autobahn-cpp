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
    /*
     * SENDER interface
     */

    /*!
     * Send the given WAMP message over the transport to the peer.
     *
     * @param message The message to be sent.
     */
    virtual void send(const std::shared_ptr<wamp_message>& message) = 0;

    /*!
     * Listener type for "on_pause_sending" events.
     */
    typedef std::function<void()> on_pause_sending_t;

    /*!
     * Add a listener to "on_pause_sending" events. The "on_pause_sending" event fires when the
     * high-watermark of the outgoing leg is reached. This happens when the
     * receiving peer (or network link) cannot keep up. Senders should stop sending
     * messages until "on_resume_sending" fires. When senders continue sending messages
     * anyway, messages will stack up in user-space buffers and memory will
     * ultimately run away. This is used to signal backpressure from the receiving
     * peer to user code.
     */
    virtual void on_pause_sending(on_pause_sending_t& listener) = 0;

    /*!
     * Listener type for "on_resume_sending" events.
     */
    typedef std::function<void()> on_resume_sending_t;

    /*!
     * Add a listener to "on_resume_sending" events. The "on_resume_sending" event fires when the
     * low-watermark of the outgoing leg is reached. This happens when the outgoing
     * buffers have drained and the receiver has catched up. Senders now may start
     * sending messages again. This is used to signal the reduction of backpressure
     * from the receiving peer to user code.
     */
    virtual void on_resume_sending(on_resume_sending_t& listener) = 0;


    /*
     * RECEIVER interface
     */

    /*!
     * Listener type for "on_message" events.
     */
    typedef std::function<void(const wamp_message&)> on_message_t;

    /*!
     * Add a listener to "on_message" events.
     */
    virtual void on_message(on_message_t& listener) = 0;

    /*!
     * Pause receiving messages and firing "on_message" events. This is
     * used to exert backpressure on the sending peer.
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
