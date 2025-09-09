# AnvilDedicatedServer
A modification for Halo Online build 11.1.604673 cert_ms29 which restores missing server hosting features.<br/>

**Part of the [Anvil Station](https://discord.gg/hetx7ekZJQ) project, dedicated to reviving the original Halo Online game.**<br/>
<div align="left">
      <a href="https://www.youtube.com/watch?v=S9f6aUFZ4Ek">
         <img src="https://github.com/yokimklein/AnvilDedicatedServer/blob/main/video_thumbnail.png" style="width:100%;">
      </a>
</div> <br/>

## Work is in progress!
Slayer is now fully playable! However other gamemodes are currently not supported. We're working to restore these as soon as possible. Please report any issues/bugs found to [our issues page.](https://github.com/yokimklein/AnvilDedicatedServer/issues)<br/>
<br/>
There currently isn't a way to join a server without connecting to our closed source game API. If you would like to test this on your own, you will have to call the hf2p_join_game function on your client manually:
```cpp
// located at (module base + 0x319880)
void __fastcall hf2p_join_game(long ipv4_address, short port, s_transport_secure_identifier session_id, s_transport_secure_address session_address)
```

## Controls
- PAGE DOWN: Starts a map vote when in the lobby, followed by the launch countdown
- INSERT: Force sets the game to Slayer on Valhalla
- HOME: Launches into a game if a valid map & gamemode are set
- END: Returns an active game to the main menu/lobby

## Quick setup
1) Download & install Visual Studio 2022
2) Download Halo Online build 11.1.604673_cert_ms29
3) Create a folder named `11.1.604673_cert_ms29` next to `AnvilDedicatedServer.sln`
4) Place the Halo Online build into that folder (where `halo_online.exe` must be located)
5) Build the hf2p_dedicated_server project in debug or release
6) Run `hf2p_dedicated_server_cache_debug.exe` or `hf2p_dedicated_server_cache_release.exe`
8) Once the game has loaded, the server will display the port, server address and server ID to connect to
11) Once the game start status is 'ready' in the console, press PAGE DOWN to begin a map vote
12) Once the map vote has concluded the launch countdown will begin and the game will start

## Special thanks & credits
- [@twist84](https://github.com/twist84) for reversing many of game's structures used in this project and allowing me to use code from [ManagedDonkey](https://github.com/twist84/ManagedDonkey)
- The [Anvil Station](https://discord.gg/hetx7ekZJQ) team
- The [ElDewrito project](https://github.com/ElDewrito/ElDorito) for engine research & for being my start into Halo modding
