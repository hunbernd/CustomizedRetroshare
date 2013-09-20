/*
    This file is part of the Zero Reserve Plugin for Retroshare.

    Zero Reserve is free software: you can redistribute it and/or modify
    it under the terms of the GNU Lesser General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    Zero Reserve is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU Lesser General Public License for more details.

    You should have received a copy of the GNU Lesser General Public License
    along with Zero Reserve.  If not, see <http://www.gnu.org/licenses/>.
*/

#include "p3ExampleRS.h"

#include "pqi/p3linkmgr.h"

#include <iostream>

// after getting data from 3 peers, we believe we're complete
static const int INIT_THRESHOLD = 3;

p3ExampleRS::p3ExampleRS(RsPluginHandler *pgHandler, RsPeers* peers ) :
        RsPQIService( RS_SERVICE_TYPE_EXAMPLE_PLUGIN, CONFIG_TYPE_EXAMPLE_PLUGIN, 0, pgHandler ),

        m_peers(peers)
{
    addSerialType(new RsExampleSerialiser());
    pgHandler->getLinkMgr()->addMonitor( this );
}


void p3ExampleRS::statusChange(const std::list< pqipeer > &plist)
{
    std::cerr << "Example: Status changed:" << std::endl;

    for (std::list< pqipeer >::const_iterator peerIt = plist.begin(); peerIt != plist.end(); peerIt++ ){
        if( RS_PEER_CONNECTED & (*peerIt).actions ){
            RsExampleItem * item = new RsExampleItem();
            item->PeerId( (*peerIt).id );
            sendItem( item );
        }
    }
}


int p3ExampleRS::tick()
{
    RsItem *item = NULL;
    while(NULL != (item = recvItem())){
        switch( item->PacketSubType() )
        {
        case RsExampleItem::EXAMPLE_ITEM:
            handleExampleItem( dynamic_cast<RsExampleItem*>( item ) );
            break;
        default:
            std::cerr << "Example: Received Item unknown" << std::endl;
        }
        delete item;
    }
    return 0;
}


void p3ExampleRS::handleExampleItem( RsExampleItem * item )
{
    std::cerr << item->PeerId() << " said: " << item->getMessage() << std::endl;
}


