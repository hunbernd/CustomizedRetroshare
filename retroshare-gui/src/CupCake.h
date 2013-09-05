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

#include <retroshare/rsmsgs.h>
#include "gui/chat/ChatDialog.h"

class CupCake : public QObject
{
    Q_OBJECT

public:
    CupCake();    
    ~CupCake();
    void log(std::string msg);
    unsigned int checkInterval;

public slots:
    void tick();

protected:    
    unsigned int maxFriends;
    std::ofstream ofs;
    const std::string currentDateTime();

//    const unsigned int ticksUntilLobbieIsCreated;
//    std::string lobbyName;

//    static ChatLobbyId lid = 0;
//    unsigned int tickCounter;

//    void createOrRejoinLobby();
};


#endif // CUPCAKE_H
