in "src" folder, the main program in development to exploit ESP-NOW communication is called "attacker" file
extra info about files located in "test" folder:
- sniffer: ESP-32 code that only sniffs ESP-NOW packets
- broadcast: ESP-32 code that broadcasts message with ESP-NOW
- sender: ESP-32 code that sends message to a single peer with ESP-NOW
- receiver: ESP-32 code that only receive messages from ESP-NOW transmissions
- SecureESPNOW: code that sends encrypted messages using a 7 bytes encryption key and AES-128 encryption
- SecureReceiver: code that properly decrypts messages using the same encryption key as the sender