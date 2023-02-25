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

#ifndef H_PDrawFreeHand
#define H_PDrawFreeHand

#include <WImageButton.h>

#include "PFreeBase.h"

class PDrawFreeHand : public PFreeBase
{
public:
    PDrawFreeHand( PCanvas *pCanvas );

    virtual void doPress( PMouseEvent *pEvent ) override;
    virtual void doMove( PMouseEvent *pEvent ) override;
    virtual void doRelease( PMouseEvent *pEvent ) override;

protected:
    QPoint              pointLast;
    QPen                pen;
    QBrush              brush;
    PContextFreeHand    t;

    void doDrawState( const QPoint & );
    void doDrawPen( const QPoint & );
    void doDrawEllipse( const QPoint & );
    void doDrawRectangle( const QPoint & );
    void doDrawCross( const QPoint & );
    void doDrawImage( const QPoint & );
};

class PFreeHandToolBar : public QWidget
{
    Q_OBJECT
public:
    PFreeHandToolBar( QWidget *p );

public slots:
    void slotRefresh( const PContextFreeHand & );

protected slots:
    void slotShape( int );
    void slotWidth( int );
    void slotHeight( int );
    void slotImage();

protected:
    QComboBox *     pShape;
    QSpinBox *      pWidth;
    QSpinBox *      pHeight;
    WImageButton *  pImage;
};

#endif
