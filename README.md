# Overview
in "src" folder, the main program in development to exploit ESP-NOW communication is called "attacker" file
extra info about files located in "test" folder:
- sniffer: ESP-32 code that only sniffs ESP-NOW packets
- broadcast: ESP-32 code that broadcasts message with ESP-NOW
- sender: ESP-32 code that sends message to a single peer with ESP-NOW
- receiver: ESP-32 code that only receive messages from ESP-NOW transmissions
- SecureESPNOW: code that sends encrypted messages using a 7 bytes encryption key and AES-128 encryption
- SecureReceiver: code that properly decrypts messages using the same encryption key as the sender

# Purpose
This project is my personal project aimed to intercept unencrypted and encrypted messages, showing the importance of message encryption in wireless communication

# Usage
Using at least 3 ESP32s, the methods to conduct the tests will be explained in the following section. Note that keywords from the Overview section will be used to describe which ESP32 program was used to conduct the tests.

## Unencryped Messages
2 ESP32 running either the broadcast or the sender program is powered up together with a receiver ESP32. A third ESP32 which intercepts this unencrypted message called a sniffer is powered up and it will print in the console the messages that it captured while the other 2 ESP32s are exchanging messages.

## Encrypted Messages
A significant change happens here when 2 ESP32 running the SecureESPNOW and SecureReceiver program is powered up and exchanges messages. Since these two ESP32s are end-to-end encrypted, the sniffer is now unable to capture any messages and the best that it can sometimes capture are gibberish data which is the encrypted message.