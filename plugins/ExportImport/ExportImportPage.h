#ifndef EXPORTPAGE_H
#define EXPORTPAGE_H

#include <retroshare-gui/mainpage.h>
#include <retroshare/rsplugin.h>
#include <retroshare/rsiface.h>
#include "ui_ExportImportPage.h"

class ExportImportPage : public MainPage
{
    Q_OBJECT
    
public:
    ExportImportPage(RsPeers* mPeers, QWidget* parent = 0, Qt::WFlags flags = 0);
    ~ExportImportPage();
    void load(){}
    bool save(QString& str){return true;}
    QString pageName()const{
        return "ExportTools";
    }
    QPixmap iconPixmap() const { return QPixmap(":/images/encrypted32.png") ; }
private:
    /** Qt Designer generated object */
    Ui::ExportPage ui;
    RsPeers* mPeers;
    void exportKeys();
    void importKeys();
    ServicePermissionFlags serviceFlags() const;

private slots:
    void setImportFile();
    void setExportFile();

};

#endif // EXPORTPAGE_H
