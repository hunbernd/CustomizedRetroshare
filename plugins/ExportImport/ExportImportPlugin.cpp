#include "ExportImportPlugin.h"
#include "ExportImportPage.h"
#include <QMessageBox>

//static void *inited = new ExportToolsPlugin() ;

extern "C" {
void *RETROSHARE_PLUGIN_provide()
{
    static ExportImportPlugin* p = new ExportImportPlugin() ;
    return (void*)p ;
}
// This symbol contains the svn revision number grabbed from the executable.
// It will be tested by RS to load the plugin automatically, since it is safe to load plugins
// with same revision numbers, assuming that the revision numbers are up-to-date.
//
uint32_t RETROSHARE_PLUGIN_revision = RsUtil::retroshareRevision() ;

// This symbol contains the svn revision number grabbed from the executable.
// It will be tested by RS to load the plugin automatically, since it is safe to load plugins
// with same revision numbers, assuming that the revision numbers are up-to-date.
//
uint32_t RETROSHARE_PLUGIN_api = RS_PLUGIN_API_VERSION ;
}

ExportImportPlugin::ExportImportPlugin()
{
    Q_INIT_RESOURCE(images);
    mIcon = new QIcon(":/images/icon.png");
}

MainPage* ExportImportPlugin::qt_page() const{
    ExportImportPage* ep = new ExportImportPage(mPeers);
    return ep;
}

ConfigPage* ExportImportPlugin::qt_config_page() const{
    return NULL;
}

QDialog* ExportImportPlugin::qt_about_page() const
{
    static QMessageBox *about_dialog = NULL ;

    if(about_dialog == NULL)
    {
        about_dialog = new QMessageBox() ;

        QString text ;
        text += QObject::tr("<h3>RetroShare ExportImport plugin</h3><br/>   * Contributors: Nyfor<br/>") ;
        text += QObject::tr("<br/><br/>This is an experimental feature.") ;

        about_dialog->setText(text) ;
        about_dialog->setStandardButtons(QMessageBox::Ok) ;
    }

    return about_dialog ;
}

QIcon* ExportImportPlugin::qt_icon() const
{
    return mIcon;
}

std::string ExportImportPlugin::getShortPluginDescription() const{
    return "This plugin exports and imports your friends' keys.";
}
std::string ExportImportPlugin::getPluginName() const {
    return "ExportImport";
}
void ExportImportPlugin::getPluginVersion(int& major,int& minor,int& svn_rev) const{
    major = 0;
    minor = 0;
    svn_rev = RsUtil::retroshareRevision() ;
}

//
//========================== Plugin Interface ================================//
//
// Use these methods to access main objects from RetroShare.
//
void ExportImportPlugin::setInterfaces(RsPlugInInterfaces& interfaces){
    mPeers = interfaces.mPeers;
}

void ExportImportPlugin::setPlugInHandler(RsPluginHandler* pgHandler){
    mPlugInHandler = pgHandler;
}
