#include "uri_map.h"
#include <cstdarg>

// API logging based on Matt's implementation in HaloToolz
std::unordered_map<std::string, const char*> backend_hashmap
{
	{ "f96526423b3c473eb3350716b74634b1", "FriendsService" },
	{ "7804a9f0c48f4e68bc3b16e9933e58e4", "SubscriptionAdd" },
	{ "dec87cc927e7495582b7adc34f0860d8", "SubscriptionRemove" },
	{ "0c9bf0e3062445a0a73330230e85c0ac", "GetSubscriptions" },

	{ "a74f776492be448295798ba5c74ab4eb", "GameStatisticsService" },
	{ "9223379eafaf410d87d67443eaccd6d6", "CheckNewUserChallenges" },
	{ "a5b78a034b7440b3a50f37f6c06602d2", "GetUserChallenges" },
	{ "80d708a9f90345ec97f50bb4d7bbdc57", "GetChallengeSlots" },
	{ "975c93760e80490dab77353e475fbf1a", "RerollSlot" },
	{ "e116f6a8e3f34405803cb440562fab9c", "GetChallengeSlotInstances" },
	{ "3c214f66f0d446948981f6cf264236d5", "GetChallengeInstances" },
	{ "5c84448cf8374e60afef20c28252dc66", "GetChallengeDefinitions" },
	{ "697218622fb0441293559baddd764c2e", "ActivateChallenges" },
	{ "3f721e32ed8940bf8a63a59bfc451781", "DeactivateChallenges" },

	{ "e4b95cd8156947828b12000f80474850", "HeartbeatService" },
	{ "461d92357a0d43628f01ddd7e075e6ea", "GetServicesStatus" },
	{ "f152c44779c9442b841ed401c012a35a", "GetHeartbeatStatus" },
	{ "852b0ea5ac8444fca71bb4d86f96a20d", "GetDate" },

	{ "8a30791a119a4a2cb6db7f496132dff3", "MessagingService" },
	{ "27d1f8c2eb7b4d2fb67562e20cda99d4", "JoinChannels" },
	{ "c62ab4fd430642688323647ff442fc26", "LeaveChannels" },
	{ "a244b7f4b0cd49259e931dd28be75c90", "Send" },
	{ "2d3844a2559b4268a984f96fc5d8014e", "SendServiceMessage" },
	{ "a48546b55a1446c7acce1061545ed196", "Receive" },

	{ "9c1aaae299da4b9db7e9583dc8b0b2f2", "VivoxService" },
	{ "e50fd076ce5f4b16955486b6cffeecd5", "Login" },

	{ "caa11c5e19dc42c38b67cf4acda5db44", "PresenceService" },
	{ "d3116e1e6b2b4157b21dd98582ee85db", "PresenceDisconnect" },
	{ "be1c65d830d445ba93f33dfeca6419ed", "PresenceConnect" },
	{ "4a2ae7db19f94cbc82af96f0c35bf95c", "PresenceGetUsersPresence" },
	{ "05c8d2d966dd4948b438066de594ccf7", "PartyJoin" },
	{ "9d17002ac24b4cecb1ac321dcdf7c495", "PartyLeave" },
	{ "21ceed397c454e38bb19526b7751bf83", "PartyKick" },
	{ "3f70ac03ffc24443bf229eab2d16a6c1", "PartySetGameData" },
	{ "5e7d3efa99c147d7a05dbea0d4629c63", "PartyGetStatus" },
	{ "d592dc7d4cf64ee8a398dbf08f210b52", "PartyGetStatusVersion" },
	{ "c0858270a5ad414ca7ff5075adcb2c6d", "CustomGameStart" },
	{ "553c7026b6c3445f9305690f347bb90c", "CustomGameStop" },
	{ "2313549188154d409cc401cb8cc6eee6", "MatchmakeStart" },
	{ "90439d26ddbe46bfb6ef8827441c24c5", "MatchmakeStop" },
	{ "8acbd061727e421bba5bef015c18e2e2", "MatchmakeGetStatus" },
	{ "724a87acdaa9410e9702ec220a8f0680", "ReportOnlineStats" },
	{ "7f1c2580473f4a17951e8129dca74adf", "GetPlaylistStat" },
	{ "ab323f1083ad4caa8658bda7912fc4a6", "GetPlaylistStatVersion" },

	{ "b4d15f136196431a998e01903c605367", "SessionControlService" },
	{ "cc6882b588d94b7a843de5289f05355b", "ClientGetStatus" },
	{ "a743cb48398e43dc8edbcc465ea52673", "GetSessionBasicData" },
	{ "0a490cb9c5e145be9a7ceb7c098caf9a", "GetSessionChain" },

	{ "ee3aec5524854d6e918307d24a14623d", "TitleResourceService" },
	{ "c3bf25f34b72499ca307c6e4431c79c1", "GetTitleConfiguration" },
	{ "8daf1363deae4319a8beecda0490b9f9", "GetTitleConfigRaw" },
	{ "a37bdcfd61fe48efaab11586255fdd54", "GetTitleTagsPatchConfiguration" },

	{ "d7e31cf042024b22ac09d2a42c0403cb", "UserStorageService" },
	{ "1275b0dfa4ed4d9bacd8bb37bdc9fac4", "SetPrivateData" },
	{ "ab52a321ea8d4a78a61f10620d74e2d1", "GetPrivateData" },
	{ "69d967acb77e455987e230d6e8b5b1c0", "SetPublicData" },
	{ "dbcbbedf8eb748b19766e1f7e25e6e46", "GetPublicData" },

	{ "416c82d428394df6a1bb9897f7d248b2", "UserService" },
	{ "aebaa08921984fe19ce6562850b34281", "SignOut" },
	{ "28635cd91941462aac4d0c36497e00f1", "GetUsersByNickname" },
	{ "fd9a7e1d508242be9effaf969860fba8", "NicknameChange" },
	{ "fba12d422e6b447ea7f3d9a19a6964bb", "GetUsersBaseData" },
	{ "f6cf5fa595f2496290875288874a3396", "GetUsersPrimaryStates" },
	{ "9e18f0339c27412c89551072bfb60fb6", "GetUserStates" },
	{ "f4278d2a42314dfbaa5f0e3d3e196dfa", "GetTransactionHistory" },
	{ "751112d775824326b45cdf038296df88", "ApplyExternalOffer" },
	{ "a9681a7e8685475599c406b8747739ee", "ApplyOfferAndGetTransactionHistory" },
	{ "c5f3082e2d584d53abcb0ff0a9ad81cf", "ApplyOfferListAndGetTransactionHistory" },
	{ "85aa11207964476da0eeee10c54bbed4", "ApplyOffer" },
	{ "ff7dfbec3141480783455955980d8084", "ApplyOfferList" },
	{ "7f561c33e3bb492693f8c6c8ede19bb4", "GetItemOffers" },
	{ "872c55a7fb1d4c848e14d96f7e78fc03", "GetShop" },

	{ "89dffc06f9fc43eba2bd9271d22a609b", "ClanService" },
	{ "a2e33ce546ed4cefaf71533fc5fd07c4", "ClanCreate" },
	{ "20baf94d9482443cb75876344d492369", "ClanGetBaseData" },
	{ "611f172a9eca4816905ebbdd7d8e55d0", "ClanGetMembership" },
	{ "a1a2b7c387a0488fa6f72847959a4ba6", "ClanJoin" },
	{ "24223c8811154233ba26ac82dc01f0c4", "ClanKick" },
	{ "f1a64d6e121543eca2b4b3ae7ed9fdeb", "ClanLeave" },
	{ "044d42867b7e431e85ae58e868e1cf96", "ClanGetByName" },

	{ "ca7ce17947c14aaa9adff2cabd8e8fdc", "ArbitraryStorageService" },
	{ "0cfbb05ba5fa4ac3a2d8760b59c78276", "WriteDiagnosticsData" },
	{ "920e14f21b584d7280f18c8fe4a90257", "WriteADFPack" },

	{ "EndpointsDispatcherService", "EndpointsDispatcherService" },
	{ "GetAuthorizationEndpointsAndDate", "GetAuthorizationEndpointsAndDate" },

	{ "AuthorizationService", "AuthorizationService" },
	{ "Enqueue", "Enqueue" },
	{ "Dequeue", "Dequeue" },
	{ "RefreshTokens", "RefreshTokens" },

};

// Takes in an obfuscated URI and overwrites it with the deobfuscated version
void backend_deobfuscate_uri(char* uri, long uri_size)
{
	// find and isolate the service and resource names
	std::string uri_str(uri);
	long first = uri_str.find('/');
	long last = uri_str.find(".svc/");
	std::string service = uri_str.substr(first + 1, last - 1);
	std::string resource = uri_str.substr(last + 5, uri_str.length() - last);

	if (backend_hashmap.find(service) != backend_hashmap.end())
	{
		service = backend_hashmap.at(service);
	}
	if (backend_hashmap.find(resource) != backend_hashmap.end())
	{
		resource = backend_hashmap.at(resource);
	}

	// rebuild the URI with the deobfuscated names
	snprintf(uri, uri_size, "/%s.svc/%s", service.c_str(), resource.c_str());
}