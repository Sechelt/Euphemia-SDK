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
