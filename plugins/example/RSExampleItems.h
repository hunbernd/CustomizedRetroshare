#ifndef RSEXAMPLEITEMS_H
#define RSEXAMPLEITEMS_H

#include "serialiser/rsserial.h"
#include <string>



const uint8_t QOS_PRIORITY_RS_EXAMPLE = 3;
extern const uint16_t RS_SERVICE_TYPE_EXAMPLE_PLUGIN;
extern const uint32_t CONFIG_TYPE_EXAMPLE_PLUGIN;


class RsExampleItem: public RsItem
{
public:
    RsExampleItem( )
        : RsItem( RS_PKT_VERSION_SERVICE,RS_SERVICE_TYPE_EXAMPLE_PLUGIN, EXAMPLE_ITEM )
    {
        setPriorityLevel(QOS_PRIORITY_RS_EXAMPLE);
        m_msg = "Hello World";
    }
    RsExampleItem(void *data, uint32_t pktsize);

    enum RS_PKT_SUBTYPE {
        EXAMPLE_ITEM = 0x01
        // add any number of item types here
    };

    virtual ~RsExampleItem() {};
    virtual void clear() {};
    virtual std::ostream& print(std::ostream &out, uint16_t indent = 0);

    virtual bool serialise(void *data,uint32_t& size);
    virtual uint32_t serial_size() const;

    const std::string & getMessage(){ return m_msg; }
private:
    std::string m_msg;
};


class RsExampleSerialiser: public RsSerialType
{
public:
    RsExampleSerialiser()
        :RsSerialType(RS_PKT_VERSION_SERVICE, RS_SERVICE_TYPE_EXAMPLE_PLUGIN)
    {
    }
    virtual ~RsExampleSerialiser() {}

    virtual uint32_t 	size (RsItem *item)
    {
        return dynamic_cast<RsExampleItem *>(item)->serial_size() ;
    }

    virtual	bool serialise  (RsItem *item, void *data, uint32_t *size)
    {
        return dynamic_cast<RsExampleItem *>(item)->serialise(data,*size) ;
    }
    virtual	RsItem *deserialise(void *data, uint32_t *size);
};

#endif // RSEXAMPLEITEMS_H
