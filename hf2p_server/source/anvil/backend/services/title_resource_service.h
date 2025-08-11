#pragma once
#include <anvil\backend\backend.h>
#include <anvil\backend\request_info.h>
#include <variant>

enum e_title_instance_game_class
{
    _instance_invalid,
    _instance_ui_desc,
    _instance_internal_currency_bundle,
    _instance_game_mode,
    _instance_map_info,
    _instance_diff_players,
    _instance_diff_coef,
    _instance_item,
    _instance_gameplay_modifier,
    _instance_weapon,
    _instance_wpn_ui_stats,
    _instance_grenade,
    _instance_booster,
    _instance_consumable,
    _instance_cons_ui_stats,
    _instance_armor_item,
    _instance_armor_suit,
    _instance_color,
    _instance_reward,
    _instance_player_level,
    _instance_data_mining,
    _instance_motd,
    _instance_advertisment,
    _instance_news,
    _instance_scoring_event,
    _instance_ds_defaults,
    _instance_challenge,
    _instance_account_label,
    _instance_emblem,
    _instance_post_match_srv_params,
    _instance_assign_kit,
    _instance_playlist,
    _instance_mp_defaults,

    k_title_instance_game_class_count
};

constexpr const char* K_TITLE_INSTANCE_GAME_CLASS[k_title_instance_game_class_count] =
{
    "INVALID_INSTANCE", // non-original
    "UI_DESC",
    "INTERNAL_CURRENCY_BUNDLE",
    "GAME_MODE",
    "MAP_INFO",
    "DIFF_PLAYERS",
    "DIFF_COEF",
    "ITEM",
    "GAMEPLAY_MODIFIER",
    "WEAPON",
    "WPN_UI_STATS",
    "GRENADE",
    "BOOSTER",
    "CONSUMABLE",
    "CONS_UI_STATS",
    "ARMOR_ITEM",
    "ARMOR_SUIT",
    "COLOR",
    "REWARD",
    "PLAYER_LEVEL",
    "DATA_MINING",
    "MOTD",
    "ADVERTISMENT",
    "NEWS",
    "SCORING_EVENT",
    "DS_DEFAULTS",
    "CHALLENGE",
    "ACCOUNT_LABEL",
    "EMBLEM",
    "POST_MATCH_SRV_PARAMS",
    "ASSIGN_KIT",
    "PLAYLIST",
    "MP_DEFAULTS"
};

enum e_title_property_types
{
    _title_property_none,
    _title_property_integer,
    _title_property_float,
    _title_property_string,
    _title_property_long,
    _title_property_integer_list,
    _title_property_string_list,
    _title_property_object_list,
    _title_property_float_list,
    
    k_title_property_types_count
};

struct s_title_instance
{
    struct s_properties
    {
        struct s_search_results
        {
            s_title_instance::s_properties* const get_property(const char* name);

            long get_integer(const char* name);
            bool get_boolean(const char* name);
            float get_float(const char* name);
            std::string get_string(const char* name);
            long64 get_long(const char* name);
            std::vector<long> get_integer_list(const char* name);
            std::vector<std::string> get_string_list(const char* name);
            std::vector<s_title_instance> get_object_list(const char* name);
            std::vector<float> get_float_list(const char* name);

            std::vector<s_title_instance::s_properties> results;
        };

        std::string name;
        e_title_property_types type;
        std::variant
            <
            /*   intVal*/ long,
            /* floatVal*/ float,
            /*   strVal*/ std::string,
            /*  longVal*/ long64,
            /*  intList*/ std::vector<long>,
            /*  strList*/ std::vector<std::string>,
            /*  objList*/ std::vector<s_title_instance>,
            /*floatList*/ std::vector<float>
            >value;
    };

    // $NOTE: this will return props in the order in which they appear in the TI, not in the order of the search parameters!
    s_properties::s_search_results get_properties(std::vector<std::string> prop_names);

    std::string name;
    std::string className;
    std::vector<std::string> parents;
    std::vector<s_properties> props;
};

class c_backend::title_resource_service
{
public:
    static void initialise(resolved_endpoint* endpoint);
    static resolved_endpoint& endpoint();

    class get_title_configuration
    {
    public:
        inline static s_request_info m_status;

        struct s_request : s_backend_request
        {
            std::string combinationHash; // $TODO: ignored by API

            std::string to_json() override;
        };
        static void request();

        struct s_response
        {
            std::string combinationHash;
            std::vector<s_title_instance> instances; // title_instances name not in response
        };
    protected:
        static void response(s_backend_response* response);
    };

private:
    inline static resolved_endpoint* m_endpoint;
};
