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

#ifndef H_PPenToolBar
#define H_PPenToolBar

#include "P.h"

#include <WColorButton.h>
#include <WLineStyleComboBox.h>

#include "PBrushToolBar.h"

class PPenToolBar : public QWidget
{
    Q_OBJECT
public:
    PPenToolBar( QWidget *pParent, bool bCompress = false );

public slots:
    void slotRefresh( const QPen & );

protected:
    WColorButton *      pColor  = nullptr;
    WLineStyleComboBox *pStyle  = nullptr;
    QSpinBox *          pWidth  = nullptr;
    QToolButton *       pMore   = nullptr;

protected slots:
    void slotColor( const QColor &color );
    void slotStyle( Qt::PenStyle n );
    void slotWidth( int n );
    void slotMore();
};

class PPenSwatch : public QWidget
{
public:
    PPenSwatch( const QPen &pen, QWidget *pParent );

    void setPen( const QPen &pen );

protected:
    virtual void paintEvent( QPaintEvent *pEvent );
    virtual QSize sizeHint() const;

private:
    QPen pen;
};

class PPenDialog : public QDialog
{
    Q_OBJECT
public:
    PPenDialog( const QPen &pen, QWidget *pParent = nullptr );

    QPen getPen() { return pen; }
    static QPen getPen( bool *pOk, const QPen &pen, QWidget *pParent );

protected:
    QPen                pen;
    // pen
    QComboBox *         pCapStyle;
    WColorButton *      pColor;
    QComboBox *         pJoinStyle;
    QSpinBox *          pMiterLimit;
    WLineStyleComboBox *pStyle;
    QSpinBox *          pWidth;
    // brush (if any) to fill strokes
    WColorButton *          pColorBrush;
    WBrushStyleComboBox *   pStyleBrush;
    QToolButton *           pMoreBrush;
    // swatch           
    PPenSwatch *        pSwatch;

protected slots:
    // pen
    void slotCapStyle( int nIndex );
    void slotColor( const QColor &color );
    void slotJoinStyle( int nIndex );
    void slotMiterLimit( int n );
    void slotStyle( Qt::PenStyle n );
    void slotWidth( int n );
    // brush (if any) to fill strokes
    void slotColorBrush( const QColor &color );
    void slotStyleBrush( Qt::BrushStyle n );
    void slotMoreBrush();
};

#endif 

