#ifndef CUPCAKE_H
#define CUPCAKE_H

#include <sys/types.h>
#include <dirent.h>
#include <errno.h>
#include <stdio.h>

#include <cstdlib>
#include <string>
#include <sstream>
#include <fstream>
#include <iostream>
#include <list>
#include <vector>
#include <retroshare/rsmsgs.h>
#include <retroshare/rspeers.h>
#include <QObject>
#include <ctime>
#include<sstream>
#include <QTextEdit>

#include <retroshare/rsmsgs.h>
#include <retroshare/rschannels.h>
#include <retroshare/rsforums.h>
#include <retroshare/rsdistrib.h>
#include "gui/chat/ChatDialog.h"

class CupCake : public QObject
{
    Q_OBJECT

public:   
    ~CupCake();
    void log(std::string msg, int loglevel);
    static CupCake* getInstance();
    unsigned int checkInterval;
    void processMessage(const ChatInfo &cinfo);

public slots:
    void tick();

protected:    
    unsigned int maxFriends;
    std::ofstream ofs;
    const std::string currentDateTime(time_t now = time(0));
    //0: repetitive, 1: info, 2: warning, 3: error
    bool minloglevel;
    //unsigned int longCycle; //ticks
    int unsubcribeMonths;

private:
    static CupCake* _instance;
    CupCake();
    void refreshlobbies();
    void refreshforums();
    void refreshchannels();
//    void printstatistics();
    unsigned int forumstat;
    unsigned int channelstat;
    unsigned int chatstat;
    //unsigned int tickCounter;

};

//    const unsigned int ticksUntilLobbieIsCreated;
//    std::string lobbyName;

//    static ChatLobbyId lid = 0;
//    unsigned int tickCounter;

//    void createOrRejoinLobby();

#endif // CUPCAKE_H
