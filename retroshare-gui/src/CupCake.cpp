#include "CupCake.h"

CupCake::CupCake()
{
    checkInterval = 30;
    maxFriends = 10;
    ofs.open("Cupcake.log");
    log("start");
    //ticksUntilLobbieIsCreated = 60;
    //tickCounter = 0;
    //lobbyName = "botlog";

    //lid = 0;
}

void CupCake::tick()
{
    log("begin tick");

    std::string lobbinames("");
    std::list<ChatLobbyInfo> lobbies;
    rsMsgs->getChatLobbyList(lobbies);
    for (std::list<ChatLobbyInfo>::const_iterator it = lobbies.begin(); it != lobbies.end();++it) {
            lobbinames += (it->lobby_name) + ";";
    }
    log("Lobbinames: " + lobbinames);

    std::string visiblelobbinames("");
    std::vector<VisibleChatLobbyRecord> visibleLobbies;
    rsMsgs->getListOfNearbyChatLobbies(visibleLobbies);
    for (std::vector<VisibleChatLobbyRecord>::const_iterator it = visibleLobbies.begin(); it != visibleLobbies.end();++it) {
            visiblelobbinames += (it->lobby_name) + ";";
    }
    log("Visible lobbinames: " + visiblelobbinames);

    log("end tick");
}

CupCake::~CupCake()
{
    log("close");
    ofs.close();
}

void CupCake::log(std::string msg)
{
    ofs << currentDateTime();
    ofs << ": ";
    ofs << msg;
    ofs << std::endl;
    /*
    if(lid != 0)
    {
        std::string vpid;
        rsMsgs->getVirtualPeerId(lid, vpid);
        rsMsgs->sendPrivateChat(vpid, msg);
    }
    */
}

const std::string CupCake::currentDateTime()
{
    time_t     now = time(0);
    struct tm  tstruct;
    char       buf[80];
    tstruct = *localtime(&now);
    // Visit http://www.cplusplus.com/reference/clibrary/ctime/strftime/
    // for more information about date/time format
    strftime(buf, sizeof(buf), "%Y-%m-%d.%X", &tstruct);
    return buf;
}

/*
CupCake::createOrRejoinLobby()
{
    std::vector<VisibleChatLobbyRecord> publicLobbies;
    rsMsgs->getListOfNearbyChatLobbies(publicLobbies);

    bool ableToRejoin = false;
    for (std::vector<VisibleChatLobbyRecord>::const_iterator it = publicLobbies.begin(); it != publicLobbies.end(); ++it)
    {
        if (it->lobby_name == lobbyName)
        {
            // rejoin
            rsMsgs->joinVisibleChatLobby(it->lobby_id);
            lid = it->lobby_id;
            ableToRejoin = true;
            return;
        }
    }

    if (!ableToRejoin)
    {
        // create new
        const std::list<std::string> emptyList = std::list<std::string>();
        lid = rsMsgs->createChatLobby(lobbyName, "", emptyList, RS_CHAT_LOBBY_PRIVACY_LEVEL_PUBLIC);
    }
}
*/
