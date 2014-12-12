#include <QTreeWidget>
#include <QTextBrowser>
#include <QTimer>
#include <QMenu>
#include <QMessageBox>
#include <time.h>
#include "ChatLobbyWidget.h"
#include "chat/CreateLobbyDialog.h"
#include "chat/ChatTabWidget.h"
#include "common/RSTreeWidgetItem.h"
#include "notifyqt.h"
#include "chat/ChatLobbyDialog.h"
#include "chat/ChatLobbyUserNotify.h"
#include "util/HandleRichText.h"
#include "util/QtVersion.h"
#include <gui/settings/rsharesettings.h>

#include "retroshare/rsmsgs.h"
#include "retroshare/rspeers.h"
#include "retroshare/rsnotify.h"

#define COLUMN_NAME       0
#define COLUMN_USER_COUNT 1
#define COLUMN_TOPIC      2
#define COLUMN_SUBSCRIBED 3
#define COLUMN_COUNT      4

#define COLUMN_DATA       0
#define ROLE_SORT         Qt::UserRole
#define ROLE_ID           Qt::UserRole + 1
#define ROLE_SUBSCRIBED   Qt::UserRole + 2
#define ROLE_PRIVACYLEVEL Qt::UserRole + 3
#define ROLE_AUTOSUBSCRIBE Qt::UserRole + 4


#define TYPE_FOLDER       0
#define TYPE_LOBBY        1

#define IMAGE_CREATE          ""
#define IMAGE_PUBLIC          ":/images/chat_x24.png"
#define IMAGE_PRIVATE         ":/images/chat_red24.png"
#define IMAGE_SUBSCRIBE       ":/images/edit_add24.png"  
#define IMAGE_UNSUBSCRIBE     ":/images/cancel.png"
#define IMAGE_PEER_ENTERING   ":images/user/add_user24.png"
#define IMAGE_PEER_LEAVING    ":images/user/remove_user24.png"
#define IMAGE_TYPING		      ":images/typing.png" 
#define IMAGE_MESSAGE	      ":images/chat.png" 
#define IMAGE_AUTOSUBSCRIBE   ":images/accepted16.png"

ChatLobbyWidget::ChatLobbyWidget(QWidget *parent, Qt::WindowFlags flags)
  : RsAutoUpdatePage(5000, parent, flags)
{
	ui.setupUi(this);

	m_bProcessSettings = false;

	QObject::connect( NotifyQt::getInstance(), SIGNAL(lobbyListChanged()), SLOT(lobbyChanged()));
	QObject::connect( NotifyQt::getInstance(), SIGNAL(chatLobbyEvent(qulonglong,int,const QString&,const QString&)), this, SLOT(displayChatLobbyEvent(qulonglong,int,const QString&,const QString&)));
	QObject::connect( NotifyQt::getInstance(), SIGNAL(chatLobbyInviteReceived()), this, SLOT(readChatLobbyInvites()));

	QObject::connect( ui.lobbyTreeWidget, SIGNAL(customContextMenuRequested(QPoint)), this, SLOT(lobbyTreeWidgetCustomPopupMenu(QPoint)));
	QObject::connect( ui.lobbyTreeWidget, SIGNAL(itemDoubleClicked(QTreeWidgetItem*,int)), this, SLOT(itemDoubleClicked(QTreeWidgetItem*,int)));
	QObject::connect( ui.lobbyTreeWidget, SIGNAL(itemSelectionChanged()), this, SLOT(updateCurrentLobby()));

	QObject::connect( ui.filterLineEdit, SIGNAL(textChanged(const QString &)), this, SLOT(filterItems(QString)));
	QObject::connect( ui.filterLineEdit, SIGNAL(filterChanged(int)), this, SLOT(filterColumnChanged(int)));
	QObject::connect( ui.createlobbytoolButton, SIGNAL(clicked()), this, SLOT(createChatLobby()));

	compareRole = new RSTreeWidgetItemCompareRole;
	compareRole->setRole(COLUMN_NAME, ROLE_SORT);

	ui.lobbyTreeWidget->setColumnCount(COLUMN_COUNT);
	ui.lobbyTreeWidget->sortItems(COLUMN_NAME, Qt::AscendingOrder);

	ui.lobbyTreeWidget->setContextMenuPolicy(Qt::CustomContextMenu) ;

	QTreeWidgetItem *headerItem = ui.lobbyTreeWidget->headerItem();
	headerItem->setText(COLUMN_NAME, tr("Name"));
	headerItem->setText(COLUMN_USER_COUNT, tr("Count"));
	headerItem->setText(COLUMN_TOPIC, tr("Topic"));
    headerItem->setText(COLUMN_SUBSCRIBED, tr("Subscribed"));
	headerItem->setTextAlignment(COLUMN_NAME, Qt::AlignHCenter | Qt::AlignVCenter);
	headerItem->setTextAlignment(COLUMN_TOPIC, Qt::AlignHCenter | Qt::AlignVCenter);
	headerItem->setTextAlignment(COLUMN_USER_COUNT, Qt::AlignHCenter | Qt::AlignVCenter);
	headerItem->setTextAlignment(COLUMN_SUBSCRIBED, Qt::AlignHCenter | Qt::AlignVCenter);

	QHeaderView *header = ui.lobbyTreeWidget->header();
	QHeaderView_setSectionResizeMode(header, COLUMN_NAME, QHeaderView::Interactive);
	QHeaderView_setSectionResizeMode(header, COLUMN_USER_COUNT, QHeaderView::Interactive);
	QHeaderView_setSectionResizeMode(header, COLUMN_TOPIC, QHeaderView::Interactive);
    QHeaderView_setSectionResizeMode(header, COLUMN_SUBSCRIBED, QHeaderView::Interactive);

    privateSubLobbyItem = new RSTreeWidgetItem(compareRole, TYPE_FOLDER);
    privateSubLobbyItem->setText(COLUMN_NAME, tr("Private Subscribed Lobbies"));
	privateSubLobbyItem->setData(COLUMN_NAME, ROLE_SORT, "1");
	//	privateLobbyItem->setIcon(COLUMN_NAME, QIcon(IMAGE_PRIVATE));
	privateSubLobbyItem->setData(COLUMN_DATA, ROLE_PRIVACYLEVEL, RS_CHAT_LOBBY_PRIVACY_LEVEL_PRIVATE);
	ui.lobbyTreeWidget->insertTopLevelItem(0, privateSubLobbyItem);

	publicSubLobbyItem = new RSTreeWidgetItem(compareRole, TYPE_FOLDER);
	publicSubLobbyItem->setText(COLUMN_NAME, tr("Public Subscribed Lobbies"));
	publicSubLobbyItem->setData(COLUMN_NAME, ROLE_SORT, "2");
	//	publicLobbyItem->setIcon(COLUMN_NAME, QIcon(IMAGE_PUBLIC));
	publicSubLobbyItem->setData(COLUMN_DATA, ROLE_PRIVACYLEVEL, RS_CHAT_LOBBY_PRIVACY_LEVEL_PUBLIC);
	ui.lobbyTreeWidget->insertTopLevelItem(1, publicSubLobbyItem);

	privateLobbyItem = new RSTreeWidgetItem(compareRole, TYPE_FOLDER);
	privateLobbyItem->setText(COLUMN_NAME, tr("Private Lobbies"));
	privateLobbyItem->setData(COLUMN_NAME, ROLE_SORT, "3");
	//	privateLobbyItem->setIcon(COLUMN_NAME, QIcon(IMAGE_PRIVATE));
	privateLobbyItem->setData(COLUMN_DATA, ROLE_PRIVACYLEVEL, RS_CHAT_LOBBY_PRIVACY_LEVEL_PRIVATE);
	ui.lobbyTreeWidget->insertTopLevelItem(2, privateLobbyItem);

	publicLobbyItem = new RSTreeWidgetItem(compareRole, TYPE_FOLDER);
	publicLobbyItem->setText(COLUMN_NAME, tr("Public Lobbies"));
	publicLobbyItem->setData(COLUMN_NAME, ROLE_SORT, "4");
	//	publicLobbyItem->setIcon(COLUMN_NAME, QIcon(IMAGE_PUBLIC));
	publicLobbyItem->setData(COLUMN_DATA, ROLE_PRIVACYLEVEL, RS_CHAT_LOBBY_PRIVACY_LEVEL_PUBLIC);
	ui.lobbyTreeWidget->insertTopLevelItem(3, publicLobbyItem);

	ui.lobbyTreeWidget->expandAll();
	ui.lobbyTreeWidget->setColumnHidden(COLUMN_NAME,false) ;
	ui.lobbyTreeWidget->setColumnHidden(COLUMN_USER_COUNT,true) ;
	ui.lobbyTreeWidget->setColumnHidden(COLUMN_TOPIC,true) ;
	ui.lobbyTreeWidget->setColumnHidden(COLUMN_SUBSCRIBED,true) ;
	ui.lobbyTreeWidget->setSortingEnabled(true) ;

	ui.lobbyTreeWidget->adjustSize();
	ui.lobbyTreeWidget->setColumnWidth(COLUMN_NAME,100);
	ui.lobbyTreeWidget->setColumnWidth(COLUMN_USER_COUNT, 50);
	ui.lobbyTreeWidget->setColumnWidth(COLUMN_TOPIC, 50);

	/** Setup the actions for the header context menu */
	showUserCountAct= new QAction(headerItem->text(COLUMN_USER_COUNT),this);
    showUserCountAct->setCheckable(true); showUserCountAct->setToolTip(tr("Show ")+showUserCountAct->text()+tr(" Column"));
    connect(showUserCountAct,SIGNAL(triggered(bool)),this,SLOT(setShowUserCountColumn(bool))) ;
    showTopicAct= new QAction(headerItem->text(COLUMN_TOPIC),this);
    showTopicAct->setCheckable(true); showTopicAct->setToolTip(tr("Show ")+showTopicAct->text()+tr(" Column"));
    connect(showTopicAct,SIGNAL(triggered(bool)),this,SLOT(setShowTopicColumn(bool))) ;
    showSubscribeAct= new QAction(headerItem->text(COLUMN_SUBSCRIBED),this);
    showSubscribeAct->setCheckable(true); showSubscribeAct->setToolTip(tr("Show ")+showSubscribeAct->text()+tr(" Column"));
    connect(showSubscribeAct,SIGNAL(triggered(bool)),this,SLOT(setShowSubscribeColumn(bool))) ;

	// Set initial size of the splitter
	QList<int> sizes;
	sizes << 200 << width(); // Qt calculates the right sizes
	ui.splitter->setSizes(sizes);

	lobbyChanged();
	showBlankPage(0) ;

	/* add filter actions */
	ui.filterLineEdit->addFilter(QIcon(), tr("Name"), COLUMN_NAME, tr("Search Name"));
	ui.filterLineEdit->setCurrentFilter(COLUMN_NAME);

	// load settings
	processSettings(true);

	QString help_str = tr("\
	                      <h1><img width=\"32\" src=\":/images/64px_help.png\">&nbsp;&nbsp;Chat Lobbies</h1>                              \
	                      <p>Chat lobbies are distributed chat rooms, and work pretty much like IRC.                                      \
	                      They allow you to talk anonymously with tons of people without the need to make friends.</p>                    \
	                      <p>A chat lobby can be public (your friends see it) or private (your friends can't see it, unless you           \
	                      invite them with <img src=\":/images/add_24x24.png\" width=12/>). \
	                      Once you have been invited to a private lobby, you will be able to see it when your friends   \
	                      are using it.</p>                                                                                               \
	                      <p>The list at left shows                                                                                     \
	                      chat lobbies your friends are participating in. You can either                                 \
		  <ul>                                                                                                            \
			  <li>Right click to create a new chat lobby</li>                                                              \
		     <li>Double click a chat lobby to enter, chat, and show it to your friends</li>                      \
	                      </ul> \
	                      Note: For the chat lobbies to work properly, your computer needs be on time.  So check your system clock!\
	                      </p>                                      \
	                      "
	                      ) ;
	    registerHelpButton(ui.helpButton,help_str) ;
}

ChatLobbyWidget::~ChatLobbyWidget()
{
    // save settings
    processSettings(false);

	if (compareRole) {
		delete(compareRole);
	}
}

UserNotify *ChatLobbyWidget::getUserNotify(QObject *parent)
{
	ChatLobbyUserNotify *notify = new ChatLobbyUserNotify(parent);
	connect(this, SIGNAL(unreadCountChanged(uint)), notify, SLOT(unreadCountChanged(uint)));

	notify->unreadCountChanged(unreadCount());

	return notify;
}

void ChatLobbyWidget::lobbyTreeWidgetCustomPopupMenu(QPoint)
{
	std::cerr << "Creating customPopupMennu" << std::endl;
	QTreeWidgetItem *item = ui.lobbyTreeWidget->currentItem();

	QMenu contextMnu(this);

	if (item && item->type() == TYPE_FOLDER) {
		QAction *action = contextMnu.addAction(QIcon(IMAGE_CREATE), tr("Create chat lobby"), this, SLOT(createChatLobby()));
		action->setData(item->data(COLUMN_DATA, ROLE_PRIVACYLEVEL).toInt());
	}

	if (item && item->type() == TYPE_LOBBY) {
		if (item->data(COLUMN_DATA, ROLE_SUBSCRIBED).toBool()) {
			contextMnu.addAction(QIcon(IMAGE_UNSUBSCRIBE), tr("Unsubscribe"), this, SLOT(unsubscribeItem()));
		} else {
			contextMnu.addAction(QIcon(IMAGE_SUBSCRIBE), tr("Subscribe"), this, SLOT(subscribeItem()));
		}
        if (item->data(COLUMN_DATA, ROLE_AUTOSUBSCRIBE).toBool()) {
            contextMnu.addAction(QIcon(IMAGE_AUTOSUBSCRIBE), tr("Remove Auto Subscribe"), this, SLOT(autoSubscribeItem()));
        } else {
            contextMnu.addAction(QIcon(IMAGE_SUBSCRIBE), tr("Add Auto Subscribe"), this, SLOT(autoSubscribeItem()));
        }
	}

	contextMnu.addSeparator();//-------------------------------------------------------------------

	showUserCountAct->setChecked(!ui.lobbyTreeWidget->isColumnHidden(COLUMN_USER_COUNT));
	showTopicAct->setChecked(!ui.lobbyTreeWidget->isColumnHidden(COLUMN_TOPIC));
	showSubscribeAct->setChecked(!ui.lobbyTreeWidget->isColumnHidden(COLUMN_SUBSCRIBED));

	QMenu *menu = contextMnu.addMenu(tr("Columns"));
	menu->addAction(showUserCountAct);
    menu->addAction(showTopicAct);
    menu->addAction(showSubscribeAct);

    contextMnu.exec(QCursor::pos());
}

void ChatLobbyWidget::lobbyChanged()
{
	updateDisplay();
}

static void updateItem(QTreeWidget *treeWidget, QTreeWidgetItem *item, ChatLobbyId id, const std::string &name, const std::string &topic, int count, bool subscribed, bool autoSubscribe)
{
	item->setText(COLUMN_NAME, QString::fromUtf8(name.c_str()));
	item->setData(COLUMN_NAME, ROLE_SORT, QString::fromUtf8(name.c_str()));

	if(topic.empty())
	{
		item->setText(COLUMN_TOPIC, qApp->translate("ChatLobbyWidget", "[No topic provided]"));
		item->setData(COLUMN_TOPIC, ROLE_SORT, qApp->translate("ChatLobbyWidget", "[No topic provided]"));
	}
	else
	{
		item->setText(COLUMN_TOPIC, QString::fromUtf8(topic.c_str()));
		item->setData(COLUMN_TOPIC, ROLE_SORT, QString::fromUtf8(topic.c_str()));
	}

    //item->setText(COLUMN_USER_COUNT, QString::number(count));
    item->setData(COLUMN_USER_COUNT, Qt::EditRole, count);

    item->setText(COLUMN_SUBSCRIBED, subscribed?qApp->translate("ChatLobbyWidget", "Yes"):qApp->translate("ChatLobbyWidget", "No"));

	item->setData(COLUMN_DATA, ROLE_ID, (qulonglong)id);
	item->setData(COLUMN_DATA, ROLE_SUBSCRIBED, subscribed);
    item->setData(COLUMN_DATA, ROLE_AUTOSUBSCRIBE, autoSubscribe);

	QColor color = treeWidget->palette().color(QPalette::Active, QPalette::Text);
	if (!subscribed) {
		// Average between Base and Text colors
		QColor color2 = treeWidget->palette().color(QPalette::Active, QPalette::Base);
		color.setRgbF((color2.redF()+color.redF())/2, (color2.greenF()+color.greenF())/2, (color2.blueF()+color.blueF())/2);
	}

	for (int column = 0; column < COLUMN_COUNT; ++column) {
		item->setTextColor(column, color);
	}
	item->setToolTip(0,QObject::tr("Subject: ")+item->text(COLUMN_TOPIC)+"\n"+QObject::tr("Participants: ")+QString::number(count)+"\n"+QObject::tr("Id: ")+QString::number(id,16)) ;
}

void ChatLobbyWidget::addChatPage(ChatLobbyDialog *d)
{
	// check that the page does not already exist. 

	if(_lobby_infos.find(d->id()) == _lobby_infos.end())
	{
		ui.stackedWidget->addWidget(d) ;

		connect(d,SIGNAL(lobbyLeave(ChatLobbyId)),this,SLOT(unsubscribeChatLobby(ChatLobbyId))) ;
		connect(d,SIGNAL(typingEventReceived(ChatLobbyId)),this,SLOT(updateTypingStatus(ChatLobbyId))) ;
		connect(d,SIGNAL(messageReceived(ChatLobbyId)),this,SLOT(updateMessageChanged(ChatLobbyId))) ;
		connect(d,SIGNAL(peerJoined(ChatLobbyId)),this,SLOT(updatePeerEntering(ChatLobbyId))) ;
		connect(d,SIGNAL(peerLeft(ChatLobbyId)),this,SLOT(updatePeerLeaving(ChatLobbyId))) ;

		ChatLobbyId id = d->id();
		_lobby_infos[id].dialog = d ;
		_lobby_infos[id].default_icon = QIcon() ;
		_lobby_infos[id].last_typing_event = time(NULL) ;
		_lobby_infos[id].unread_count = 0;

		std::list<ChatLobbyInfo> lobbies;
		rsMsgs->getChatLobbyList(lobbies);

		for (std::list<ChatLobbyInfo>::const_iterator it = lobbies.begin(); it != lobbies.end();++it) {
			if (it->lobby_id == id) {
				_lobby_infos[id].default_icon = (it->lobby_privacy_level == RS_CHAT_LOBBY_PRIVACY_LEVEL_PRIVATE) ? QIcon(IMAGE_PRIVATE) : QIcon(IMAGE_PUBLIC);
			}
		}
	}
}

void ChatLobbyWidget::setCurrentChatPage(ChatLobbyDialog *d)
{
	ui.stackedWidget->setCurrentWidget(d) ;

	if (d) {
		QTreeWidgetItem *item = getTreeWidgetItem(d->id());
		if (item) {
			ui.lobbyTreeWidget->setCurrentItem(item);
		}
	}
}

//#define CHAT_LOBBY_GUI_DEBUG
void ChatLobbyWidget::updateDisplay()
{
#ifdef CHAT_LOBBY_GUI_DEBUG
	std::cerr << "updating chat lobby display!" << std::endl;
#endif
	std::vector<VisibleChatLobbyRecord> visibleLobbies;
	rsMsgs->getListOfNearbyChatLobbies(visibleLobbies);

	std::list<ChatLobbyInfo> lobbies;
	rsMsgs->getChatLobbyList(lobbies);

#ifdef CHAT_LOBBY_GUI_DEBUG
	std::cerr << "got " << visibleLobbies.size() << " visible lobbies" << std::endl;
#endif

	// now, do a nice display of lobbies
	
    RsPeerId vpid;
	std::list<ChatLobbyInfo>::const_iterator lobbyIt;

	// remove not existing public lobbies

	for(int p=0;p<4;++p)
	{
		QTreeWidgetItem *lobby_item =NULL;
		switch (p) 
		{
			case 0: lobby_item = privateSubLobbyItem; break;
			case 1: lobby_item = publicSubLobbyItem; break;
			case 2: lobby_item = privateLobbyItem; break;
			default:
			case 3: lobby_item = publicLobbyItem; break;
		}
		//QTreeWidgetItem *lobby_item = (p==0)?publicLobbyItem:privateLobbyItem ;

		int childCnt = lobby_item->childCount();
		int childIndex = 0;

		while (childIndex < childCnt) {
			QTreeWidgetItem *itemLoop = lobby_item->child(childIndex);
			if (itemLoop->type() == TYPE_LOBBY) 
			{
				// check for visible lobby
				//
				uint32_t i;
	
				for (i = 0; i < visibleLobbies.size(); ++i) 
					if (itemLoop->data(COLUMN_DATA, ROLE_ID).toULongLong() == visibleLobbies[i].lobby_id) 
						break;

				if (i >= visibleLobbies.size()) 
				{
					// Check for participating lobby with public level
					//
					for (lobbyIt = lobbies.begin(); lobbyIt != lobbies.end(); ++lobbyIt) 
						if(itemLoop->data(COLUMN_DATA, ROLE_ID).toULongLong() == lobbyIt->lobby_id) 
							break;

					if (lobbyIt == lobbies.end()) 
					{
						delete(lobby_item->takeChild(lobby_item->indexOfChild(itemLoop)));
						childCnt = lobby_item->childCount();
						continue;
					}
				}
			}
			++childIndex;
		}
	}

	// Now add visible lobbies
	//
	for (uint32_t i = 0; i < visibleLobbies.size(); ++i) 
	{
		const VisibleChatLobbyRecord &lobby = visibleLobbies[i];

#ifdef CHAT_LOBBY_GUI_DEBUG
		std::cerr << "adding " << lobby.lobby_name << "topic " << lobby.lobby_topic << " #" << std::hex << lobby.lobby_id << std::dec << " public " << lobby.total_number_of_peers << " peers. Lobby type: " << lobby.lobby_privacy_level << std::endl;
#endif


		bool subscribed = false;
		if (rsMsgs->getVirtualPeerId(lobby.lobby_id, vpid)) {
			subscribed = true;
		}

		QTreeWidgetItem *item = NULL;
		QTreeWidgetItem *lobby_item =NULL;
		QTreeWidgetItem *lobby_other_item =NULL;
		if (lobby.lobby_privacy_level == RS_CHAT_LOBBY_PRIVACY_LEVEL_PUBLIC)
		{
			if (subscribed) 
			{ 
				lobby_item = publicSubLobbyItem; 
				lobby_other_item = publicLobbyItem;
			} 
			else 
			{ 
				lobby_item = publicLobbyItem; 
				lobby_other_item = publicSubLobbyItem;
			}
		} 
		else
		{
			if (subscribed) 
			{ 
				lobby_item = privateSubLobbyItem; 
				lobby_other_item = privateLobbyItem;
			} 
			else 
			{ 
				lobby_item = privateLobbyItem; 
				lobby_other_item = privateSubLobbyItem;
			}
		}
		//QTreeWidgetItem *lobby_item = (lobby.lobby_privacy_level == RS_CHAT_LOBBY_PRIVACY_LEVEL_PUBLIC)?publicLobbyItem:privateLobbyItem ;

		// Search existing item
		//
		int childCnt = lobby_other_item->childCount();
		for (int childIndex = 0; childIndex < childCnt; ++childIndex)
		{
			QTreeWidgetItem *itemLoop = lobby_other_item->child(childIndex);
			if (itemLoop->type() == TYPE_LOBBY && itemLoop->data(COLUMN_DATA, ROLE_ID).toULongLong() == lobby.lobby_id) {
				delete(lobby_other_item->takeChild(lobby_other_item->indexOfChild(itemLoop)));
				childCnt = lobby_other_item->childCount();
				break;
			}
		}
		childCnt = lobby_item->childCount();
		for (int childIndex = 0; childIndex < childCnt; ++childIndex)
		{
			QTreeWidgetItem *itemLoop = lobby_item->child(childIndex);
			if (itemLoop->type() == TYPE_LOBBY && itemLoop->data(COLUMN_DATA, ROLE_ID).toULongLong() == lobby.lobby_id) {
				item = itemLoop;
				break;
			}
		}

		QIcon icon;
		if (item == NULL) 
		{
			item = new RSTreeWidgetItem(compareRole, TYPE_LOBBY);
			icon = (lobby.lobby_privacy_level == RS_CHAT_LOBBY_PRIVACY_LEVEL_PUBLIC) ? QIcon(IMAGE_PUBLIC) : QIcon(IMAGE_PRIVATE);
			lobby_item->addChild(item);
		} 
		else
		{
			if (item->data(COLUMN_DATA, ROLE_SUBSCRIBED).toBool() != subscribed) {
				// Replace icon
				icon = (lobby.lobby_privacy_level == RS_CHAT_LOBBY_PRIVACY_LEVEL_PUBLIC) ? QIcon(IMAGE_PUBLIC) : QIcon(IMAGE_PRIVATE);
			}
		}
		if (!icon.isNull()) {
			item->setIcon(COLUMN_NAME, subscribed ? icon : icon.pixmap(ui.lobbyTreeWidget->iconSize(), QIcon::Disabled));
		}

		bool autoSubscribe = rsMsgs->getLobbyAutoSubscribe(lobby.lobby_id);

		if (autoSubscribe && subscribed)
		{
			if(_lobby_infos.find(lobby.lobby_id) == _lobby_infos.end())
			{
				if (item == ui.lobbyTreeWidget->currentItem())
				{
					ChatDialog::chatFriend(vpid) ;
				}else{
					ChatDialog::chatFriend(vpid,false) ;
				}
			}
		}

		updateItem(ui.lobbyTreeWidget, item, lobby.lobby_id, lobby.lobby_name,lobby.lobby_topic, lobby.total_number_of_peers, subscribed, autoSubscribe);
	}

	//	time_t now = time(NULL) ;

	// Now add participating lobbies.
	//
	for (lobbyIt = lobbies.begin(); lobbyIt != lobbies.end(); ++lobbyIt) 
	{
		const ChatLobbyInfo &lobby = *lobbyIt;

#ifdef CHAT_LOBBY_GUI_DEBUG
		std::cerr << "adding " << lobby.lobby_name << "topic " << lobby.lobby_topic << " #" << std::hex << lobby.lobby_id << std::dec << " private " << lobby.nick_names.size() << " peers." << std::endl;
#endif

		QTreeWidgetItem *itemParent;
		if (lobby.lobby_privacy_level == RS_CHAT_LOBBY_PRIVACY_LEVEL_PUBLIC) {
            itemParent = publicSubLobbyItem;
		} else {
            itemParent = privateSubLobbyItem;
		}

		QTreeWidgetItem *item = NULL;

		// search existing item
		int childCount = itemParent->childCount();
		for (int childIndex = 0; childIndex < childCount; ++childIndex) {
			QTreeWidgetItem *itemLoop = itemParent->child(childIndex);
			if (itemLoop->type() == TYPE_LOBBY && itemLoop->data(COLUMN_DATA, ROLE_ID).toULongLong() == lobby.lobby_id) {
				item = itemLoop;
				break;
			}
		}

		QIcon icon;
		if (item == NULL) {
			item = new RSTreeWidgetItem(compareRole, TYPE_LOBBY);
            icon = (lobby.lobby_privacy_level == RS_CHAT_LOBBY_PRIVACY_LEVEL_PUBLIC) ? QIcon(IMAGE_PUBLIC) : QIcon(IMAGE_PRIVATE);
			itemParent->addChild(item);
		} else {
			if (!item->data(COLUMN_DATA, ROLE_SUBSCRIBED).toBool()) {
				// Replace icon
                icon = (lobby.lobby_privacy_level == RS_CHAT_LOBBY_PRIVACY_LEVEL_PUBLIC) ? QIcon(IMAGE_PUBLIC) : QIcon(IMAGE_PRIVATE);
			}
		}
		if (!icon.isNull()) {
			item->setIcon(COLUMN_NAME, icon);
		}

		bool autoSubscribe = rsMsgs->getLobbyAutoSubscribe(lobby.lobby_id);

		updateItem(ui.lobbyTreeWidget, item, lobby.lobby_id, lobby.lobby_name,lobby.lobby_topic, lobby.nick_names.size(), true, autoSubscribe);
	}
	publicSubLobbyItem->setHidden(publicSubLobbyItem->childCount()==0);
	privateSubLobbyItem->setHidden(privateSubLobbyItem->childCount()==0);
}

void ChatLobbyWidget::createChatLobby()
{
	int privacyLevel = 0;
	QAction *action = qobject_cast<QAction*>(sender());
	if (action) {
		privacyLevel = action->data().toInt();
	}

    std::list<RsPeerId> friends;
	CreateLobbyDialog(friends, privacyLevel).exec();
}

void ChatLobbyWidget::showLobby(QTreeWidgetItem *item)
{
	if (item == NULL || item->type() != TYPE_LOBBY) {
		showBlankPage(0) ;
		return;
	}

	ChatLobbyId id = item->data(COLUMN_DATA, ROLE_ID).toULongLong();

	if(_lobby_infos.find(id) == _lobby_infos.end())
		showBlankPage(id) ;
	else
		ui.stackedWidget->setCurrentWidget(_lobby_infos[id].dialog) ;
}

static void subscribeLobby(QTreeWidgetItem *item)
{
	if (item == NULL && item->type() != TYPE_LOBBY) {
		return;
	}

	ChatLobbyId id = item->data(COLUMN_DATA, ROLE_ID).toULongLong();
	if (rsMsgs->joinVisibleChatLobby(id)) {
        RsPeerId vpeer_id;
		if (rsMsgs->getVirtualPeerId(id, vpeer_id)) {
            ChatDialog::chatFriend(vpeer_id,true) ;
		}
	}
}

void ChatLobbyWidget::autoSubscribeLobby(QTreeWidgetItem *item)
{
    if (item == NULL && item->type() != TYPE_LOBBY) {
        return;
    }

    ChatLobbyId id = item->data(COLUMN_DATA, ROLE_ID).toULongLong();
    bool isAutoSubscribe = rsMsgs->getLobbyAutoSubscribe(id);
    rsMsgs->setLobbyAutoSubscribe(id, !isAutoSubscribe);
    if (!isAutoSubscribe) subscribeLobby(item);
}

void ChatLobbyWidget::showBlankPage(ChatLobbyId id)
{
	// show the default blank page.
	ui.stackedWidget->setCurrentWidget(ui._lobby_blank_page) ;

	// Update information
	std::vector<VisibleChatLobbyRecord> lobbies;
	rsMsgs->getListOfNearbyChatLobbies(lobbies);

	for(std::vector<VisibleChatLobbyRecord>::const_iterator it(lobbies.begin());it!=lobbies.end();++it)
		if( (*it).lobby_id == id)
		{
			ui.lobbyname_lineEdit->setText( RsHtml::plainText(it->lobby_name) );
			ui.lobbyid_lineEdit->setText( QString::number((*it).lobby_id,16) );
			ui.lobbytopic_lineEdit->setText( RsHtml::plainText(it->lobby_topic) );
			ui.lobbytype_lineEdit->setText( (( (*it).lobby_privacy_level == RS_CHAT_LOBBY_PRIVACY_LEVEL_PRIVATE)?tr("Private"):tr("Public")) );
			ui.lobbypeers_lineEdit->setText( QString::number((*it).total_number_of_peers) );

			ui.lobbyInfoLabel->setText(tr("You're not subscribed to this lobby; Double click-it to enter and chat.") );
			return ;
		}

	ui.lobbyname_lineEdit->clear();
	ui.lobbyid_lineEdit->clear();
	ui.lobbytopic_lineEdit->clear();
	ui.lobbytype_lineEdit->clear();
	ui.lobbypeers_lineEdit->clear();

	QString text = tr("No lobby selected. \nSelect lobbies at left to show details.\nDouble click lobbies to enter and chat.") ;
	ui.lobbyInfoLabel->setText(text) ;
}

void ChatLobbyWidget::subscribeItem()
{
	subscribeLobby(ui.lobbyTreeWidget->currentItem());
}

void ChatLobbyWidget::autoSubscribeItem()
{
	autoSubscribeLobby(ui.lobbyTreeWidget->currentItem());
}

QTreeWidgetItem *ChatLobbyWidget::getTreeWidgetItem(ChatLobbyId id)
{
    for(int p=0;p<4;++p)
	{
        QTreeWidgetItem *lobby_item =NULL;
        switch (p) {
        case 0: lobby_item = privateSubLobbyItem; break;
        case 1: lobby_item = publicSubLobbyItem; break;
        case 2: lobby_item = privateLobbyItem; break;
        case 4: lobby_item = publicLobbyItem; break;
        default: lobby_item = publicLobbyItem;
        }
        //QTreeWidgetItem *lobby_item = (p==0)?publicLobbyItem:privateLobbyItem ;

		int childCnt = lobby_item->childCount();
		int childIndex = 0;

		while (childIndex < childCnt) {
			QTreeWidgetItem *itemLoop = lobby_item->child(childIndex);

			if (itemLoop->type() == TYPE_LOBBY && itemLoop->data(COLUMN_DATA, ROLE_ID).toULongLong() == id) 
				return itemLoop ;

			++childIndex ;
		}
	}
	return NULL ;
}
void ChatLobbyWidget::updateTypingStatus(ChatLobbyId id)
{
	QTreeWidgetItem *item = getTreeWidgetItem(id) ;
	
	if(item != NULL)
	{
		item->setIcon(COLUMN_NAME,QIcon(IMAGE_TYPING)) ;
		_lobby_infos[id].last_typing_event = time(NULL) ;

		QTimer::singleShot(5000,this,SLOT(resetLobbyTreeIcons())) ;
	}
}
void ChatLobbyWidget::updatePeerLeaving(ChatLobbyId id)
{
	QTreeWidgetItem *item = getTreeWidgetItem(id) ;
	
	if(item != NULL)
	{
		item->setIcon(COLUMN_NAME,QIcon(IMAGE_PEER_LEAVING)) ;
		_lobby_infos[id].last_typing_event = time(NULL) ;

		QTimer::singleShot(5000,this,SLOT(resetLobbyTreeIcons())) ;
	}
}
void ChatLobbyWidget::updatePeerEntering(ChatLobbyId id)
{
	QTreeWidgetItem *item = getTreeWidgetItem(id) ;
	
	if(item != NULL)
	{
		item->setIcon(COLUMN_NAME,QIcon(IMAGE_PEER_ENTERING)) ;
		_lobby_infos[id].last_typing_event = time(NULL) ;

		QTimer::singleShot(5000,this,SLOT(resetLobbyTreeIcons())) ;
	}
}
void ChatLobbyWidget::resetLobbyTreeIcons()
{
	time_t now = time(NULL) ;

	for(std::map<ChatLobbyId,ChatLobbyInfoStruct>::iterator it(_lobby_infos.begin());it!=_lobby_infos.end();++it)
		if(it->second.last_typing_event + 5 <= now)
		{
			getTreeWidgetItem(it->first)->setIcon(COLUMN_NAME,it->second.default_icon) ;
			//std::cerr << "Reseted 1 lobby icon." << std::endl;
		}
}

void ChatLobbyWidget::unsubscribeItem()
{
	QTreeWidgetItem *item = ui.lobbyTreeWidget->currentItem();
	if (item == NULL && item->type() != TYPE_LOBBY) {
		return;
	}

	const ChatLobbyId id = item->data(COLUMN_DATA, ROLE_ID).toULongLong();

	unsubscribeChatLobby(id) ;
}

void ChatLobbyWidget::unsubscribeChatLobby(ChatLobbyId id)
{
	std::cerr << "Unsubscribing from chat lobby" << std::endl;

	// close the tab.

	std::map<ChatLobbyId,ChatLobbyInfoStruct>::iterator it = _lobby_infos.find(id) ;

	if(it != _lobby_infos.end())
	{
		ui.stackedWidget->removeWidget(it->second.dialog) ;
		_lobby_infos.erase(it) ;
		emit unreadCountChanged(unreadCount());
	}

	// Unsubscribe the chat lobby
    RsPeerId vpeer_id;
	if (rsMsgs->getVirtualPeerId(id, vpeer_id)) 
		ChatDialog::closeChat(vpeer_id);

	rsMsgs->unsubscribeChatLobby(id);
    bool isAutoSubscribe = rsMsgs->getLobbyAutoSubscribe(id);
	if (isAutoSubscribe) rsMsgs->setLobbyAutoSubscribe(id, !isAutoSubscribe);

	ChatLobbyDialog *cldCW=NULL ;
	if (NULL != (cldCW = dynamic_cast<ChatLobbyDialog *>(ui.stackedWidget->currentWidget())))
	{

		QTreeWidgetItem *qtwiFound = getTreeWidgetItem(cldCW->id());
		if (qtwiFound) {
			ui.lobbyTreeWidget->setCurrentItem(qtwiFound);
		}
	} else {
		ui.lobbyTreeWidget->clearSelection();

	}
}

void ChatLobbyWidget::updateCurrentLobby()
{
	QList<QTreeWidgetItem *> items = ui.lobbyTreeWidget->selectedItems() ;

	if(items.empty())
		showLobby(0) ;
	else
	{
		QTreeWidgetItem *item = items.front();
		showLobby(item);

		ChatLobbyId id = item->data(COLUMN_DATA, ROLE_ID).toULongLong();

		if(_lobby_infos.find(id) != _lobby_infos.end()) {
            int iPrivacyLevel= item->parent()->data(COLUMN_DATA, ROLE_PRIVACYLEVEL).toInt();
            QIcon icon = (iPrivacyLevel==RS_CHAT_LOBBY_PRIVACY_LEVEL_PUBLIC) ? QIcon(IMAGE_PUBLIC) : QIcon(IMAGE_PRIVATE);
			_lobby_infos[id].default_icon = icon ;
			_lobby_infos[id].unread_count = 0;
			item->setIcon(COLUMN_NAME, icon) ;

			emit unreadCountChanged(unreadCount());
		}
	}

	if (ui.filterLineEdit->text().isEmpty() == false) {
		filterItems(ui.filterLineEdit->text());
	}
}
void ChatLobbyWidget::updateMessageChanged(ChatLobbyId id)
{
	QTreeWidgetItem *current_item = ui.lobbyTreeWidget->currentItem();

	// Don't show anything for current lobby.
	//
	if(current_item != NULL && current_item->data(COLUMN_DATA, ROLE_ID).toULongLong() == id)
		return ;

	_lobby_infos[id].default_icon = QIcon(IMAGE_MESSAGE) ;
	++_lobby_infos[id].unread_count;

	emit unreadCountChanged(unreadCount());

	QTreeWidgetItem *item = getTreeWidgetItem(id) ;

	if(item == NULL)
		return ;

	item->setIcon(COLUMN_NAME,_lobby_infos[id].default_icon) ;
}

void ChatLobbyWidget::itemDoubleClicked(QTreeWidgetItem *item, int /*column*/)
{
	subscribeLobby(item);
}

void ChatLobbyWidget::displayChatLobbyEvent(qulonglong lobby_id, int event_type, const QString& nickname, const QString& str)
{
    RsPeerId vpid;
	if (rsMsgs->getVirtualPeerId(lobby_id, vpid)) {
		if (ChatLobbyDialog *cld = dynamic_cast<ChatLobbyDialog*>(ChatDialog::getExistingChat(vpid))) {
			cld->displayLobbyEvent(event_type, nickname, str);
		}
	}
}

void ChatLobbyWidget::readChatLobbyInvites()
{
	std::list<ChatLobbyInvite> invites;
	rsMsgs->getPendingChatLobbyInvites(invites);

	for(std::list<ChatLobbyInvite>::const_iterator it(invites.begin());it!=invites.end();++it) {
		if (QMessageBox::Ok == QMessageBox::question(this, tr("Invitation to chat lobby"), tr("%1 invites you to chat lobby named %2").arg(QString::fromUtf8(rsPeers->getPeerName((*it).peer_id).c_str())).arg(RsHtml::plainText(it->lobby_name)), QMessageBox::Ok, QMessageBox::Ignore)) {
			std::cerr << "Accepting invite to lobby " << (*it).lobby_name << std::endl;

			rsMsgs->acceptLobbyInvite((*it).lobby_id);

            RsPeerId vpid;
			if(rsMsgs->getVirtualPeerId((*it).lobby_id,vpid )) {
                ChatDialog::chatFriend(vpid,true);
			} else {
				std::cerr << "No lobby known with id 0x" << std::hex << (*it).lobby_id << std::dec << std::endl;
			}
		} else {
			rsMsgs->denyLobbyInvite((*it).lobby_id);
		}
	}
}

void ChatLobbyWidget::filterColumnChanged(int)
{
	filterItems(ui.filterLineEdit->text());
}

void ChatLobbyWidget::filterItems(const QString &text)
{
	int filterColumn = ui.filterLineEdit->currentFilter();

	int count = ui.lobbyTreeWidget->topLevelItemCount ();
	for (int index = 0; index < count; ++index) {
		filterItem(ui.lobbyTreeWidget->topLevelItem(index), text, filterColumn);
	}
}

bool ChatLobbyWidget::filterItem(QTreeWidgetItem *item, const QString &text, int filterColumn)
{
    bool visible = true;

    if (text.isEmpty() == false) {
        if (item->text(filterColumn).contains(text, Qt::CaseInsensitive) == false) {
            visible = false;
        }
    }

    int visibleChildCount = 0;
    int count = item->childCount();
    for (int index = 0; index < count; ++index) {
        if (filterItem(item->child(index), text, filterColumn)) {
            ++visibleChildCount;
        }
    }

    if (visible || visibleChildCount) {
        item->setHidden(false);
    } else {
        item->setHidden(true);
    }

    return (visible || visibleChildCount);
}


void ChatLobbyWidget::processSettings(bool bLoad)
{
	m_bProcessSettings = true;

	QHeaderView *Header = ui.lobbyTreeWidget->header () ;

	Settings->beginGroup(QString("ChatLobbyWidget"));

    if (bLoad) {
        // load settings

		// state of the lists
		Header->restoreState(Settings->value("lobbyList").toByteArray());

		setShowUserCountColumn(Settings->value("showUserCountColumn", !ui.lobbyTreeWidget->isColumnHidden(COLUMN_USER_COUNT)).toBool());
		setShowTopicColumn(Settings->value("showTopicColumn", !ui.lobbyTreeWidget->isColumnHidden(COLUMN_TOPIC)).toBool());
		setShowSubscribeColumn(Settings->value("showSubscribeColumn", !ui.lobbyTreeWidget->isColumnHidden(COLUMN_SUBSCRIBED)).toBool());
	} else {
		// save settings

		// state of the lists
		Settings->setValue("lobbyList", Header->saveState());

		Settings->setValue("showUserCountColumn", !ui.lobbyTreeWidget->isColumnHidden(COLUMN_USER_COUNT));
		Settings->setValue("showTopicColumn", !ui.lobbyTreeWidget->isColumnHidden(COLUMN_TOPIC));
		Settings->setValue("showSubscribeColumn", !ui.lobbyTreeWidget->isColumnHidden(COLUMN_SUBSCRIBED));
	}

	Settings->endGroup();
    m_bProcessSettings = false;
}

void ChatLobbyWidget::setShowUserCountColumn(bool show)
{
	if (ui.lobbyTreeWidget->isColumnHidden(COLUMN_USER_COUNT) == show) {
		ui.lobbyTreeWidget->setColumnHidden(COLUMN_USER_COUNT, !show);
	}
	ui.lobbyTreeWidget->header()->setVisible(getNumColVisible()>1);
}

void ChatLobbyWidget::setShowTopicColumn(bool show)
{
	if (ui.lobbyTreeWidget->isColumnHidden(COLUMN_TOPIC) == show) {
		ui.lobbyTreeWidget->setColumnHidden(COLUMN_TOPIC, !show);
	}
	ui.lobbyTreeWidget->header()->setVisible(getNumColVisible()>1);
}

void ChatLobbyWidget::setShowSubscribeColumn(bool show)
{
	if (ui.lobbyTreeWidget->isColumnHidden(COLUMN_SUBSCRIBED) == show) {
		ui.lobbyTreeWidget->setColumnHidden(COLUMN_SUBSCRIBED, !show);
	}
	ui.lobbyTreeWidget->header()->setVisible(getNumColVisible()>1);
}

int ChatLobbyWidget::getNumColVisible()
{
	int iNumColVis=0;
	for (int iColumn = 0; iColumn < COLUMN_COUNT; ++iColumn) {
		if (!ui.lobbyTreeWidget->isColumnHidden(iColumn)) {
			++iNumColVis;
		}
	}
    return iNumColVis;
}

uint ChatLobbyWidget::unreadCount()
{
	uint count = 0;

	for (std::map<ChatLobbyId,ChatLobbyInfoStruct>::iterator it = _lobby_infos.begin(); it != _lobby_infos.end(); ++it) {
		count += it->second.unread_count;
	}

	return count;
}
