/*
 * libretroshare/src/serialiser: rsgxsupdateitems.h
 *
 * RetroShare Serialiser.
 *
 * Copyright 2012 Christopher Evi-Parker
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Library General Public
 * License Version 2 as published by the Free Software Foundation.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Library General Public License for more details.
 *
 * You should have received a copy of the GNU Library General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307
 * USA.
 *
 * Please report all bugs and problems to "retroshare@lunamutt.com".
 *
 */

#include "rsgxsupdateitems.h"




RsGxsGrpUpdateItem::RsGxsGrpUpdateItem() {
}

RsGxsGrpUpdateItem::~RsGxsGrpUpdateItem() {
}

void RsGxsGrpUpdateItem::clear()
{
	grpUpdateTS = 0;
	peerId.clear();
}

std::ostream& RsGxsGrpUpdateItem::print(std::ostream& out, uint16_t indent)
{

}

RsGxsMsgUpdateItem::RsGxsMsgUpdateItem()
{
}

RsGxsMsgUpdateItem::~RsGxsMsgUpdateItem() {
}

void RsGxsMsgUpdateItem::clear()
{
    msgUpdateTS.clear();
    peerId.clear();
}

std::ostream& RsGxsMsgUpdateItem::print(std::ostream& out, uint16_t indent)
{
}

uint32_t RsGxsUpdateSerialiser::size(RsItem* item)
{
	RsGxsMsgUpdateItem* mui = NULL;
	RsGxsGrpUpdateItem* gui = NULL;

    if((mui = dynamic_cast<RsGxsMsgUpdateItem*>(item))  != NULL)
    {
        return sizeGxsMsgUpdate(mui);
    }else if(( gui = dynamic_cast<RsGxsGrpUpdateItem*>(item)) != NULL){
        return sizeGxsGrpUpdate(gui);
    }else
    {
#ifdef RSSERIAL_DEBUG
    	std::cerr << "RsGxsUpdateSerialiser::size(): Could not find appropriate size function"
    			  << std::endl;
#endif
    	return 0;
    }
}

bool RsGxsUpdateSerialiser::serialise(RsItem* item, void* data,
		uint32_t* size)
{
}

RsItem* RsGxsUpdateSerialiser::deserialise(void* data, uint32_t* size)
{

#ifdef RSSERIAL_DEBUG
	std::cerr << "RsNxsSerialiser::deserialise()" << std::endl;
#endif
	/* get the type and size */
	uint32_t rstype = getRsItemId(data);

	if ((RS_PKT_VERSION_SERVICE != getRsItemVersion(rstype)) ||
			(SERVICE_TYPE != getRsItemService(rstype)))
	{
			return NULL; /* wrong type */
	}

	switch(getRsItemSubType(rstype))
	{

	case RS_PKT_SUBTYPE_GXS_MSG_UPDATE:
		return deserialGxsMsgUpdate(data, size);
	case RS_PKT_SUBTYPE_GXS_GRP_UPDATE:
		return deserialGxsGrpUpddate(data, size);
	default:
		{
#ifdef RSSERIAL_DEBUG
			std::cerr << "RsGxsUpdateSerialiser::deserialise() : data has no type"
					  << std::endl;
#endif
			return NULL;

		}
	}
}

uint32_t RsGxsUpdateSerialiser::sizeGxsGrpUpdate(RsGxsGrpUpdateItem* item)
{
	uint32_t s = 8; // header size
	s += GetTlvStringSize(item->peerId);
	s += 4;
	return s;
}

bool RsGxsUpdateSerialiser::serialiseGxsGrpUpdate(RsGxsGrpUpdateItem* item,
		void* data, uint32_t* size)
{
#ifdef RSSERIAL_DEBUG
    std::cerr << "RsGxsUpdateSerialiser::serialiseGxsGrpUpdate()" << std::endl;
#endif

    uint32_t tlvsize = sizeGxsGrpUpdate(item);
    uint32_t offset = 0;

    if(*size < tlvsize){
#ifdef RSSERIAL_DEBUG
        std::cerr << "RsGxsUpdateSerialiser::serialiseGxsGrpUpdate() size do not match" << std::endl;
#endif
        return false;
    }

    *size = tlvsize;

    bool ok = true;

    ok &= setRsItemHeader(data, tlvsize, item->PacketId(), tlvsize);

    /* skip the header */
    offset += 8;

    /* RsNxsSyncm */


    ok &= SetTlvString(data, *size, &offset, TLV_TYPE_STR_PEERID, item->peerId);
    ok &= setRawUInt32(data, *size, &offset, item->grpUpdateTS);

    if(offset != tlvsize){
#ifdef RSSERIAL_DEBUG
        std::cerr << "RsGxsUpdateSerialiser::serialiseGxsGrpUpdate() FAIL Size Error! " << std::endl;
#endif
        ok = false;
    }

#ifdef RSSERIAL_DEBUG
    if (!ok)
    {
        std::cerr << "RsGxsUpdateSerialiser::serialiseGxsGrpUpdate() NOK" << std::endl;
    }
#endif

    return ok;
}

RsGxsGrpUpdateItem* RsGxsUpdateSerialiser::deserialGxsGrpUpddate(void* data,
		uint32_t* size)
{

}

uint32_t RsGxsUpdateSerialiser::sizeGxsMsgUpdate(RsGxsMsgUpdateItem* item)
{
	uint32_t s = 8; // header size
	s += GetTlvStringSize(item->peerId);

	const std::map<std::string, uint32_t>& msgUpdateTS = item->msgUpdateTS;
	std::map<std::string, uint32_t>::const_iterator cit = msgUpdateTS.begin();

	for(; cit != msgUpdateTS.end(); cit++)
	{
		s += GetTlvStringSize(cit->first);
		s += 4;
	}

        s += 4; // number of map items

	return s;
}

bool RsGxsUpdateSerialiser::serialiseGxsMsgUpdate(RsGxsMsgUpdateItem* item,
		void* data, uint32_t* size)
{
#ifdef RSSERIAL_DEBUG
    std::cerr << "RsGxsUpdateSerialiser::serialiseGxsMsgUpdate()" << std::endl;
#endif

    uint32_t tlvsize = sizeGxsMsgUpdate(item);
    uint32_t offset = 0;

    if(*size < tlvsize){
#ifdef RSSERIAL_DEBUG
        std::cerr << "RsGxsUpdateSerialiser::serialiseGxsMsgUpdate() size do not match" << std::endl;
#endif
        return false;
    }

    *size = tlvsize;

    bool ok = true;

    ok &= setRsItemHeader(data, tlvsize, item->PacketId(), tlvsize);

    /* skip the header */
    offset += 8;

    /* RsNxsSyncm */


    ok &= SetTlvString(data, *size, &offset, TLV_TYPE_STR_PEERID, item->peerId);

    const std::map<std::string, uint32_t>& msgUpdateTS = item->msgUpdateTS;
    std::map<std::string, uint32_t>::const_iterator cit = msgUpdateTS.begin();

    uint32_t numItems = msgUpdateTS.size();
    ok &= setRawUInt32(data, *size, &offset, numItems);

    for(; cit != msgUpdateTS.end(); cit++)
    {
        ok &= SetTlvString(data, *size, &offset, TLV_TYPE_STR_PEERID, cit->first);
        ok &= setRawUInt32(data, *size, &offset, cit->second);
    }

    if(offset != tlvsize){
#ifdef RSSERIAL_DEBUG
        std::cerr << "RsGxsUpdateSerialiser::serialiseGxsMsgUpdate() FAIL Size Error! " << std::endl;
#endif
        ok = false;
    }

#ifdef RSSERIAL_DEBUG
    if (!ok)
    {
        std::cerr << "RsGxsUpdateSerialiser::serialiseGxsMsgUpdate() NOK" << std::endl;
    }
#endif

    return ok;
}

RsGxsMsgUpdateItem* RsGxsUpdateSerialiser::deserialGxsMsgUpdate(void* data,
		uint32_t* size)
{
#ifdef RSSERIAL_DEBUG
    std::cerr << "RsGxsUpdateSerialiser::deserialGxsMsgUpdate()" << std::endl;
#endif
    /* get the type and size */
    uint32_t rstype = getRsItemId(data);
    uint32_t rssize = getRsItemSize(data);

    uint32_t offset = 0;


    if ((RS_PKT_VERSION_SERVICE != getRsItemVersion(rstype)) ||
            (SERVICE_TYPE != getRsItemService(rstype)) ||
            (RS_PKT_SUBTYPE_NXS_SYNC_MSG != getRsItemSubType(rstype)))
    {
#ifdef RSSERIAL_DEBUG
            std::cerr << "RsGxsUpdateSerialiser::deserialGxsMsgUpdate() FAIL wrong type" << std::endl;
#endif
            return NULL; /* wrong type */
    }

    if (*size < rssize)    /* check size */
    {
#ifdef RSSERIAL_DEBUG
            std::cerr << "RsGxsUpdateSerialiser::deserialGxsMsgUpdate() FAIL wrong size" << std::endl;
#endif
            return NULL; /* not enough data */
    }

    /* set the packet length */
    *size = rssize;

    bool ok = true;

    RsGxsMsgUpdateItem* item = new RsGxsMsgUpdateItem(getRsItemService(rstype));

    /* skip the header */
    offset += 8;

    ok &= GetTlvString(data, *size, &offset, TLV_TYPE_STR_HASH_SHA1, item->peerId);
    uint32_t numUpdateItems;
    ok &= getRawUInt32(data, *size, &offset, &(numUpdateItems));
    std::map<std::string, uint32_t>& msgUpdateItem = item->grpUpdateTS;
    std::string grpId;
    uint32_t updateTS;
    for(uint32_t i; i < numUpdateItems; i++)
    {
        ok &= GetTlvString(data, *size, &offset, TLV_TYPE_STR_GROUPID, grpId);

        if(!ok)
            break;

        ok &= getRawUInt32(data, *size, &offset, &(updateTS));

        if(!ok)
            break;

        msgUpdateItem.insert(std::make_pair(grpId, updateTS));                
    }

    if (offset != rssize)
    {
#ifdef RSSERIAL_DEBUG
            std::cerr << "RsGxsUpdateSerialiser::deserialGxsMsgUpdate() FAIL size mismatch" << std::endl;
#endif
            /* error */
            delete item;
            return NULL;
    }

    if (!ok)
    {
#ifdef RSSERIAL_DEBUG
            std::cerr << "RsGxsUpdateSerialiser::deserialGxsMsgUpdate() NOK" << std::endl;
#endif
            delete item;
            return NULL;
    }

    return item;
}
