#include "CupCake.h"

CupCake::CupCake()
{
    checkInterval = 30;
    maxFriends = 10;
    minloglevel = 0;
    forumstat = 0;
    chatstat = 0;
    channelstat = 0;
    ofs.open("Cupcake.log");
    log("start", 1);
    //ticksUntilLobbieIsCreated = 60;
    //tickCounter = 0;
    //lobbyName = "botlog";

    //lid = 0;
}

void CupCake::tick()
{
    log("begin tick", 0);

    refreshlobbies();
    refreshchannels();
    refreshforums();
    //printstatistics();

    log("end tick", 0);
}

CupCake::~CupCake()
{
    log("close", 1);
    ofs.close();
}

void CupCake::log(std::string msg, int loglevel)
{
    if(loglevel < minloglevel) return;
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

void CupCake::refreshchannels()
{
    log("begin channels refresh", 0);
    if (!rsChannels) {
            return;
    }

    std::list<ChannelInfo> channelList;
    std::list<ChannelInfo>::iterator it;
    rsChannels->getChannelList(channelList);
    for(it = channelList.begin(); it != channelList.end(); it++) {
        uint32_t flags = it->channelFlags;
        if(!(flags & (RS_DISTRIB_SUBSCRIBED | RS_DISTRIB_ADMIN)))
        {
            std::string cn(it->channelName.begin(), it->channelName.end());
            log("Subscribing to channel: " + cn, 1);
            rsChannels->channelSubscribe(it->channelId, true, false);
        }
    }    
    //statistics
    unsigned int mcount, ucount, count;
    count = channelList.size();
    rsChannels->getMessageCount("", mcount, ucount);
    if(channelstat != (count + mcount + ucount))
    {
        channelstat = count + mcount + ucount;
        std::stringstream ss;
        ss << "Channel count: " << count << ", new messages: " << mcount << ", unread messages: " << ucount;
        log(ss.str(), 1);
    }
    log("end channels refresh", 0);
}

void CupCake::refreshlobbies()
{
    log("begin lobbies refresh", 0);
    ChatLobbyId lid;
    std::string vpid;
    std::vector<VisibleChatLobbyRecord> visibleLobbies;
    rsMsgs->getListOfNearbyChatLobbies(visibleLobbies);
    for (std::vector<VisibleChatLobbyRecord>::const_iterator it = visibleLobbies.begin(); it != visibleLobbies.end();++it) {
        lid = it->lobby_id;
        //check if we are subcribed
        if(!rsMsgs->getVirtualPeerId(lid, vpid))
        {
            //join
            rsMsgs->joinVisibleChatLobby(lid);
            log("Subscribing to lobby: " + it->lobby_name, 1);
            if(rsMsgs->getVirtualPeerId(lid, vpid))
            {
                ChatDialog::chatFriend(vpid,false);
                log("Subscribed to lobby: " + it->lobby_name, 1);
            }
            else
            {
                log("Failed to subscribe to lobby: " + it->lobby_name, 2);
            }
        }
    }
    //statistics
    unsigned int count;
    count = visibleLobbies.size();
    if(chatstat != count)
    {
        chatstat = count;
        std::stringstream ss;
        ss << "Chat lobby count: " << count;
        log(ss.str(), 1);
    }
    log("end lobbies refresh", 0);
}

void CupCake::refreshforums()
{
    log("begin forums refresh", 0);
    if (!rsForums) {
            return;
    }

    std::list<ForumInfo> forumList;
    std::list<ForumInfo>::iterator it;
    rsForums->getForumList(forumList);
    for(it = forumList.begin(); it != forumList.end(); it++) {
        uint32_t flags = it->subscribeFlags;
        if(!(flags & (RS_DISTRIB_SUBSCRIBED | RS_DISTRIB_ADMIN)))
        {
            std::string fn(it->forumName.begin(), it->forumName.end());
            log("Subscribing to forum: " + fn, 1);
            rsForums->forumSubscribe(it->forumId, true);
        }
    }
    //statistics
    unsigned int mcount, ucount, count;
    count = forumList.size();
    rsForums->getMessageCount("", mcount, ucount);
    if(forumstat != (count + mcount + ucount))
    {
        forumstat = count + mcount + ucount;
        std::stringstream ss;
        ss << "Forum count: " << count << ", new messages: " << mcount << ", unread messages: " << ucount;
        log(ss.str(), 1);
    }
    log("end forums refresh", 0);
}

const std::string CupCake::currentDateTime()
{
    time_t     now = time(0);
    struct tm  tstruct;
    char       buf[80];
    tstruct = *localtime(&now);
    // Visit http://www.cplusplus.com/reference/clibrary/ctime/strftime/
    // for more information about date/time format
    strftime(buf, sizeof(buf), "%Y-%m-%d %X", &tstruct);
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

/*
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
 */
