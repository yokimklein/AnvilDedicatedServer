# AnvilDedicatedServer
A WIP modification for Halo Online 11.1.604673 cert_ms29 which restores missing networking functions for server hosting.

# Notice
**This is currently incomplete** - you cannot join a hosted game. You can however create a local session and change your player loadout.
Most of the missing code has been reimplemented, but a bug prevents joins from finalising.

# The issue (packet overflow)
- The final packet sent to the client when joining is the membership-update packet.
- Currently, when it is sent to the client, it is rejected due to a 'message overflow', and the client aborts and attempts to reconnect (this happens infinitely as each membership update overflows, until the client gives up on joining).
- The overflow means the packet is too large for the client to accept, and while the struct itself is larger than the maximum packet size, the game is meant to break it up into smaller chunks for the client to handle.
- I can only assume this either isn't happening (which I doubt as the code seems to remain for it) or our struct is wrong somehow and causing the decoded packet to be larger than it should be.
- I've run out of motivation to continue working on the project for the time being as I've spent so long trying to figure this one out. Hopefully someone with more experience than me can take this and get it working.

# Another issue (simulation)
The distributed simulation mode is partially broken in the ms29 client. Some weapons don't fire, effects don't appear. There may be more code missing from the client in this area.

# Usage
1) Build hf2p_dedicated_server.dll
2) Place it in the same folder as halo_online.exe
3) Run the launcher to inject the dll
4) Open the console window that appears

# Controls
- PAGE UP: Creates the host session (Run this only when the loading screen has disappeared otherwise the game will crash)
- PAGE DOWN: Prints the server's secure address and identifier to the console for clients to use when joining
- INSERT: Sets the host's player data (The session needs at least 1 player to launch, so do this if you are on your own)
- DELETE: Adds a test player to the session. This is buggy, your camera will be tied to this player though you will still control the original.
- HOME: Launches the session
- END: Ends the session

# Quick start (Boot into a session on your own)
- Press PAGE UP once the loading screen as disappeared (you will see activity in the console window)
- Press INSERT
- Press HOME

# Special thanks
- @theTwist84 for reversing most of the structs used in the project
- The [ElDewrito project](https://github.com/ElDewrito/ElDorito) for it's patching & hooking code
- The [Anvil Station](https://discord.gg/hetx7ekZJQ) team
