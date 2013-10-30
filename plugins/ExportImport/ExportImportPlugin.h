#ifndef EXPORTTOOLSPLUGIN_H
#define EXPORTTOOLSPLUGIN_H

#include <retroshare/rsplugin.h>

class ExportImportPlugin : public RsPlugin
{
public:
    ExportImportPlugin();

    MainPage* qt_page()const;
    ConfigPage* qt_config_page() const;
    QDialog* qt_about_page() const;
    QIcon* qt_icon() const;

    std::string getShortPluginDescription() const;
    std::string getPluginName() const;
    void getPluginVersion(int& major,int& minor,int& svn_rev) const;

    //
    //========================== Plugin Interface ================================//
    //
    // Use these methods to access main objects from RetroShare.
    //
    void setInterfaces(RsPlugInInterfaces& interfaces);
    void setPlugInHandler(RsPluginHandler* pgHandler);

private:
    RsPeers* mPeers;
    RsPluginHandler* mPlugInHandler;
    QIcon* mIcon;
};

#endif // EXPORTTOOLSPLUGIN_H
