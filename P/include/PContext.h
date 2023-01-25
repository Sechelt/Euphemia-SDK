#ifndef H_PContext
#define H_PContext

#include "P.h"

class PContext : public QObject 
{
    Q_OBJECT
public:
    static PContext *instance();

    void setImage( QImage *p );
    void setPen( const QPen & );
    void setBrush( const QBrush & );
    void setFont( const QFont & );

    QImage *getImage()      { return pImage; }
    QPen    getPen()        { return pen;   }
    QBrush  getBrush()      { return brush; }
    QFont   getFont()       { return font;  }

signals:
    void signalModified( const QPen & );
    void signalModified( const QBrush & );
    void signalModified( const QFont & );

protected:
    QImage *pImage = nullptr;
    QPen    pen;         // defines lines and outlines
    QBrush  brush;       // defines fill details
    QFont   font;        // defines font details for drawing text
};

#define g_Context PContext::instance()

#endif

