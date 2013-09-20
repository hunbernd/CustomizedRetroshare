

#ifndef P3ZERORESERVERRS_H
#define P3ZERORESERVERRS_H


#include "RSExampleItems.h"

#include "retroshare/rspeers.h"
#include "plugins/rspqiservice.h"
#include "pqi/pqimonitor.h"




class RsPluginHandler;
class RsPeers;

class p3ExampleRS : public RsPQIService,   // the service interface calls tick()
                    public pqiMonitor      // the monitor tells us when friends changed their status
{
public:
    p3ExampleRS(RsPluginHandler *pgHandler, RsPeers* peers);

    virtual int tick();
    virtual void statusChange(const std::list<pqipeer> &plist);


private:
    void handleMessage( RsExampleItem *item );
    void handleExampleItem( RsExampleItem * item );

private:

    RsPeers * m_peers;
};

#endif // P3ZERORESERVERRS_H
