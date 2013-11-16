/****************************************************************
 *  RetroShare is distributed under the following license:
 *
 *  Copyright (C) 2011 - 2011 RetroShare Team
 *
 *  Cyril Soler (csoler@users.sourceforge.net)
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

#include <QCheckBox>
#include <QMessageBox>
#include <QDir>
#include <QKeyEvent>
#include "RsCollectionDialog.h"
#include "RsCollectionFile.h"
#include "util/misc.h"

#define COLUMN_FILE   0
#define COLUMN_SIZE   1
#define COLUMN_HASH   2
#define COLUMN_COUNT  3


RsCollectionDialog::RsCollectionDialog(const QString& collectionFileName,const std::vector<ColFileInfo>& colFileInfos,const bool& creation=false)
	: _filename(collectionFileName),_colFileInfos(colFileInfos),_creationMode(creation)
{
	setupUi(this) ;

	uint32_t size = _colFileInfos.size();
	for(uint32_t i=0;i<size;++i)
	{
		const ColFileInfo &colFileInfo = _colFileInfos[i];
		_newColFileInfos.push_back(colFileInfo);
	}

	setWindowFlags(Qt::Window); // for maximize button
	setWindowFlags(windowFlags() & ~Qt::WindowMinimizeButtonHint);

	setWindowTitle(QString("%1 - %2").arg(windowTitle()).arg(QFileInfo(_filename).completeBaseName()));

	// 1 - add all elements to the list.

	_fileEntriesTW->setColumnCount(COLUMN_COUNT) ;

	QTreeWidgetItem *headerItem = _fileEntriesTW->headerItem();
	headerItem->setText(COLUMN_FILE, tr("File"));
	headerItem->setText(COLUMN_SIZE, tr("Size"));
	headerItem->setText(COLUMN_HASH, tr("Hash"));

	bool wrong_chars = !updateList();

	// 2 - connect necessary signals/slots

	connectUpdate(true);
	connect(_add_PB,SIGNAL(clicked()),this,SLOT(add())) ;
	connect(_selectAll_PB,SIGNAL(clicked()),this,SLOT(selectAll())) ;
	connect(_deselectAll_PB,SIGNAL(clicked()),this,SLOT(deselectAll())) ;
	connect(_cancel_PB,SIGNAL(clicked()),this,SLOT(cancel())) ;
	connect(_save_PB,SIGNAL(clicked()),this,SLOT(save())) ;
	connect(_download_PB,SIGNAL(clicked()),this,SLOT(download())) ;

	connect(_hashBox, SIGNAL(fileHashingFinished(QList<HashedFile>)), this, SLOT(fileHashingFinished(QList<HashedFile>)));

	_add_PB->setVisible(_creationMode);
	_save_PB->setVisible(_creationMode);
	_download_PB->setVisible(!_creationMode);

	_fileEntriesTW->installEventFilter(this);

	setAcceptDrops(true);
	_hashBox->setDropWidget(this);
	_hashBox->setAutoHide(true);
	_hashBox->setDefaultTransferRequestFlags(RS_FILE_REQ_ANONYMOUS_ROUTING) ;

	if(wrong_chars)
		QMessageBox::warning(NULL,tr("Bad filenames have been cleaned"),tr("Some filenames or directory names contained forbidden characters.\nCharacters <b>\",|,/,\\,&lt;,&gt;,*,?</b> will be replaced by '_'.\n Concerned files are listed in red.")) ;
}

bool RsCollectionDialog::updateList()
{
	bool wrong_chars = false ;

	uint32_t size = _newColFileInfos.size();

	uint64_t total_size ;
	uint32_t total_files ;

	for(uint32_t i=0;i<size;++i)
	{
		const ColFileInfo &colFileInfo = _newColFileInfos[i];

		QList<QTreeWidgetItem*> founds;
		if (colFileInfo.type == DIR_TYPE_DIR){
			founds = _fileEntriesTW->findItems(colFileInfo.path + "/" +colFileInfo.name, Qt::MatchExactly, COLUMN_FILE);
		} else {
			founds = _fileEntriesTW->findItems(colFileInfo.hash, Qt::MatchExactly | Qt::MatchRecursive, COLUMN_HASH);
		}
		if (founds.count() == 0) {
		QTreeWidgetItem *item = new QTreeWidgetItem;

		item->setFlags(Qt::ItemIsUserCheckable | item->flags());
			item->setCheckState(COLUMN_FILE, Qt::Checked);
			item->setData(COLUMN_FILE, Qt::UserRole, qVariantFromValue(colFileInfo));
			item->setText(COLUMN_FILE, colFileInfo.path + "/" + colFileInfo.name);
			item->setText(COLUMN_SIZE, misc::friendlyUnit(colFileInfo.size));
			item->setText(COLUMN_HASH, colFileInfo.hash);
			if (colFileInfo.type==DIR_TYPE_DIR) item->setToolTip(COLUMN_FILE,tr("This is a directory. Double-click to expend it."));

			if(colFileInfo.filename_has_wrong_characters)
		{
			wrong_chars = true ;
				item->setTextColor(COLUMN_FILE, QColor(255,80,120)) ;
		}

		_fileEntriesTW->addTopLevelItem(item);

			if(colFileInfo.type == DIR_TYPE_DIR) {
				wrong_chars |= addChild(item, colFileInfo.children, total_size, total_files);
			} else {
				total_size += colFileInfo.size ;
		total_files++ ;
			}
		}
	}

	_filename_TL->setText(_filename) ;
	for (int column = 0; column < _fileEntriesTW->columnCount(); ++column) {
		_fileEntriesTW->resizeColumnToContents(column);
	}

	updateSizes() ;

	return !wrong_chars;
}

bool RsCollectionDialog::addChild(QTreeWidgetItem *parent, std::vector<ColFileInfo> child, uint64_t total_size, uint32_t total_files)
{
	bool wrong_chars = false ;

	uint32_t size = child.size();
	for(uint32_t i=0;i<size;++i)
	{
		const ColFileInfo &colFileInfo = child[i];

		QList<QTreeWidgetItem*> founds;
		if (colFileInfo.type == DIR_TYPE_DIR){
			founds = _fileEntriesTW->findItems(colFileInfo.name, Qt::MatchExactly, COLUMN_FILE);
		} else {
			founds = _fileEntriesTW->findItems(colFileInfo.hash, Qt::MatchExactly | Qt::MatchRecursive, COLUMN_HASH);
		}
		if (founds.count() == 0) {
			QTreeWidgetItem *item = new QTreeWidgetItem;

			item->setFlags(Qt::ItemIsUserCheckable | item->flags());
			item->setCheckState(COLUMN_FILE, Qt::Checked);
			item->setData(COLUMN_FILE, Qt::UserRole, qVariantFromValue(colFileInfo));
			item->setText(COLUMN_FILE, colFileInfo.path + "/" + colFileInfo.name);
			item->setText(COLUMN_SIZE, misc::friendlyUnit(colFileInfo.size));
			item->setText(COLUMN_HASH, colFileInfo.hash);
			if (colFileInfo.type==DIR_TYPE_DIR) item->setToolTip(COLUMN_FILE,tr("This is a directory. Double-click to expend it."));

			if(colFileInfo.filename_has_wrong_characters)
			{
				wrong_chars = true ;
				item->setTextColor(COLUMN_FILE, QColor(255,80,120)) ;
			}

			parent->addChild(item);

			if (colFileInfo.type == DIR_TYPE_DIR){
				wrong_chars |= addChild(item, colFileInfo. children, total_size, total_files);
			} else {
				total_size += colFileInfo.size ;
				total_files++ ;
			}
		}
	}
	return wrong_chars;
}

bool RsCollectionDialog::eventFilter(QObject *obj, QEvent *event)
{
	if (obj == _fileEntriesTW) {
		if (event->type() == QEvent::KeyPress) {
			QKeyEvent *keyEvent = static_cast<QKeyEvent*>(event);
			if (keyEvent && keyEvent->key() == Qt::Key_Space) {
				// Space pressed

				// get state of current item
				QTreeWidgetItem *item = _fileEntriesTW->currentItem();
				if (item) {
					Qt::CheckState checkState = (item->checkState(COLUMN_FILE) == Qt::Checked) ? Qt::Unchecked : Qt::Checked;

					//connectUpdate(false);

					// set state of all selected items
					QList<QTreeWidgetItem*> selectedItems = _fileEntriesTW->selectedItems();
					QList<QTreeWidgetItem*>::iterator it;
					for (it = selectedItems.begin(); it != selectedItems.end(); ++it) {
						if ((*it)->checkState(COLUMN_FILE) != checkState)
							(*it)->setCheckState(COLUMN_FILE, checkState);
					}

					//updateSizes();

					//connectUpdate(true);
				}

				return true; // eat event
			}
		}
	}
	// pass the event on to the parent class
	return QDialog::eventFilter(obj, event);
}

void RsCollectionDialog::connectUpdate(bool doConnect)
{
	if (doConnect) {
		connect(_fileEntriesTW, SIGNAL(itemChanged(QTreeWidgetItem*,int)), this, SLOT(itemChanged(QTreeWidgetItem*,int)));
	} else {
		disconnect(_fileEntriesTW, SIGNAL(itemChanged(QTreeWidgetItem*,int)), this, SLOT(itemChanged(QTreeWidgetItem*,int)));
	}
}

void RsCollectionDialog::updateSizes()
{
	uint64_t total_size = 0 ;
	uint32_t total_files = 0 ;

	QTreeWidgetItemIterator itemIterator(_fileEntriesTW);
	QTreeWidgetItem *item;
	while ((item = *itemIterator) != NULL) {
		itemIterator++;

		if (item->checkState(COLUMN_FILE) == Qt::Checked) {
			ColFileInfo colFileInfo = qVariantValue<ColFileInfo>(item->data(COLUMN_FILE, Qt::UserRole));

			if (colFileInfo.type == DIR_TYPE_FILE){
				total_size += colFileInfo.size;
			++total_files ;
			}
		}
	}
	_selectedFiles_TL->setText(QString::number(total_files)) ;
	_totalSize_TL->setText(misc::friendlyUnit(total_size)) ;
}

void RsCollectionDialog::itemChanged(QTreeWidgetItem *item, int /*column*/)
{
	ColFileInfo colFileInfo = qVariantValue<ColFileInfo>(item->data(COLUMN_FILE, Qt::UserRole));
	colFileInfo.checked = (item->checkState(COLUMN_FILE)==Qt::Checked);

	if ((item->childCount()>0) && (item->checkState(COLUMN_FILE)!=Qt::PartiallyChecked)) changeCheckAllChild(item,colFileInfo.checked);
	QApplication::processEvents();
	if (item->parent()!=NULL) checkAllParent(item->parent());

	updateSizes();
}

void RsCollectionDialog::changeCheckAllChild(QTreeWidgetItem *item, bool checked)
{
	for (int i=0; i<item->childCount(); i++) {
		if ((item->child(i)->checkState(COLUMN_FILE)==Qt::Checked)!=checked) {
			item->child(i)->setCheckState(COLUMN_FILE, checked ? Qt::Checked : Qt::Unchecked);
			if (item->child(i)->childCount()>0) changeCheckAllChild(item->child(i),checked);
		}
	}
}

void RsCollectionDialog::checkAllParent(QTreeWidgetItem *item)
{
	bool allChildrenChecked=true;
	bool atLeastOneChecked=false;
	for (int i=0; i<item->childCount(); i++) {
		bool bChecked=(item->child(i)->checkState(COLUMN_FILE)==Qt::Checked);
		allChildrenChecked&=bChecked;
		atLeastOneChecked|=bChecked;
	}
	item->setCheckState(COLUMN_FILE,allChildrenChecked?Qt::Checked:atLeastOneChecked?Qt::PartiallyChecked:Qt::Unchecked);
	if (item->parent()!=NULL) checkAllParent(item->parent());
}

void RsCollectionDialog::selectDeselectAll(bool select)
{
	//connectUpdate(false);

	QTreeWidgetItemIterator itemIterator(_fileEntriesTW);
	QTreeWidgetItem *item;
	while ((item = *itemIterator) != NULL) {
		itemIterator++;
		if (item->checkState(COLUMN_FILE) != select ? Qt::Checked : Qt::Unchecked)
			item->setCheckState(COLUMN_FILE, select ? Qt::Checked : Qt::Unchecked);
	}

	//updateSizes();

	//connectUpdate(true);
}

void RsCollectionDialog::selectAll()
{
	std::cerr << "Selecting all !" << std::endl;

	selectDeselectAll(true);
}

void RsCollectionDialog::deselectAll()
{
	std::cerr << "Deselecting all !" << std::endl;

	selectDeselectAll(false);
}

void RsCollectionDialog::cancel() 
{
	std::cerr << "Canceling!" << std::endl;
	close() ;
}

void RsCollectionDialog::download() 
{
	std::cerr << "Downloading!" << std::endl;

	QString dldir = QString::fromUtf8(rsFiles->getDownloadDirectory().c_str()) ;

	std::cerr << "downloading all these files:" << std::endl;

	QTreeWidgetItemIterator itemIterator(_fileEntriesTW);
	QTreeWidgetItem *item;
	while ((item = *itemIterator) != NULL) {
		itemIterator++;

		ColFileInfo colFileInfo = qVariantValue<ColFileInfo>(item->data(COLUMN_FILE, Qt::UserRole));

		if (item->checkState(COLUMN_FILE) == Qt::Checked) {
			std::cerr << colFileInfo.name.toStdString() << " " << colFileInfo.hash.toStdString() << " " << colFileInfo.size << " " << colFileInfo.path.toStdString() << std::endl;
			QString cleanPath = dldir + colFileInfo.path ;
			std::cerr << "making directory " << cleanPath.toStdString() << std::endl;

			if(!QDir(QApplication::applicationDirPath()).mkpath(cleanPath))
				QMessageBox::warning(NULL,QObject::tr("Unable to make path"),QObject::tr("Unable to make path:")+"<br>  "+cleanPath) ;
			if (colFileInfo.type==DIR_TYPE_FILE)
				rsFiles->FileRequest(colFileInfo.name.toUtf8().constData(), colFileInfo.hash.toUtf8().constData(), colFileInfo.size, cleanPath.toUtf8().constData(), RS_FILE_REQ_ANONYMOUS_ROUTING, std::list<std::string>());
		}
		else
			std::cerr<<"Skipping file : " << colFileInfo.name.toStdString() << std::endl;
	}

	close();
}

void RsCollectionDialog::add()
{
	QStringList files;
	if (misc::getOpenFileNames(this, RshareSettings::LASTDIR_EXTRAFILE, tr("Add Extra File"), "", files)) {
		_hashBox->addAttachments(files,RS_FILE_REQ_ANONYMOUS_ROUTING /*, 0*/);
	}

}

void RsCollectionDialog::fileHashingFinished(QList<HashedFile> hashedFiles)
{
	std::cerr << "RsCollectionDialog::fileHashingFinished() started." << std::endl;

	QString message;

	QList<HashedFile>::iterator it;
	for (it = hashedFiles.begin(); it != hashedFiles.end(); ++it) {
		HashedFile& hashedFile = *it;

		ColFileInfo colFileInfo;
		colFileInfo.name=hashedFile.filename;
		colFileInfo.path=hashedFile.filepath.left(hashedFile.filepath.length()-hashedFile.filename.length()-1);
		colFileInfo.size=hashedFile.size;
		colFileInfo.hash=QString::fromStdString(hashedFile.hash);
		colFileInfo.filename_has_wrong_characters=false;
		colFileInfo.type=DIR_TYPE_FILE;

		_newColFileInfos.push_back(colFileInfo);

	}

	std::cerr << "RsCollectionDialog::fileHashingFinished message : " << message.toStdString() << std::endl;

	updateList();
}

void RsCollectionDialog::save()
{
	std::cerr << "Saving!" << std::endl;

	emit saveColl(_newColFileInfos);
	close();
}
