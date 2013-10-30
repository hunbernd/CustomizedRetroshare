#include "ExportImportManager.h"
#include <QtGlobal>
#include "json/json.h"

ExportImportManager::ExportImportManager(RsPeers* mPeers)
{
    this->mPeers = mPeers;
    this->import_groups = false;
}

std::string ExportImportManager::exportJson(){
    std::list<std::string> gpg_ids;
    mPeers->getGPGAcceptedList(gpg_ids);
    //mPeers->getGPGAllList(gpg_ids);

    Json::Value root;
    std::list<RsGroupInfo> group_info_list;
    mPeers->getGroupInfoList(group_info_list);
    Json::Value json_groups;
    for(std::list<RsGroupInfo>::iterator list_iter = group_info_list.begin(); list_iter !=  group_info_list.end(); list_iter++)
    {
        Json::Value json_group;
        RsGroupInfo group_info= *list_iter;
        json_group["id"] = group_info.id;
        json_group["name"] = group_info.name;
        json_group["flag"] = group_info.flag;
        Json::Value json_peer_ids;
        for(std::list<std::string>::iterator i = group_info.peerIds.begin(); i !=  group_info.peerIds.end(); i++){
            json_peer_ids.append(*i);
        }
        json_group["peerIds"] = json_peer_ids;
        json_groups.append(json_group);
    }
    root["groups"] = json_groups;

    Json::Value json_gpg_ids;
    RsPeerDetails gpg_detail;
    for(std::list<std::string>::iterator list_iter = gpg_ids.begin(); list_iter !=  gpg_ids.end(); list_iter++)
    {
        mPeers->getGPGDetails(*list_iter, gpg_detail);
        Json::Value json_gpg_id;
        //json_gpg_id["id"] = gpg_detail.id;
        json_gpg_id["name"] = gpg_detail.name;
        json_gpg_id["gpg_id"] = gpg_detail.gpg_id;
        json_gpg_id["email"] = gpg_detail.email;
        json_gpg_id["trustLvl"] = gpg_detail.trustLvl;
        //json_gpg_id["validLvl"] = gpg_detail.validLvl;
        //json_gpg_id["location"] = gpg_detail.location;
        //json_gpg_id["service_perm_flags"] = gpg_detail.service_perm_flags.toUInt32();
        std::list<std::string> ssl_ids;
        mPeers->getAssociatedSSLIds(*list_iter, ssl_ids);
        Json::Value json_ssl_ids;
        for(std::list<std::string>::iterator list_iter = ssl_ids.begin(); list_iter !=  ssl_ids.end(); list_iter++)
        {
            RsPeerDetails detail;
            if (!mPeers->getPeerDetails(*list_iter, detail))
            {
                std::cerr<<("Error : cannot get peer details.");
                return NULL;
            }
            //std::string invite = mPeers->GetRetroshareInvite(detail.id,ui._shouldAddSignatures_CB->isChecked(),ui._useOldFormat_CB->isChecked()) ; // this needs to be a SSL id
            std::string invite = mPeers->GetRetroshareInvite(detail.id,true,false) ; // this needs to be a SSL id

            Json::Value json_ssl_id;
            json_ssl_id["id"] = detail.id.c_str();
            json_ssl_id["location"] = detail.location;
            json_ssl_id["pubkey"] = invite.c_str();
            json_ssl_id["service_perm_flags"] = detail.service_perm_flags.toUInt32();
            json_ssl_ids.append(json_ssl_id);
        }
        json_gpg_id["ssl_ids"] = json_ssl_ids;
        json_gpg_ids.append(json_gpg_id);
    }
    root["gpg_ids"] = json_gpg_ids;
    this->export_count = json_gpg_ids.size();
    Json::StyledWriter writer;
    return writer.write(root);
}

void ExportImportManager::importData(const std::string &certFileStr)
{
    Json::Value root;
    Json::Reader reader;
    if (reader.parse(certFileStr, root ))
    {
        Json::Value gpg_ids = root["gpg_ids"];
        foreach(const Json::Value& gpg_id, gpg_ids)
        {
            Json::Value json_ssl_ids = gpg_id["ssl_ids"];
            foreach(const Json::Value& json_ssl_id, json_ssl_ids){
                std::string certStr = json_ssl_id["pubkey"].asString();
/*
#if SVN_REVISION_NUMBER <= 5995
                std::string
#else
                uint32_t
#endif
*/
uint32_t cert_error_code;
std::string haha;
                RsPeerDetails peerDetails;
                if (mPeers->loadDetailsFromStringCert(certStr, peerDetails, cert_error_code)) {
                    std::string ssl_id, pgp_id ;
                    if(!mPeers->loadCertificateFromString(certStr,ssl_id,pgp_id, haha))
                    {
                        std::cerr << "ConnectFriendWizard::accept(): cannot load that certificate." << std::endl;
                    } else {
                        ServicePermissionFlags service_perm_flags(json_ssl_id["service_perm_flags"].asUInt());
                        if (!peerDetails.gpg_id.empty()) {

                            std::cerr << "ConclusionPage::validatePage() accepting GPG key for connection." << std::endl;
                            mPeers->addFriend("", peerDetails.gpg_id, service_perm_flags);
                        }

                        if (peerDetails.id != "") {
                            mPeers->addFriend(peerDetails.id, peerDetails.gpg_id, service_perm_flags) ;

                            //let's check if there is ip adresses in the wizard.
                            if (!peerDetails.extAddr.empty() && peerDetails.extPort) {
                                std::cerr << "ConnectFriendWizard::accept() : setting ip ext address." << std::endl;
                                mPeers->setExtAddress(peerDetails.id, peerDetails.extAddr, peerDetails.extPort);
                            }
                            if (!peerDetails.localAddr.empty() && peerDetails.localPort) {
                                std::cerr << "ConnectFriendWizard::accept() : setting ip local address." << std::endl;
                                mPeers->setLocalAddress(peerDetails.id, peerDetails.localAddr, peerDetails.localPort);
                            }
                            if (!peerDetails.dyndns.empty()) {
                                std::cerr << "ConnectFriendWizard::accept() : setting DynDNS." << std::endl;
                                mPeers->setDynDNS(peerDetails.id, peerDetails.dyndns);
                            }
                            if (!peerDetails.location.empty()) {
                                std::cerr << "ConnectFriendWizard::accept() : setting peerLocation." << std::endl;
                                mPeers->setLocation(peerDetails.id, peerDetails.location);
                            }
                        }

                        //rsicontrol->getNotify().notifyListChange(NOTIFY_LIST_NEIGHBOURS,1) ;
                    }
                } else {
                    //setField("errorMessage", QString(tr("Certificate Load Failed:something is wrong with %1 ")).arg(fn) + ": " + getErrorString(cert_error_code));
                }
            }
            mPeers->trustGPGCertificate(gpg_id["gpg_id"].asString(), gpg_id["trustLvl"].asUInt());
        }
        if(this->import_groups){
            Json::Value json_groups = root["groups"];
            foreach(const Json::Value& json_group, json_groups)
            {
                std::string groupId = json_group["id"].asString();
                Json::Value json_peers = json_group["peerIds"];
                if (!groupId.empty()) {
                    RsGroupInfo groupInfo;
                    if(!mPeers->getGroupInfo(groupId, groupInfo)){
                        groupInfo.id = json_group["id"].asString();
                        groupInfo.name = json_group["name"].asString();
                        groupInfo.flag = json_group["flag"].asUInt();
                        mPeers->addGroup(groupInfo);
                    }
                    foreach(const Json::Value& json_peer, json_peers){
                        mPeers->assignPeerToGroup(groupId, json_peer.asString(), true);

                    }
                }
            }
        }
    }
}
