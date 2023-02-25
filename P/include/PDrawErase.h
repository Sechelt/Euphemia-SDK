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

#ifndef H_PDrawErase
#define H_PDrawErase

#include <WImageButton.h>

#include "PFreeBase.h"

class PDrawErase : public PFreeBase
{
public:
    PDrawErase( PCanvas *pCanvas );

    void doPress( PMouseEvent *pEvent ) override;
    void doMove( PMouseEvent *pEvent ) override;
    void doRelease( PMouseEvent *pEvent ) override;

protected:
    QPoint              pointLast;
    QColor              colorTransparent;
    QPen                pen;
    PContextErase       t;

    void doErase( const QPoint & );
    void doErasePen( const QPoint & );
    void doEraseEllipse( const QPoint & );
    void doEraseRectangle( const QPoint & );
    void doEraseCross( const QPoint & );
    void doEraseImage( const QPoint & );

private:
    QImage *getMask( QImage *pImage );

    void doErase( const QRect &rect, const QImage &imageMask, QImage *pImageCanvas, const QColor &colorMask );
};

class PEraseToolBar : public QWidget
{
    Q_OBJECT
public:
    PEraseToolBar( QWidget *p );

public slots:
    void slotRefresh( const PContextErase & );

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
