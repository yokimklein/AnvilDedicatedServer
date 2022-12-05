# AnvilDedicatedServer
A WIP modification for Halo Online 11.1.604673 cert_ms29 which restores missing networking functions for server hosting.<br/>

Part of the [Anvil Station](https://discord.gg/hetx7ekZJQ) project, which aims to recreate the original Halo Online API and game servers.<br/>

**We're looking for new developers** to help us with the reverse engineering efforts, so if you're competent with IDA, C++ and have some understanding of how the 3rd gen blam engine works, [come talk to us!](https://discord.gg/hetx7ekZJQ)

# Notice
**This is currently incomplete** - clients can join the server, but they will get softlocked on the pre-game camera when the game starts and no simulation updates will be sent to them. The host will be able to see client players in game however.

# Simulation issues
The missing network session code responsible for handling players joining has been reimplemented. There is still missing simulation code however, so we can't quite play games yet. Some weapons don't fire and effects don't appear.<br/>
I don't know what the full extent of the missing simulation code is yet, however I have found that game_engine_attach_to_simulation as well as various simulation_action_object_create & simulation_action_object_update calls are missing throughout the codebase.

# Usage
1) Build hf2p_dedicated_server.dll
2) Place it in the same folder as halo_online.exe
3) Run the launcher to inject the dll
4) Open the console window that appears

# Controls
- PAGE UP: Creates the host session (Run this only when the loading screen has disappeared otherwise the game will crash)
- PAGE DOWN: Prints the server's secure address and identifier to the console for clients to use when joining
- INSERT: Sets player data for players 0 & 1 (Required to launch the session)
- HOME: Launches the session
- END: Ends the session

# Quick start (Boot into a session on your own)
- Press PAGE UP once the loading screen has disappeared to create the session
- Press INSERT to set the required player data
- Press HOME to launch into the session

# Special thanks
- @theTwist84 and his [ManagedDonkey](https://github.com/theTwist84/ManagedDonkey/) project for reversing most of the structs used in the project
- The [ElDewrito project](https://github.com/ElDewrito/ElDorito) for it's patching & hooking code
- The [Anvil Station](https://discord.gg/hetx7ekZJQ) team
