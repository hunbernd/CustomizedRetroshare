
#include "ExamplePlugin.h"
#include "ExampleDialog.h"

#include "p3ExampleRS.h"


#include <retroshare/rsplugin.h>
#include <util/rsversion.h>
#include <QTranslator>
#include <QMessageBox>


ExamplePlugin * g_ExamplePlugin;

extern "C" {
	void *RETROSHARE_PLUGIN_provide()
	{
        g_ExamplePlugin = new ExamplePlugin() ;
        return (void*) g_ExamplePlugin;
	}
	// This symbol contains the svn revision number grabbed from the executable. 
	// It will be tested by RS to load the plugin automatically, since it is safe to load plugins
	// with same revision numbers, assuming that the revision numbers are up-to-date.
	//
	uint32_t RETROSHARE_PLUGIN_revision = SVN_REVISION_NUMBER ;

	// This symbol contains the svn revision number grabbed from the executable. 
	// It will be tested by RS to load the plugin automatically, since it is safe to load plugins
	// with same revision numbers, assuming that the revision numbers are up-to-date.
	//
	uint32_t RETROSHARE_PLUGIN_api = RS_PLUGIN_API_VERSION ;
}

#define IMAGE_LINKS ":/images/bitcoin.png"

void ExamplePlugin::getPluginVersion(int& major,int& minor,int& svn_rev) const
{
    major = 5 ;
    minor = 4 ;
    svn_rev = SVN_REVISION_NUMBER ;
}

ExamplePlugin::ExamplePlugin()
{
    mainpage = NULL ;
    mIcon = NULL ;
    mPlugInHandler = NULL;
    m_Example = NULL;
    m_peers = NULL;

}

void ExamplePlugin::setInterfaces(RsPlugInInterfaces &interfaces)
{
    m_peers = interfaces.mPeers;
}

MainPage *ExamplePlugin::qt_page() const
{
    if(mainpage == NULL){
        mainpage = new ExampleDialog( );
    }

    return mainpage ;
}


void ExamplePlugin::setPlugInHandler(RsPluginHandler *pgHandler)
{
    mPlugInHandler = pgHandler;
}

QIcon *ExamplePlugin::qt_icon() const
{
    if(mIcon == NULL)
    {
        Q_INIT_RESOURCE(Example_images) ;

        mIcon = new QIcon(IMAGE_LINKS) ;
    }

    return mIcon ;
}

RsPQIService * ExamplePlugin::rs_pqi_service() const
{
    if(m_Example == NULL){
        m_Example = new p3ExampleRS(mPlugInHandler, m_peers ) ;
    }

    return m_Example ;
}

std::string ExamplePlugin::getShortPluginDescription() const
{
    return QApplication::translate("ExamplePlugin", "This plugin says \"hello\"").toUtf8().constData();
}

std::string ExamplePlugin::getPluginName() const
{
    return QApplication::translate("ExamplePlugin", "Example" ).toUtf8().constData();
}

QTranslator* ExamplePlugin::qt_translator(QApplication */*app*/, const QString& languageCode, const QString& externalDir) const
{
    if (languageCode == "en") {
        return NULL;
    }

    QTranslator* translator = new QTranslator();

    if (translator->load(externalDir + "/Example_" + languageCode + ".qm")) {
        return translator;
    } else if (translator->load(":/lang/Example_" + languageCode + ".qm")) {
        return translator;
    }

    delete(translator);
    return NULL;
}

QDialog * ExamplePlugin::qt_about_page() const
{
    static QMessageBox *about_dialog = NULL ;

    if(about_dialog == NULL)
    {
        about_dialog = new QMessageBox() ;

        QString text ;
        text += QObject::tr("<h3>RetroShare Example plugin</h3><br/>   * This example code is in the public domain<br/>" ) ;
        text += QObject::tr("<br/>Saying hello to each friend coming on-line") ;

        about_dialog->setText(text) ;
        about_dialog->setStandardButtons(QMessageBox::Ok) ;
    }

    return about_dialog ;
}

void ExamplePlugin::stop() const
{
    std::cerr << "Example Plugin: Closing Database" << std::endl;

}
