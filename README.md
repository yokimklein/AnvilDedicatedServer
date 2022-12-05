# AnvilDedicatedServer
A WIP modification for Halo Online 11.1.604673 cert_ms29 which restores missing networking functionality for server hosting.<br/>

Part of the [Anvil Station](https://discord.gg/hetx7ekZJQ) project, which aims to recreate the original Halo Online API and game servers.<br/>

**We're looking for new developers** to help us with the reverse engineering efforts. If you're competent with IDA & C++, [come talk to us!](https://discord.gg/hetx7ekZJQ)

# Notice
**This is currently incomplete.**<br/>
Clients can join the server and load into maps, but they will be softlocked on the pre-game camera for the duration of the game.<br/>
The host is able to play the game mostly normally however and see other clients in game.<br/><br/>
We don't currently have a way for clients to attempt connections outside of our closed source API. If you want to do this, you will have to call hf2p_join_game manually.
```cpp
// located at base pointer + 0x319880
void __fastcall hf2p_join_game(long ip_address, short port, s_transport_secure_identifier session_id, s_transport_secure_address host_address)
```

# Simulation issues
The missing code responsible for handling clients connecting to & joining the server has been fully reimplemented.<br/>
There is still missing code for sending simulation updates, so games aren't currently playable for clients. For the host, some weapons don't fire and some effects don't appear.<br/>

# Usage
1) Build hf2p_dedicated_server.dll & hf2p_launcher.exe
2) Place it in the same folder as halo_online.exe
3) Run the launcher to inject the dll
4) Open the console window that appears
5) Wait for the game to load.
6) The server IP, secure address & secure ID required for clients to connect should be automatically printed to the console
7) Wait for the game start status to print 'ready' in the console
8) Press INSERT to set player data, then HOME to launch into the game

# Controls
- INSERT: Sets test player data for all players
- HOME: Launches into a game
- END: Ends the game

# Special thanks
- @theTwist84 for reversing a lot of the game's structs and allowing me to use code from his [ManagedDonkey](https://github.com/theTwist84/ManagedDonkey/) project
- The [ElDewrito project](https://github.com/ElDewrito/ElDorito) for its patching & hooking code
- The [Anvil Station](https://discord.gg/hetx7ekZJQ) team
