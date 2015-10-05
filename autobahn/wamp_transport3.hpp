class wamp_transport
{
public:
    /*!
     * Attaches a handler to the transport. Only one handler may
     * be attached at any given time.
     *
     * @param handler The handler to attach to this transport.
     */
     virtual void set_transport_handler(const std::shared_ptr<wamp_transport_handler>& handler) = 0;

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
    virtual void send_message(const std::shared_ptr<wamp_message>& message) = 0;

    /*!
     * Pause receiving messages on the attached transport handler. This is
     * used by transport handlers to excert backpressure on the sending peer.
     */
    virtual void pause_receiving() = 0;

    /*!
     * Resume receiving messages on the attached transport handler. This is
     * used by transport handlers to signal readiness to consume more messages
     * and lift backpressure from the sending peer.
     */
    virtual void resume_receiving() = 0;
};


class wamp_transport_handler
{
public:
    /*!
     * Called by the transport when attaching a handler.
     *
     * @param transport The transport being attached to.
     */
    virtual void on_attach(const std::shared_ptr<wamp_transport>& transport) = 0;

    /*!
     * Called by the transport when a message is received.
     *
     * @param message The message that has been received.
     */
    virtual void on_message_received(const wamp_message& message) = 0;

    /*!
     * Called by the transport when outgoing buffers are filling up (have reached
     * the high watermark of buffered data) and this transport handler should (temporarily)
     * stop sending messages.
     */
    virtual void pause_sending() = 0;

    /*!
     * Called by the transport when outgoing buffers are getting empty (have reached
     * the low watermark of buffered data) and this transport handler may resume
     * sending messages.
     */
    virtual void resume_sending() = 0;
};
