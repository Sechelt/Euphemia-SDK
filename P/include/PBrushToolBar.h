#ifndef H_PBrushToolBar
#define H_PBrushToolBar

#include <WColorButton.h>
#include <WBrushStyleComboBox.h>
#include <WImageButton.h>

class PBrushToolBar : public QWidget
{
    Q_OBJECT
public:
    PBrushToolBar( QWidget *pParent );

public slots:
    void slotRefresh( const QBrush & );

protected:
    WColorButton *          pColor;
    WBrushStyleComboBox *   pStyle;
    WImageButton *          pImage;
    QToolButton *           pMore;

protected slots:
    void slotColor( const QColor &color );
    void slotStyle( Qt::BrushStyle n );
    void slotImage();
    void slotMore();
};


class PBrushSwatch : public QWidget
{
    Q_OBJECT
public:
    PBrushSwatch( const QBrush &brush, QWidget *pParent );

    void setBrush( const QBrush & );

protected:
    void paintEvent( QPaintEvent *pEvent );
    QSize sizeHint() const;

private:
    QBrush  brush;
};


class PBrushDialog : public QDialog
{
    Q_OBJECT
public:
    PBrushDialog( const QBrush &brush, QWidget *pParent = nullptr );

    QBrush getBrush() { return brush; }
    static QBrush getBrush( bool *pOk, const QBrush &brush, QWidget *pParent );

protected:
    QBrush                  brush;
    WColorButton *          pColor;
    WBrushStyleComboBox *   pStyleComboBox;
    WImageButton *          pImage;
    PBrushSwatch *          pSwatch;

protected slots:
    void slotColor( QColor color );
    void slotStyle( Qt::BrushStyle n );
    void slotImage();
};


#endif 

