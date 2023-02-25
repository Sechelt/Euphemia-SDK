/*
 * Copyright 2022-2023, Peter Harvey <pharvey@codebydesign.com>
 *
 * This file is part of Euphemia-SDK.
 *
 * This program is free software; you can redistribute it and/or modify it
 * under the terms of the GNU Lesser General Public License as published by the Free
 * Software Foundation; either version 2 of the License, or (at your option)
 * any later version.
 *
 * This program is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE.  See the GNU Lesser General Public License for
 * more details.
 *
 * You should have received a copy of the GNU Lesser General Public License along with
 * this program. If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef H_PPasteRectangle
#define H_PPasteRectangle

#include "PDrawRectangle.h"

class PPasteRectangle : public PDrawRectangle
{
    Q_OBJECT
public:
    PPasteRectangle( PCanvas *pCanvas );
    PPasteRectangle( PCanvas *pCanvas, const QImage & );

    virtual void doPress( PMouseEvent *pEvent ) override;
    virtual void doCommit() override;

    virtual bool canCommit() override;

protected:
    QImage image;

    virtual void doPaint( QPainter *, bool bCommit = true ) override;
    virtual void doDrawState( const QPoint &point ) override;
};

class PPasteToolBar : public QWidget
{
    Q_OBJECT
public:
    PPasteToolBar( QWidget *p );

public slots:
    void slotRefresh( const PContextPaste & );

protected:
    QComboBox *pComposition;
    QCheckBox *pStamp;

protected slots:
    void slotComposition( int );
    void slotStamp( int );
};


#endif
