# AnvilDedicatedServer
A modification for Halo Online build 11.1.604673 cert_ms29 which restores missing server hosting features.<br/>

**Part of the [Anvil Station](https://discord.gg/hetx7ekZJQ) project, dedicated to reviving the original Halo Online game.**<br/>
<div align="left">
      <a href="https://www.youtube.com/watch?v=S9f6aUFZ4Ek">
         <img src="https://github.com/yokimklein/AnvilDedicatedServer/blob/master/video_thumbnail.png" style="width:100%;">
      </a>
</div> <br/>

We're looking for new developers to help us with the reverse engineering efforts. If you're interested and familiar with IDA & C++, [come talk to us!](https://discord.gg/hetx7ekZJQ)

## Work is in progress!
Whilst slayer is playable in a mostly stable state, desyncs are present alongside some general jank-iness, and other gamemodes are not supported. Please bare this in mind!<br/>
Parts of the codebase are also long overdue for a cleanup.<br/>
<br/>
There currently isn't a way to join a server without connecting to our closed source game API. If you would like to test this on your own, you will have to call the hf2p_join_game function on your client manually:
```cpp
// located at (base address + 0x319880)
void __fastcall hf2p_join_game(long ip_address, short port, s_transport_secure_identifier session_id, s_transport_secure_address host_address)
```

## Controls
- PAGE DOWN: Starts a map vote when in the lobby, followed by the launch countdown
- INSERT: Force sets the game to Slayer on Valhalla
- HOME: Launches into a game if a valid map & gamemode are set
- END: Returns an active game to the main menu/lobby

## Quick setup
1) Build hf2p_dedicated_server.dll & hf2p_launcher.exe
2) Place the files in the same folder as halo_online.exe
3) Run hf2p_launcher.exe to boot the game with the dll injected
4) A console window should appear alongside the game, open it
5) Once the game has loaded, the server will create a lobby on port 11774 ready to join
6) The server IP, secure address & secure ID will be printed to the console which clients must provide in order to connect
7) Once the game start status is 'ready' in the console you can press PAGE DOWN to begin a map vote
8) Once the map vote has concluded the launch countdown will begin and the game will start

## Special thanks & credits
- [@twist84](https://github.com/twist84) for reversing many of game's structures used in this project and allowing me to use code from [ManagedDonkey](https://github.com/twist84/ManagedDonkey)
- The [Anvil Station](https://discord.gg/hetx7ekZJQ) team
- The [ElDewrito project](https://github.com/ElDewrito/ElDorito) for engine research & for being my start into Halo modding
