# IoT Portfolio Peer-Informed Report
**Module:** UFCFVK-15-2 Internet of Things  
**Student:** Arda Pekar  
**Student ID:** 24061895

---

## Activity 3.1: Analysis of the eBike IoT System

### Aspect 1: Connectivity & Networking Perspective — Protocol Choice

The eBike IoT system developed in the worksheets uses UDP (User Datagram Protocol) at the transport layer for communication between eBike clients and the gateway. This choice has direct implications for the system's behaviour. UDP is connectionless and does not guarantee delivery, ordering, or error checking, which means a DATA packet containing GPS coordinates could be lost without the gateway being notified. In the implemented solution, the gateway sends an acknowledgement back to the client after each DATA message, partially compensating for UDP's unreliability. However, this is a custom mechanism rather than a built-in protocol guarantee.

In a real-world e-bike fleet system such as Lime or Santander Cycles, this trade-off is significant. GPS location data is time-sensitive — a delayed or lost packet is less harmful than a delayed acknowledgement causing the client to retry and flood the network. UDP's low overhead suits constrained on-board computers with limited bandwidth. However, for critical operations such as lock/unlock commands, UDP without a robust retry mechanism poses a reliability risk. In the implemented system, a COMMAND message could be dropped with no retry, leaving the eBike in the wrong state. A real deployment would likely use MQTT over TCP for command messages, which provides guaranteed delivery via QoS levels, while keeping UDP for high-frequency GPS telemetry.

### Aspect 2: Security & Privacy Perspective — Device Authentication

The implemented eBike system has no device authentication mechanism. Any process that knows the gateway's IP and port can send JOIN or DATA messages, and the gateway will accept them without verification. In the worksheets, the eBike client simply sends a JOIN message with an ebike_id field, which the gateway trusts unconditionally. There is also no encryption — all GPS coordinates, lock status, and timestamps are transmitted in plaintext JSON over the emulated network.

In a real-world deployment, this would be a critical vulnerability. An attacker could inject fake eBike location data, send false COMMAND messages to lock legitimate bikes, or monitor GPS data to track users' movements, raising serious GDPR concerns around location data as personal data. Commercial IoT fleets address this through mutual TLS authentication, where each device holds a unique certificate, and data is encrypted in transit. The OWASP IoT Top 10 lists insecure network services and lack of secure update mechanisms as primary risks. The current implementation prioritises simplicity and functionality for simulation purposes, but a production system would require a secure boot process, device certificates, and encrypted channels as a minimum baseline.

---

## Activity 3.2: Peer-Informed Reflection

After completing the analysis above, I discussed it with two classmates: Bertun Alpaydin and Cem Basoglu.

Bertun noted that my analysis of UDP was strong in linking the protocol choice to the specific implementation, but suggested I could deepen the critique by referencing specific IoT protocol standards such as CoAP, which was covered in the module lectures and is specifically designed for constrained IoT devices using UDP with optional reliability features. He pointed out that CoAP's confirmable messages would directly address the reliability gap I identified, making it a more precise real-world alternative than MQTT for this use case. I found this feedback valid — I had mentioned MQTT as an alternative but CoAP is actually a closer architectural match for the UDP-based simulated network.

Cem focused on the security analysis and suggested including a concrete reference to GDPR Article 4, which defines location data as personal data when it can identify an individual, strengthening the privacy argument. He also noted that I could mention the specific threat of replay attacks — where an attacker captures and resends valid JOIN packets to impersonate a registered eBike.

In giving feedback to my peers, I suggested that their analyses would benefit from more direct links to their own code rather than general IoT descriptions, and encouraged them to reference specific lines or components of their implementation.

Reflecting on the exchange, I would revisit the security section to incorporate the GDPR reference and replay attack scenario. The discussion confirmed that connecting module content — specifically the CoAP lecture — to the implementation analysis produces a stronger critical argument. The peer feedback process helped me identify gaps between describing what the system does versus critically evaluating why those design choices matter in context.

*Peers discussed with: Bertun Alpaydin, Cem Basoglu*