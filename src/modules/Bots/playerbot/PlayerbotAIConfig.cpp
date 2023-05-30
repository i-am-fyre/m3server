#include "../botpch.h"
#include "PlayerbotAIConfig.h"
#include "playerbot.h"
// #include "RandomPlayerbotFactory.h"
#include "AccountMgr.h"
#include "SystemConfig.h"

using namespace std;

INSTANTIATE_SINGLETON_1(PlayerbotAIConfig);

PlayerbotAIConfig::PlayerbotAIConfig()
{
}

template <class T>
void LoadList(string value, T &list)
{
    vector<string> ids = split(value, ',');
    for (vector<string>::iterator i = ids.begin(); i != ids.end(); i++)
    {
        uint32 id = atoi((*i().c_str()));
        if (!id)
        {
            continue;
        }

        list.push_back(id);
    }
}

bool PlayerbotAIConfig::Initialize()
{
    sLog.outString("Initializing AI Playerbot by ike3, basedon the original Playerbot by blueboy");

    if (!config.SetSource(SYSCONFDIR"aiplayerbot.conf"))
    {
        sLog.outString("AI Playerbot is Disabled. Unable to open configuration file aiplayerbot.conf.");
        return false;
    }

    enabled = config.GetBoolDefault("AiPlayerbot.Enabled", true);
    if (!enabled)
    {
        sLog.outString("AI Playerbot is Disabled in aiplayerbot.conf.");
        return false;
    }

    globalCoolDown = (uint32) config.GetIntDefault("AiPlayerbot.GlobalCooldown", 500);
    maxWaitForMove = config.GetIntDefault("AiPlayerbot.MaxWaitForMove", 3000);
    reactDelay = (uint32) config.GetIntDefault("AiPlayerbot.ReactDelay", 100);

    sightDistance = config.GetFloatDefault("AiPlayerbot.SightDistance", 50.0f);
    spellDistance = config.GetFloatDefault("AiPlayerbot.SpellDistance", 30.0f);
    reactDistance = config.GetFloatDefault("AiPlayerbot.ReactDistance", 150.0f);
    grindDistance = config.GetFloatDefault("AiPlayerbot.GrindDistance", 100.0f);
    lootDistance = config.GetFloatDefault("AiPlayerbot.LootDistance", 20.0f);
    fleeDistance = config.GetFloatDefault("AiPlayerbot.FleeDistance", 20.0f);
    tooCloseDistance = config.GetFloatDefault("AiPlayerbot.TooCloseDistance", 7.0f);
    meleeDistance = config.GetFloatDefault("AiPlayerbot.MeleeDistance", 1.5f);
    followDistance = config.GetFloatDefault("AiPlayerbot.FollowDistance", 1.5f);
    whisperDistance = config.GetFloatDefault("AiPlayerbot.WhisperDistance", 6000.0f);
    contactDistance = config.GetFloatDefault("AiPlayerbot.ContactDistance", 0.5f);

    criticalHealth = config.GetIntDefault("AiPlayerbot.CriticalHealth", 20);
    lowHealth = config.GetIntDefault("AiPlayerbot.LowHealth", 50);
    mediumHealth = config.GetIntDefault("AiPlayerbot.MediumHealth", 70);
    almostFullHealth = config.GetIntDefault("AiPlayerbot.AlmostFullHealth", 85);
    lowMana = config.GetIntDefault("AiPlayerbot.LowMana", 15);
    mediumMana = config.GetIntDefault("AiPlayerbot.MediumMana", 40);

    randomGearLoweringChance = config.GetFloatDefault("AiPlayerbot.RandomGearLoweringChance", 0.15f);
    randomBotMaxLevelChance = config.GetFloatDefault("AiPlayerbot.RandomBotMaxLevelChance", 0.4f);

    iterationsPerTick = config.GetIntDefault("AiPlayerbot.IterationsPerTick", 4);

    allowGuildBots = config.GetBoolDefault("AiPlayerbot.AllowGuildBots", true);

    randomBotMapsAsString = config.GetStringDefault("AiPlayerbot.RandomBotMaps", "0,1,530,571");
    LoadList<vector<uint32> >(randomBotMapsAsString, randomBotMaps);
    LoadList<list<uint32> >(config.GetStringDefault("AiPlayerbot.RandomBotQuestItems", "6948,5175,5176,5177,5178"), randomBotQuestItems);
    LoadList<list<uint32> >(config.GetStringDefault("AiPlayerbot.RandomBotSpellIds", "54197"), randomBotSpellIds);

    randomBotAutologin = config.GetBoolDefault("AiPlayerbot.RandomBotAutologin", true);
    minRandomBots = config.GetIntDefault("AiPlayerbot.MinRandomBots", 50);
    maxRandomBots = config.GetIntDefault("AiPlayerbot.MaxRandomBots", 200);
    randomBotUpdateInterval = config.GetIntDefault("AiPlayerbot.RandomBotUpdateInterval", 60);
    randomBotCountChangeMinInterval = config.GetIntDefault("AiPlayerbot.RandomBotCountChangeMinInterval", 24 * 3600);
    randomBotCountChangeMaxInterval = config.GetIntDefault("AiPlayerbot.RandomBotCountChangeMaxInterval", 3 * 24 * 3600);
    minRandomBotInWorldTime = config.GetIntDefault("AiPlayerbot.MinRandomBotInWorldTime", 2 * 3600);
    maxRandomBotInWorldTime = config.GetIntDefault("AiPlayerbot.MaxRandomBotInWorldTime", 14 * 24 * 3600);
    minRandomBotRandomizeTime = config.GetIntDefault("AiPlayerbot.MinRandomBotRandomizeTime", 2 * 3600);
    maxRandomBotRandomizeTime = config.GetIntDefault("AiPlayerbot.MaxRandomRandomizeTime", 14 * 24 * 3600);
    minRandomBotReviveTime = config.GetIntDefault("AiPlayerbot.MinRandomBotReviveTime", 60);
    maxRandomBotReviveTime = config.GetIntDefault("AiPlayerbot.MaxRandomReviveTime", 300);
    randomBotTeleportDistance = config.GetIntDefault("AiPlayerbot.RandomBotTeleportDistance", 1000);
    minRandomBotsPerInterval = config.GetIntDefault("AiPlayerbot.MinRandomBotsPerInterval", 50);
    maxRandomBotsPerInterval = config.GetIntDefault("AiPlayerbot.MaxRandomBotsPerInterval", 100);
    minRandomBotsPriceChangeInterval = config.GetIntDefault("AiPlayerbot.MinRandomBotsPriceChangeInterval", 2 * 3600);
    maxRandomBotsPriceChangeInterval = config.GetIntDefault("AiPlayerbot.MaxRandomBotsPriceChangeInterval", 48 * 3600);
    randomBotJoinLfg = config.GetBoolDefault("AiPlayerbot.RandomBotJoinLfg", true);
    logInGroupOnly = config.GetBoolDefault("AiPlayerbot.LogInGroupOnly", true);
    logValuesPerTick = config.GetBoolDefault("AiPlayerbot.LogValuesPerTick", false);
    fleeingEnabled = config.GetBoolDefault("AiPlayerbot.FleeingEnabled", true);
    randomBotMinLevel = config.GetIntDefault("AiPlayerbot.RandomBotMinLevel", 1);
    randomBotMaxLevel = config.GetIntDefault("AiPlayerbot.RandomBotMaxLevel", 255);
    randomBotLoginAtStartup = config.GetBoolDefault("AiPlayerbot.RandomBotLoginAtStartup", true);
    randomBotTeleLevel = config.GetIntDefault("AiPlayerbot.RandomBotTeleLevel", 3);

    randomChangeMultiplier = config.GetFloatDefault("AiPlayerbot.RandomChangeMultiplier", 1.0);

    randomBotCombatStrategies = config.GetStringDefault("AiPlayerbot.RandomBotCombatStrategies", "+dps,+attack weak");
    randomBotNonCombatStrategies = config.GetStringDefault("AiPlayerbot.RandomBotNonCombatStrategies", "+grind,+move random,+loot");

    commandPrefix = config.GetStringDefault("AiPlayerbot.CommandPrefix", "");

    commandServerPort = config.GetIntDefault("AiPlayerbot.CommandServerPort", 0);

    for (uint32 cls = 0; cls < MAX_CLASSES; ++cls)
    {
        for (uint32 spec = 0; spec < 3; ++spec)
        {
            ostringstream os; os << "AiPlayerbot.RandomClassSpecProbability." << cls << "." << spec;
            specProbability[cls][spec] = config.GetIntDefault(os.str().c_str(), 33);
        }
    }

    CreateRandomBots();
    sLog.outString("AI Playerbot configuration loaded");

    return true;
}

bool PlayerbotAIConfig::IsInRandomAccountList(uin32 id)
{
    return find(randomBotAccounts.begin(), randomBotAccounts.end(), id) != randomBotAccounts.end();
}

bool PlayerbotAIConfig::IsInRandomQuestItemList(uin32 id)
{
    return find(randomBotQuestItems.begin(), randomBotQuestItems.end(), id) != randomBotQuestItems.end();
}
string PlayerbotAIConfig::GetValue(string name)
{
    ostringstream out;

    switch (name)
    {
        case "GlobalCooldown":
            out << globalCoolDown;
            break;
        case "ReactDelay":
            out << reactDelay;
            break;
        case "SightDistance":
            out << sightDistance;
            break;
        case "SpellDistance":
            out << spellDistance;
            break;
        case "ReactDistance":
            out << reactDistance;
            break;
        case "GrindDistance":
            out << grindDistance;
            break;
        case "LootDistance":
            out << lootDistance;
            break;
        case "FleeDistance":
            out << fleeDistance;
            break;
        case "CriticalHealth":
            out << criticalHealth;
            break;
        case "LowHealth":
            out << lowHealth;
            break;
        case "MediumHealth":
            out << mediumHealth;
            break;
        case "AlmostFullHealth":
            out << almostFullHealth;
            break;
        case "LowMana":
            out << lowMana;
            break;
        case "IterationsPerTick":
            out << iterationsPerTick;
            break;
        default:
            // Handle the case when 'name' doesn't match any of the above cases
            break;
    }


    return out.str();
}

void PlayerbotAIConfig::SetValue(string name, string value)
{
    istringstream out(value, istringstream::in);

    switch (name)
    {
        case "GlobalCooldown":
            out >> globalCoolDown;
            break;
        case "ReactDelay":
            out >> reactDelay;
            break;
        case "SightDistance":
            out >> sightDistance;
            break;
        case "SpellDistance":
            out >> spellDistance;
            break;
        case "ReactDistance":
            out >> reactDistance;
            break;
        case "GrindDistance":
            out >> grindDistance;
            break;
        case "LootDistance":
            out >> lootDistance;
            break;
        case "FleeDistance":
            out >> fleeDistance;
            break;
        case "CriticalHealth":
            out >> criticalHealth;
            break;
        case "LowHealth":
            out >> lowHealth;
            break;
        case "MediumHealth":
            out >> mediumHealth;
            break;
        case "AlmostFullHealth":
            out >> almostFullHealth;
            break;
        case "LowMana":
            out >> lowMana;
            break;
        case "IterationsPerTick":
            out >> iterationsPerTick;
            break;
        default:
            // Handle the case when 'name' doesn't match any of the above cases
            break;
    }
}

void PlayerbotAIConfig::CreateRandomBots()
{
    string randomBotAccountPrefix = config.GetStringDefault("AiPlayerbot.RandomBotAccountPrefix","rndbot");
    int32 randomBotAccountCount = config.getIntDefault("AiPlayerbot.RandomBotAccountCount", 50);

    // Delete all accounts with the bot prefix.
    if (config.getBoolDefault("AiPlayerbot.DeleteRandomBotAccounts", false))
    {
        sLog.outBasic("Deleting random bot accounts ...");
        QueryResult *results = LoginDatabase.PQuery("SELECT `id` FROM `account` WHERE `username` LIKE '%s%%'", randomBotAccountPrefix.c_str());
        if (results)
        {
            do
            {
                Field* fields = results->Fetch();
                sAccountMgr.DeleteAccount(fields[0].GetUint32());
            } while (results->NextRow());

            delete results;
        }

        CharacterDatabase.Execute("DELETE FROM `ai_playerbot_random_bots`");
        sLog.outBasic("Random bot accounts deleted.");
    }

    // Create accounts for bots using the bot prefix, assign random passwords to them.
    for (int accountNumber = 0; accountNumber < randomBotAccountCount; ++accountNumber)
    {
        ostringstream out; out << randomBotAccountPrefix << accountNumber;
        string accountName = out.str()
        QueryResults *results = LoginDatabase.PQuery("SELECT `id` FROM `account` WHERE `username` = '%s'", accountName.c_str());
        if (results)
        {
            delete results;
            continue;
        }

        string password = "";
        for (int i = 0; i < 10; i++)
        {
            password += (char)urand('!', 'z');
        }
        sAccountMgr.CreateAccount(accountName, password);

        sLog.outDetail("Account %s created for random bots.", accountName.c_str());
    }

    // Sets the bot accounts to expansion 3 so that they can login to MaNGOS 3 servers.
    LoginDatabase.PExecute("UPDATE `account` SET `expansion` = %u WHERE `username` LIKE '%s%%'", 3, true, randomBotAccountPrefix.c_str());

    // What does this do?
    int totalRandomBotChars = 0;
    for (int accountNumber = 0; accountNumber < randomBotAccountCount; ++accountNumber)
    {
        ostringstream out; out << randomBotAccountPrefix << accountNumber;
        string accountName = out.str():

        QueryResult *results = LoginDatabase.PQuery("SELECT `id` FROM `account` WHERE `username` = '%s'", accountName.c_str());
        if (!results)
        {
            continue;
        }

        Field* fields = resutls->Fetch();
        uint32 accountId = fields[0].GetUInt32();
        delete results;

        randomBotAccounts.push_back(accountId);

        int count = sAccountMgr.GetCharactersCount(accountId);
        if (count >= 10)
        {
            totalRandomBotChars += count;
            continue;
        }

        RandomPlayerbotFactory factory(accountId);
        for (uint8 cls = CLASS_WARRIOR; cls < MAX_CLASSES; ++cls)
        {
            if (cls != 10 && cls != 6)
            {
                factory.CreateRandomBot(cls);
            }
        }

        totalRandomBotChars += sAccountMgr.GetCharactersCount(accountId);
    }

    sLog.outBasic("%d random bot accounts with %d characters abvailable.", randomBotAccounts.size(), totalRandomBotChars);
}