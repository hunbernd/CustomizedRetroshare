/****************************************************************
 *  RetroShare is distributed under the following license:
 *
 *  Copyright (C) 2008 Robert Fernie
 *
 *  This program is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU General Public License
 *  as published by the Free Software Foundation; either version 2
 *  of the License, or (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program; if not, write to the Free Software
 *  Foundation, Inc., 51 Franklin Street, Fifth Floor, 
 *  Boston, MA  02110-1301, USA.
 ****************************************************************/

#include <QFile>
#include <QFileInfo>
#include "common/vmessagebox.h"

#include "PhotoDialog.h"
#include "PhotoShow.h"
#include "rsiface/rspeers.h"
#include "rsiface/rsphoto.h"

#include <iostream>
#include <sstream>

#include <QContextMenuEvent>
#include <QMenu>
#include <QCursor>
#include <QPoint>
#include <QMouseEvent>
#include <QPixmap>
#include <QMessageBox>
#include <QHeaderView>
#include <QTimer>


/* Images for context menu icons */
#define IMAGE_REMOVEFRIEND       ":/images/removefriend16.png"
#define IMAGE_EXPIORTFRIEND      ":/images/exportpeers_16x16.png"
#define IMAGE_CHAT               ":/images/chat.png"
/* Images for Status icons */
#define IMAGE_ONLINE             ":/images/donline.png"
#define IMAGE_OFFLINE            ":/images/dhidden.png"

#define PHOTO_ICON_SIZE		90


#define PHOTO_PEER_COL_NAME 	0
#define PHOTO_PEER_COL_SHOW 	1
#define PHOTO_PEER_COL_PHOTO 	2
#define PHOTO_PEER_COL_PID 	3
#define PHOTO_PEER_COL_SID 	4
#define PHOTO_PEER_COL_PHOTOID 	5

#define PHOTO_LIST_COL_PHOTO	0
#define PHOTO_LIST_COL_NAME	1
#define PHOTO_LIST_COL_COMMENT  2
#define PHOTO_LIST_COL_DATE	3
#define PHOTO_LIST_COL_LOCATION 4
#define PHOTO_LIST_COL_SIZE	5
#define PHOTO_LIST_COL_PEERID   6
#define PHOTO_LIST_COL_PHOTOID  7

/******
 * #define PHOTO_DEBUG 1
 *****/



/** Constructor */
PhotoDialog::PhotoDialog(QWidget *parent)
: MainPage(parent)
{
  /* Invoke the Qt Designer generated object setup routine */
  ui.setupUi(this);

  connect( ui.peerTreeWidget, SIGNAL( customContextMenuRequested( QPoint ) ), this, SLOT( peerTreeWidgetCustomPopupMenu( QPoint ) ) );
  connect( ui.photoTreeWidget, SIGNAL( customContextMenuRequested( QPoint ) ), this, SLOT( photoTreeWidgetCustomPopupMenu( QPoint ) ) );

  connect( ui.peerTreeWidget, SIGNAL( currentItemChanged ( QTreeWidgetItem * , QTreeWidgetItem * ) ), this, SLOT( updatePhotoList( ) ) );

  connect( ui.photoTreeWidget, SIGNAL( itemDoubleClicked ( QTreeWidgetItem * , int ) ), this, SLOT( showPhoto( QTreeWidgetItem *, int ) ) );
  connect( ui.addButton, SIGNAL( clicked( ) ), this, SLOT( addPhotos( ) ) );

  /* hide the Tree +/- */
  ui.photoTreeWidget -> setRootIsDecorated( false );

  QSize iconSize(PHOTO_ICON_SIZE,PHOTO_ICON_SIZE);
  ui.photoTreeWidget->setIconSize(iconSize);
  
    /* Set header resize modes and initial section sizes */
//	QHeaderView * _header = ui.peertreeWidget->header () ;
//   	_header->setResizeMode (0, QHeaderView::Custom);
//	_header->setResizeMode (1, QHeaderView::Interactive);
//	_header->setResizeMode (2, QHeaderView::Interactive);
//	_header->setResizeMode (3, QHeaderView::Interactive);
//	_header->setResizeMode (4, QHeaderView::Interactive);
//	_header->setResizeMode (5, QHeaderView::Interactive);
//	_header->setResizeMode (6, QHeaderView::Interactive);
//	_header->setResizeMode (7, QHeaderView::Interactive);
//	_header->setResizeMode (8, QHeaderView::Interactive);
//	_header->setResizeMode (9, QHeaderView::Interactive);
//	_header->setResizeMode (10, QHeaderView::Interactive);
//	_header->setResizeMode (11, QHeaderView::Interactive);
//    
//	_header->resizeSection ( 0, 25 );
//	_header->resizeSection ( 1, 100 );
//	_header->resizeSection ( 2, 100 );
//	_header->resizeSection ( 3, 100 );
//	_header->resizeSection ( 4, 100 );
//	_header->resizeSection ( 5, 200 );
//	_header->resizeSection ( 6, 100 );
//	_header->resizeSection ( 7, 100 );
//	_header->resizeSection ( 8, 100 );
//	_header->resizeSection ( 9, 100 );
//	_header->resizeSection ( 10, 100 );


	/* Set a GUI update timer - much cleaner than
	 * doing everything through the notify agent 
	 */

  QTimer *timer = new QTimer(this);
  timer->connect(timer, SIGNAL(timeout()), this, SLOT(checkUpdate()));
  timer->start(1000);

  /* Hide platform specific features */
#ifdef Q_WS_WIN

#endif

}

void PhotoDialog::checkUpdate()
{
	/* update */
	if (!rsPhoto)
		return;

	if (rsPhoto->updated())
	{
		insertShowLists();
	}

	return;
}


void PhotoDialog::peerTreeWidgetCustomPopupMenu( QPoint point )
{

      QMenu contextMnu( this );
      QMouseEvent *mevent = new QMouseEvent( QEvent::MouseButtonPress, point, Qt::RightButton, Qt::RightButton, Qt::NoModifier );

      QAction *ins = new QAction(QIcon(IMAGE_EXPIORTFRIEND), tr( "Insert Show Lists" ), this );
      connect( ins , SIGNAL( triggered() ), this, SLOT( insertShowLists() ) );

      contextMnu.clear();
      contextMnu.addAction(ins);
      contextMnu.exec( mevent->globalPos() );
}

void PhotoDialog::photoTreeWidgetCustomPopupMenu( QPoint point )
{

      QMenu contextMnu( this );
      QMouseEvent *mevent = new QMouseEvent( QEvent::MouseButtonPress, point, Qt::RightButton, Qt::RightButton, Qt::NoModifier );

      QAction *rm = new QAction(QIcon(IMAGE_EXPIORTFRIEND), tr( "Remove" ), this );
      connect( rm , SIGNAL( triggered() ), this, SLOT( removePhoto() ) );

      contextMnu.clear();
      contextMnu.addAction(rm);
      contextMnu.addSeparator(); 
      contextMnu.exec( mevent->globalPos() );
}

void PhotoDialog::insertShowLists()
{
	/* clear it all */
	ui.peerTreeWidget->clear();

	/* iterate through peers */
	addShows(rsPeers->getOwnId());

	std::list<std::string> ids;
	std::list<std::string>::iterator it;
	rsPeers->getFriendList(ids);

	for(it = ids.begin(); it != ids.end(); it++)
	{
		addShows(*it);
	}
}

void PhotoDialog::addShows(std::string id)
{
	std::list<std::string> allPhotos;
	std::list<std::string> showIds;
	std::list<std::string>::iterator it;

        QTreeWidgetItem *peerItem = new QTreeWidgetItem((QTreeWidget*)0);
	peerItem->setText(PHOTO_PEER_COL_NAME, QString::fromStdString(rsPeers->getPeerName(id)));
	peerItem->setText(PHOTO_PEER_COL_PID, QString::fromStdString(id));
	peerItem->setText(PHOTO_PEER_COL_SID, "");
	peerItem->setText(PHOTO_PEER_COL_PHOTOID, "");

	ui.peerTreeWidget->insertTopLevelItem(0, peerItem);

        QTreeWidgetItem *allItem = new QTreeWidgetItem((QTreeWidget*)0);
	allItem->setText(PHOTO_PEER_COL_SHOW, "All Photos");
	allItem->setText(PHOTO_PEER_COL_PID, QString::fromStdString(id));
	allItem->setText(PHOTO_PEER_COL_SID, "");
	allItem->setText(PHOTO_PEER_COL_PHOTOID, "");

	peerItem->addChild(allItem);

	rsPhoto->getPhotoList(id, allPhotos);
	rsPhoto->getShowList(id, showIds);

	for(it = allPhotos.begin(); it != allPhotos.end(); it++)
	{
        	QTreeWidgetItem *photoItem = new QTreeWidgetItem((QTreeWidget*)0);
		photoItem->setText(PHOTO_PEER_COL_PHOTO, QString::fromStdString(*it));

		photoItem->setText(PHOTO_PEER_COL_PID, QString::fromStdString(id));
		photoItem->setText(PHOTO_PEER_COL_SID, "");
		photoItem->setText(PHOTO_PEER_COL_PHOTOID, QString::fromStdString(*it));

		allItem->addChild(photoItem);
	}


	for(it = showIds.begin(); it != showIds.end(); it++)
	{
		/* get details */
		RsPhotoShowDetails detail;
		rsPhoto->getShowDetails(id, *it, detail);

        	QTreeWidgetItem *showItem = new QTreeWidgetItem((QTreeWidget*)0);
		showItem->setText(PHOTO_PEER_COL_SHOW, QString::fromStdString(*it));

		showItem->setText(PHOTO_PEER_COL_PID, QString::fromStdString(id));
		showItem->setText(PHOTO_PEER_COL_SID, QString::fromStdString(*it));
		showItem->setText(PHOTO_PEER_COL_PHOTOID, "");

		peerItem->addChild(showItem);

		std::list<RsPhotoShowInfo>::iterator sit;
		for(sit = detail.photos.begin(); sit != detail.photos.end(); sit++)
		{
        		QTreeWidgetItem *photoItem = new QTreeWidgetItem((QTreeWidget*)0);
			photoItem->setText(PHOTO_PEER_COL_PHOTO, QString::fromStdString(sit->photoId));

			photoItem->setText(PHOTO_PEER_COL_PID, QString::fromStdString(id));
			photoItem->setText(PHOTO_PEER_COL_SID, QString::fromStdString(*it));
			photoItem->setText(PHOTO_PEER_COL_PHOTOID, QString::fromStdString(sit->photoId));

			showItem->addChild(photoItem);
		}
	}
}


void PhotoDialog::updatePhotoList()
{
#ifdef PHOTO_DEBUG 
	std::cerr << "PhotoDialog::updatePhotoList()" << std::endl;
#endif

	/* get current item */
	QTreeWidgetItem *item = ui.peerTreeWidget->currentItem();

	if (!item)
	{
		/* leave current list */
#ifdef PHOTO_DEBUG 
		std::cerr << "PhotoDialog::updatePhotoList() No Current item -> leave" << std::endl;
#endif
		return;
	}

	/* check if it has changed */
	std::string pid = item->text(PHOTO_PEER_COL_PID).toStdString();
	std::string sid = item->text(PHOTO_PEER_COL_SID).toStdString();

	if ((mCurrentPID == pid) && (mCurrentSID == sid))
	{
		/* still good */
#ifdef PHOTO_DEBUG 
		std::cerr << "PhotoDialog::updatePhotoList() List still good!" << std::endl;
#endif
		return;
	}

#ifdef PHOTO_DEBUG 
	std::cerr << "PhotoDialog::updatePhotoList() pid: " << pid << " sid: " << sid << std::endl;
#endif
	/* get the list of photos */

	ui.photoTreeWidget->clear();
        QList<QTreeWidgetItem *> items;

	if (sid != "")
	{
#ifdef PHOTO_DEBUG 
		std::cerr << "PhotoDialog::updatePhotoList() SID -> showing show" << std::endl;
#endif
		/* load up show list */
		RsPhotoShowDetails detail;
		rsPhoto->getShowDetails(pid, sid, detail);


		std::list<RsPhotoShowInfo>::iterator sit;
		for(sit = detail.photos.begin(); sit != detail.photos.end(); sit++)
		{
			RsPhotoDetails photoDetail;

			if (!rsPhoto->getPhotoDetails(pid, sit->photoId, photoDetail))
			{
#ifdef PHOTO_DEBUG 
	std::cerr << "PhotoDialog::updatePhotoList() getPhotoDetails: " << sit->photoId << " FAILED" << std::endl;
#endif
				continue;
			}

        		QTreeWidgetItem *photoItem = new QTreeWidgetItem((QTreeWidget*)0);
			if (photoDetail.isAvailable)
			{
				QPixmap qpp(QString::fromStdString(photoDetail.path));
				photoItem->setIcon(PHOTO_LIST_COL_PHOTO, 
					QIcon(qpp.scaledToHeight(PHOTO_ICON_SIZE)));

  				QSize iconSize(PHOTO_ICON_SIZE + 10,PHOTO_ICON_SIZE + 10);
  				photoItem->setSizeHint(PHOTO_LIST_COL_PHOTO, iconSize);
			}
			else
			{
				photoItem->setText(PHOTO_LIST_COL_PHOTO, "Photo Not Available");
			}

			photoItem->setText(PHOTO_LIST_COL_NAME, 
					QString::fromStdString(photoDetail.name));
			photoItem->setText(PHOTO_LIST_COL_COMMENT, 
					QString::fromStdWString(photoDetail.comment));
			photoItem->setText(PHOTO_LIST_COL_DATE, 
					QString::fromStdString(photoDetail.date));
			photoItem->setText(PHOTO_LIST_COL_LOCATION, 
					QString::fromStdString(photoDetail.location));
			photoItem->setText(PHOTO_LIST_COL_SIZE, 
					QString::number(photoDetail.size));
			photoItem->setText(PHOTO_LIST_COL_PEERID, 
					QString::fromStdString(photoDetail.id));
			photoItem->setText(PHOTO_LIST_COL_PHOTOID, 
					QString::fromStdString(photoDetail.hash));

			items.append(photoItem);
		}
	}
	else
	{
#ifdef PHOTO_DEBUG 
		std::cerr << "PhotoDialog::updatePhotoList() No SID -> show all" << std::endl;
#endif

		std::list<std::string> photoIds;
		std::list<std::string>::iterator pit;
		rsPhoto->getPhotoList(pid, photoIds);
		for(pit = photoIds.begin(); pit != photoIds.end(); pit++)
		{
			RsPhotoDetails photoDetail;

			if (!rsPhoto->getPhotoDetails(pid, *pit, photoDetail))
			{
#ifdef PHOTO_DEBUG 
	std::cerr << "PhotoDialog::updatePhotoList() getPhotoDetails: " << *pit << " FAILED" << std::endl;
#endif
				continue;
			}

        		QTreeWidgetItem *photoItem = new QTreeWidgetItem((QTreeWidget*)0);
			if (photoDetail.isAvailable)
			{
				QPixmap qpp(QString::fromStdString(photoDetail.path));
				photoItem->setIcon(PHOTO_LIST_COL_PHOTO, 
					QIcon(qpp.scaledToHeight(PHOTO_ICON_SIZE)));

  				QSize iconSize(PHOTO_ICON_SIZE + 10,PHOTO_ICON_SIZE + 10);
  				photoItem->setSizeHint(PHOTO_LIST_COL_PHOTO, iconSize);
			}
			else
			{
				photoItem->setText(PHOTO_LIST_COL_PHOTO, "Photo Not Available");
			}

			photoItem->setText(PHOTO_LIST_COL_NAME, 
					QString::fromStdString(photoDetail.name));
			photoItem->setText(PHOTO_LIST_COL_COMMENT, 
					QString::fromStdWString(photoDetail.comment));
			photoItem->setText(PHOTO_LIST_COL_DATE, 
					QString::fromStdString(photoDetail.date));
			photoItem->setText(PHOTO_LIST_COL_LOCATION, 
					QString::fromStdString(photoDetail.location));
			photoItem->setText(PHOTO_LIST_COL_SIZE, 
					QString::number(photoDetail.size));
			photoItem->setText(PHOTO_LIST_COL_PEERID, 
					QString::fromStdString(photoDetail.id));
			photoItem->setText(PHOTO_LIST_COL_PHOTOID, 
					QString::fromStdString(photoDetail.hash));


#ifdef PHOTO_DEBUG 
			std::cerr << "PhotoDialog::updatePhotoList() added Item: " << *pit << std::endl;
#endif
			items.append(photoItem);
		}

	}

	/* update ids? */
	mCurrentPID = pid;
	mCurrentSID = sid;

	/* add the items in! */
	ui.photoTreeWidget->insertTopLevelItems(0, items);
	ui.photoTreeWidget->update();
}



/* get the list of peers from the RsIface.  */
void  PhotoDialog::insertExample()
{

}

QTreeWidgetItem *PhotoDialog::getCurrentLine()
{
	/* get the current, and extract the Id */

	/* get a link to the table */
        QTreeWidget *peerWidget = ui.photoTreeWidget;
        QTreeWidgetItem *item = peerWidget -> currentItem();
        if (!item)
        {
#ifdef PHOTO_DEBUG 
		std::cerr << "Invalid Current Item" << std::endl;
#endif
		return NULL;
	}

	/* Display the columns of this item. */
#ifdef PHOTO_DEBUG 
	std::ostringstream out;
        out << "CurrentPeerItem: " << std::endl;

	for(int i = 1; i < 6; i++)
	{
		QString txt = item -> text(i);
		out << "\t" << i << ":" << txt.toStdString() << std::endl;
	}
	std::cerr << out.str();
#endif
	return item;
}

void PhotoDialog::removePhoto()
{
	QTreeWidgetItem *c = getCurrentLine();
#ifdef PHOTO_DEBUG 
	std::cerr << "PhotoDialog::removePhoto()" << std::endl;
#endif
}


void PhotoDialog::addPhotos()
{
	/* get file dialog */
 	QStringList files = QFileDialog::getOpenFileNames(this,
			         "Select one or more Photos to add",
			         "/home", "Images (*.png *.xpm *.jpg *.gif)");
	/* add photo to list */
 	QStringList::iterator it;
	for(it = files.begin(); it != files.end(); it++)
	{
		addPhoto(*it);
	}
}

void PhotoDialog::addPhoto(QString filename)
{
	/* store in rsPhoto */
	std::string photoId = rsPhoto->addPhoto(filename.toStdString());

}

void PhotoDialog::showPhoto( QTreeWidgetItem *item, int column)
{

	if (!item)
		return;

	/* get the photoId */
	std::string pid = item->text(PHOTO_LIST_COL_PEERID).toStdString();
	std::string photoid = item->text(PHOTO_LIST_COL_PHOTOID).toStdString();

	PhotoShow *newView = new PhotoShow();
	newView->show();

	newView->setPeerId(pid);
	newView->setPhotoId(photoid);

	return;
}





