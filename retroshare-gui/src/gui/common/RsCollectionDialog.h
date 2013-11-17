/*************************************:***************************
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

#include "ui_RsCollectionDialog.h"
#include "RsCollectionFile.h"

class QCheckBox ;

class RsCollectionDialog: public QDialog, public Ui::RsCollectionDialog
{
	Q_OBJECT

public:
		RsCollectionDialog(const QString& filename, const std::vector<ColFileInfo> &colFileInfos,const bool& creation) ;

private slots:
	void download() ;
	void selectAll() ;
	void deselectAll() ;
	void cancel() ;
	void updateSizes() ;
	void itemChanged(QTreeWidgetItem *item, int column);
		void changeCheckAllChild(QTreeWidgetItem *item, bool checked);
		void checkAllParent(QTreeWidgetItem *item);
		void add();
		void fileHashingFinished(QList<HashedFile> hashedFiles);
		void save();

signals:
		void saveColl(std::vector<ColFileInfo>);

protected:
	bool eventFilter(QObject *obj, QEvent *ev);

private:
	void selectDeselectAll(bool select);
	void connectUpdate(bool doConnect);
		bool updateList();
		bool addChild(QTreeWidgetItem *parent, std::vector<ColFileInfo> child, uint64_t total_size, uint32_t total_files);

		const QString _filename ;
		const std::vector<ColFileInfo>& _colFileInfos ;
		const bool _creationMode ;
		QString _newFilename ;
		std::vector<ColFileInfo> _newColFileInfos ;
};
