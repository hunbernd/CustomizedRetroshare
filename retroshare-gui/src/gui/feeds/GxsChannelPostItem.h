/*
 * Retroshare Gxs Feed Item
 *
 * Copyright 2012-2013 by Robert Fernie.
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Library General Public
 * License Version 2.1 as published by the Free Software Foundation.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Library General Public License for more details.
 *
 * You should have received a copy of the GNU Library General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307
 * USA.
 *
 * Please report all bugs and problems to "retroshare@lunamutt.com".
 *
 */

#ifndef _GXS_CHANNEL_POST_ITEM_H
#define _GXS_CHANNEL_POST_ITEM_H

#include <retroshare/rsgxschannels.h>
#include "gui/gxs/GxsFeedItem.h"
#include <stdint.h>

namespace Ui {
class GxsChannelPostItem;
}

class FeedHolder;
class SubFileItem;

class GxsChannelPostItem : public GxsFeedItem
{
	Q_OBJECT

public:
	/** Default Constructor */
	GxsChannelPostItem(FeedHolder *parent, uint32_t feedId, const RsGxsGroupId &groupId, const RsGxsMessageId &messageId, bool isHome, bool autoUpdate);
	GxsChannelPostItem(FeedHolder *parent, uint32_t feedId, const RsGxsChannelPost &post, uint32_t subscribeFlags, bool isHome, bool autoUpdate);
	virtual ~GxsChannelPostItem();

	void setFileCleanUpWarning(uint32_t time_left);
	bool setPost(const RsGxsChannelPost &post);

protected: 
	virtual void loadMessage(const uint32_t &token);

private slots:
	/* default stuff */
	void toggle();
	void readAndClearItem();
	void download();
	void play();
	void copyLink();
	void loadComments();

	void readToggled(bool checked);
	void channelMsgReadSatusChanged(const QString& channelId, const QString& msgId, int status);

	void unsubscribeChannel();
	void updateItem();

	void makeUpVote();
	void makeDownVote();

signals:
	void vote(const RsGxsGrpMsgIdPair& msgId, bool up);	

private:
	void setup();
	void loadPost(const RsGxsChannelPost &post);

	void setReadStatus(bool isNew, bool isUnread);

	bool mInUpdateItemStatic;

	uint32_t mMode;
	uint32_t mSubscribeFlags;
	RsGxsChannelPost mPost;

	std::list<SubFileItem*> mFileItems;

	/** Qt Designer generated object */
	Ui::GxsChannelPostItem *ui;
};

#endif
