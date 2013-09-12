#include "ExportImportPage.h"
#include "ExportImportManager.h"
#include "ui_ExportImportPage.h"
#include "ExportImportPlugin.h"
#include <QMessageBox>
#include <QFileDialog>
#include <QTextStream>

//#define DEBUG
#define SEPARATOR ";"

ExportImportPage::ExportImportPage(RsPeers* mPeers, QWidget* parent, Qt::WFlags flags)
    : MainPage(parent, flags)
{
    this->mPeers = mPeers;

    /* Invoke the Qt Designer generated object setup routine */
    ui.setupUi(this);

    connect(ui.importDirButton, SIGNAL(clicked()), this, SLOT(setImportFile()));
    connect(ui.exportDirButton, SIGNAL(clicked()), this, SLOT(setExportFile()));
}

ExportImportPage::~ExportImportPage(){}

void ExportImportPage::exportKeys(){
    if(ui.exportFilenameLine->text().length() > 0){
        QFile certFile(ui.exportFilenameLine->text());
        if (certFile.open(QIODevice::WriteOnly | QIODevice::Text)) {
            QTextStream out(&certFile);
            ExportImportManager exporter(mPeers);
            out<<exporter.exportJson().c_str();
            certFile.close();
            QMessageBox::information(this,
                                     tr("RetroShare"),
                                     QString(tr("%1 keys exported!")).arg(exporter.getExportCount()));
        } else {
            QMessageBox::information(this,
                                     tr("RetroShare"),
                                     QString(tr("Export failed, file open failed!")));
        }
    } else {
        QMessageBox::information(this,
                                 tr("RetroShare"),
                                 QString(tr("Export failed, no filepath set")));
    }
}

void ExportImportPage::importKeys(){
    QString fn = ui.importFilepath->text();
    if (QFile::exists(fn)) {
        std::string certFileStr;
        QFile CertFile(fn);
        if (CertFile.open(QIODevice::ReadOnly | QIODevice::Text)) {
            certFileStr = QString(CertFile.readAll()).toStdString();
            CertFile.close();
        }

        if (certFileStr.empty()) {
            QMessageBox::information(this,
                                     tr("RetroShare"),
                                     QString(tr("Certificate Load Failed:can't read from file %1 ")).arg(fn));
        } else {
            ExportImportManager importer(mPeers);
            importer.importData(certFileStr);
        }
    } else {
        QMessageBox::information(this,
                                 tr("RetroShare"),
                                 QString(tr("Certificate Load Failed:file %1 not found")).arg(fn));
    }
}

void ExportImportPage::setImportFile()
{
    QString qdir = QFileDialog::getOpenFileName(this, tr("Set Import File"));
    ui.importFilepath->setText(qdir);
    if(!qdir.isEmpty()){
        importKeys();
    }
}

void ExportImportPage::setExportFile()
{
    QString qdir = QFileDialog::getSaveFileName(this, tr("Set Export File"));
    ui.exportFilenameLine->setText(qdir);
    if(!qdir.isEmpty()){
        exportKeys();
    }
}
