#ifndef ZERORESERVEPLUGIN_H
#define ZERORESERVEPLUGIN_H

#include <retroshare/rsplugin.h>
#include <retroshare-gui/mainpage.h>

class p3ExampleRS;

class ExamplePlugin: public RsPlugin
{
public:
    ExamplePlugin() ;
    virtual ~ExamplePlugin() {}

    virtual MainPage       *qt_page()       			const	;
    virtual QIcon          *qt_icon()       			const	;
    virtual QTranslator    *qt_translator(QApplication *app, const QString& languageCode, const QString& externalDir) const;
    virtual QDialog * qt_about_page() const;
    virtual void stop() const;

    virtual void getPluginVersion(int& major,int& minor,int& svn_rev) const ;
    virtual void setPlugInHandler(RsPluginHandler *pgHandler);

    virtual std::string configurationFileName() const { return "example.cfg" ; }

    virtual std::string getShortPluginDescription() const ;
    virtual std::string getPluginName() const;
    virtual RsPQIService * rs_pqi_service() const;
    virtual void setInterfaces(RsPlugInInterfaces& interfaces);
private:
    mutable RsPluginHandler *mPlugInHandler;
    mutable MainPage* mainpage ;
    mutable QIcon* mIcon ;
    mutable RsPeers* m_peers;
    mutable p3ExampleRS * m_Example;

};

extern ExamplePlugin * g_ExamplePlugin;


#endif

