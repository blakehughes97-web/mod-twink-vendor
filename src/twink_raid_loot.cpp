#include "ScriptMgr.h"
#include "Player.h"
#include "Config.h"
#include "Log.h"
#include "Chat.h"

class TwinkRaidLoot_CommandScript : public CommandScript
{
public:
    TwinkRaidLoot_CommandScript() : CommandScript("TwinkRaidLoot_CommandScript") { }

    std::vector<ChatCommand> GetCommands() const override
    {
        static std::vector<ChatCommand> commandTable =
        {
            { "twinkraidloot", rbac::RBAC_PERM_COMMAND_GM, false, &HandleTwinkRaidLootCommand, "" },
        };
        return commandTable;
    }

    static bool HandleTwinkRaidLootCommand(ChatHandler* handler, const char* /*args*/)
    {
        if (!sConfigMgr->GetOption<bool>("TwinkRaidLoot.Enable", true))
        {
            handler->SendSysMessage("|cffff0000TwinkRaidLoot module is disabled.|r");
            return true;
        }

        Player* player = handler->GetSession()->GetPlayer();
        if (!player) return true;

        uint32 vendorEntry = sConfigMgr->GetOption<uint32>("TwinkRaidLoot.VendorEntry", 990001);

        Creature* vendor = player->SummonCreature(vendorEntry, player->GetPositionX(), player->GetPositionY(), player->GetPositionZ(), player->GetOrientation(), TEMPSUMMON_MANUAL_DESPAWN);
        if (vendor)
        {
            handler->PSendSysMessage("|cff00ff00[Twink Raid Loot]|r Vendor spawned (GUID: %u). It is now permanent.", vendor->GetGUID().GetCounter());
            vendor->SaveToDB();
        }
        else
            handler->SendSysMessage("|cffff0000Failed to spawn vendor.|r");

        return true;
    }
};

class TwinkRaidLoot_WorldScript : public WorldScript
{
public:
    TwinkRaidLoot_WorldScript() : WorldScript("TwinkRaidLoot_WorldScript") { }

    void OnStartup() override
    {
        TC_LOG_INFO("server.loading", ">> mod-twink-raid-loot loaded - Twink Raid Loot Vendor (19/29/39/49/59/69/79) ready!");
    }
};

void AddTwinkRaidLootScripts()
{
    new TwinkRaidLoot_CommandScript();
    new TwinkRaidLoot_WorldScript();
}