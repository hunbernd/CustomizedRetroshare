#ifndef EXPORTIMPORTMANAGER_H
#define EXPORTIMPORTMANAGER_H

#include <retroshare/rsplugin.h>

class ExportImportManager
{
public:
    ExportImportManager(RsPeers* mPeers);
    std::string exportJson();
    void importData(const std::string& json_string);
    int getExportCount(){return export_count;}
private:
    RsPeers* mPeers;
    bool import_groups;
    int export_count;
};

#endif // EXPORTIMPORTMANAGER_H
