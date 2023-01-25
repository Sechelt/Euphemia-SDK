#ifndef H_PPenToolBar
#define H_PPenToolBar

#include <WColorButton.h>
#include <WLineStyleComboBox.h>

#include "PBrushToolBar.h"

class PPenToolBar : public QWidget
{
    Q_OBJECT
public:
    PPenToolBar( QWidget *pParent );

public slots:
    void slotRefresh( const QPen & );

protected:
    WColorButton *      pColor;
    WLineStyleComboBox *pStyle;
    QSpinBox *          pWidth;
    QToolButton *       pMore;

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

