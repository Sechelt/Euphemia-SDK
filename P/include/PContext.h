#ifndef H_PContext
#define H_PContext

#include <WPersist.h>

#include "P.h"

class PContextGradient
{
public:
    /*!< put these into a QComboBox with QGradient::Preset's to combine selecting for a preset or other supported gradient                              */
    /*!< this works because presets start at 0 and this is not likley to change                                                                         */
    enum StandardGradient       
    {
        StandardGradientLinear = -1,
        StandardGradientRadial = -2,
        StandardGradientConical = -3
    };

    int                 nType   = StandardGradientLinear;                  /*!< value will be either a QGradient::Preset or PContextGradient::StandardGradient               */
    QGradient::Spread   nSpread = QGradient::PadSpread;

    inline bool operator==( const PContextGradient &t ) 
    {
        if ( t.nType != nType ) return false;
        if ( t.nSpread != nSpread ) return false;
        return true;
    }

    void doSave( QDomDocument *pdomDoc, QDomElement *pdomElem );
    void doLoad( QDomElement *pdomElem );
};

class PContextGradientLinear
{
public:
    QLinearGradient gradient;

    inline bool operator==( const PContextGradientLinear &t ) 
    {
        if ( t.gradient != gradient ) return false;
        return true;
    }

    void doSave( QDomDocument *pdomDoc, QDomElement *pdomElem );
    void doLoad( QDomElement *pdomElem );
};

class PContextGradientRadial
{
public:
    QRadialGradient gradient;

    inline bool operator==( const PContextGradientRadial &t ) 
    {
        if ( t.gradient != gradient ) return false;
        return true;
    }

    void doSave( QDomDocument *pdomDoc, QDomElement *pdomElem );
    void doLoad( QDomElement *pdomElem );
};

class PContextGradientConical
{
public:
    QConicalGradient gradient;

    inline bool operator==( const PContextGradientConical &t ) 
    {
        if ( t.gradient != gradient ) return false;
        return true;
    }

    void doSave( QDomDocument *pdomDoc, QDomElement *pdomElem );
    void doLoad( QDomElement *pdomElem );
};

class PContextFreeHand
{
public:
    enum Shapes
    {
        ShapePen,           /*!< uses current pen to draw a line from last point to current point - easily most efficient   */
        ShapeEllipse,       /*!< ellipse filled with current pen color and of specified size                                */
        ShapeRectangle,     /*!< rectangle filled with current pen color and of specified size                              */
        ShapeCross,         /*!< a horizontal and vertical line with current pen color and of specified size                */
        ShapeImageScaled,   /*!< specified image scaled to size                                                             */
        ShapeImage          /*!< specified image                                                                            */
    };                                                                                                                      

    Shapes  nShape  = ShapePen;                                                                                            
    QSize   size    = QSize( 5, 5 );                                                                                        
    QImage  image;          /*!< this will be 'stamped' and/or 'dragged' - so not like using QBrush::texture                */

    inline bool operator==( const PContextFreeHand &t ) 
    {
        if ( t.nShape != nShape ) return false;
        if ( t.size != size ) return false;
        if ( t.image != image ) return false;
        return true;
    }

    void doSave( QDomDocument *pdomDoc, QDomElement *pdomElem );
    void doLoad( QDomElement *pdomElem );
};

class PContextErase
{
public:

    // NOTE: Very similar to FreeHand but Erase will not use pen/color as composition will be set to erase background with any color.
    //       Convenient to have seperate settings to save/restore default to.

    enum Shapes
    {
        ShapePen,                       /*!< uses current pen for width, shape is a circle              */
        ShapeEllipse,                   /*!< ellipse of specified size                                  */
        ShapeRectangle,                 /*!< rectangle of specified size                                */
        ShapeCross,                     /*!< a horizontal and vertical line of specified size           */
        ShapeImageScaled,               /*!< specified image scaled to size                             */
        ShapeImage                      /*!< specified image                                            */
    };

    Shapes  nShape  = ShapeEllipse;
    QSize   size    = QSize( 5, 5 );
    QImage  image;                      /*!< this will be 'stamped' and/or 'dragged'                    */

    inline bool operator==( const PContextErase &t ) 
    {
        if ( t.nShape != nShape ) return false;
        if ( t.size != size ) return false;
        if ( t.image != image ) return false;
        return true;
    }

    void doSave( QDomDocument *pdomDoc, QDomElement *pdomElem );
    void doLoad( QDomElement *pdomElem );
};

class PContextPaste
{
public:

    QPainter::CompositionMode nCompositionMode = QPainter::CompositionMode_SourceOver;
    bool bStamp = false;

    inline bool operator==( const PContextPaste &t ) 
    {
        if ( t.nCompositionMode != nCompositionMode ) return false;
        if ( t.bStamp != bStamp ) return false;
        return true;
    }

    void doSave( QDomDocument *pdomDoc, QDomElement *pdomElem );
    void doLoad( QDomElement *pdomElem );
};

class PContextSpray
{
public:
    int nRadius = 10;
    int nPoints = 5;

    inline bool operator==( const PContextSpray &t ) 
    {
        if ( t.nRadius != nRadius ) return false;
        if ( t.nPoints != nPoints ) return false;
        return true;
    }

    void doSave( QDomDocument *pdomDoc, QDomElement *pdomElem );
    void doLoad( QDomElement *pdomElem );
};

class PContextPolygonFilled
{
public:
    Qt::FillRule nFillRule = Qt::OddEvenFill;

    inline bool operator==( const PContextPolygonFilled &t ) 
    {
        if ( t.nFillRule != nFillRule ) return false;
        return true;
    }

    void doSave( QDomDocument *pdomDoc, QDomElement *pdomElem );
    void doLoad( QDomElement *pdomElem );
};

class PContextText
{
public:
    QString stringText;
    Qt::AlignmentFlag nHAlign = Qt::AlignHCenter;
    Qt::AlignmentFlag nVAlign = Qt::AlignVCenter;

    inline bool operator==( const PContextText &t ) 
    {
        if ( t.stringText != stringText ) return false;
        if ( t.nHAlign != nHAlign ) return false;
        if ( t.nVAlign != nVAlign ) return false;
        return true;
    }

    void doSave( QDomDocument *pdomDoc, QDomElement *pdomElem );
    void doLoad( QDomElement *pdomElem );
};

class PContextGeneral
{
public:
    PContextGeneral();

    QBrush  brushTransparency;          /*!< brush to use to represent transparency - default is block pattern but a solid color may be easier to work with     */
    bool    bRestoreState = false;      /*!< UI restore does not always work on Linux under Wayland due to a Qt problem - so here we can turn it on/off         */
    bool    bAutoCommit = true;         /*!< objects derived from PShapeBase can be manipulated before committing to image - here you can turn it on/off        */
    QGradient::Type nTypeGradient = QGradient::LinearGradient;      /*!< last used gradient type                                                                */

    inline bool operator==( const PContextGeneral &t ) 
    {
        if ( t.brushTransparency != brushTransparency ) return false;
        if ( t.bRestoreState != bRestoreState ) return false;
        if ( t.bAutoCommit != bAutoCommit ) return false;
        return true;
    }

    void doSave( QDomDocument *pdomDoc, QDomElement *pdomElem );
    void doLoad( QDomElement *pdomElem );
};

class PContext : public QObject 
{
    Q_OBJECT
public:
    PContext();

    static PContext *instance();

    void setImage( QImage *p );
    void setPen( const QPen & );
    void setBrush( const QBrush & );
    void setFont( const QFont & );
    void setGradient( const PContextGradient & );
    void setGradientLinear( const PContextGradientLinear & );
    void setGradientRadial( const PContextGradientRadial & );
    void setGradientConical( const PContextGradientConical & );
    void setFreeHand( const PContextFreeHand & );
    void setErase( const PContextErase & );
    void setSpray( const PContextSpray & );
    void setText( const PContextText & );
    void setPolygonFilled( const PContextPolygonFilled & );
    void setPaste( const PContextPaste & );
    void setGeneral( const PContextGeneral & );

    QImage *                getImage()          { return pImage;            }
    QPen                    getPen()            { return pen;               }
    QBrush                  getBrush()          { return brush;             }
    QFont                   getFont()           { return font;              }
    PContextGradient        getGradient()       { return gradient;          }
    PContextGradientLinear  getGradientLinear() { return gradientlinear;    }   
    PContextGradientRadial  getGradientRadial() { return gradientradial;    }   
    PContextGradientConical getGradientConical(){ return gradientconical;   }   
    PContextFreeHand        getFreeHand()       { return freehand;          }
    PContextErase           getErase()          { return erase;             }
    PContextSpray           getSpray()          { return spray;             }
    PContextText            getText()           { return text;              }
    PContextPolygonFilled   getPolygonFilled()  { return polygonfilled;     }
    PContextPaste           getPaste()          { return paste;             }
    PContextGeneral         getGeneral()        { return general;           }

    void doSave();
    void doLoad();

signals:
    void signalModified( const QPen & );
    void signalModified( const QBrush & );
    void signalModified( const QFont & );
    void signalModified( const PContextGradient & );
    void signalModified( const PContextGradientLinear & );
    void signalModified( const PContextGradientRadial & );
    void signalModified( const PContextGradientConical & );
    void signalModified( const PContextFreeHand & );
    void signalModified( const PContextSpray & );
    void signalModified( const PContextErase & );
    void signalModified( const PContextText & );
    void signalModified( const PContextPolygonFilled & );
    void signalModified( const PContextPaste & );
    void signalModified( const PContextGeneral & );

public slots:
    void slotImage( QImage *p );
    void slotPen( const QPen &t );
    void slotBrush( const QBrush &t );
    void slotFont( const QFont &t );
    void slotGradient( const PContextGradient &t );
    void slotGradientLinear( const PContextGradientLinear &t );
    void slotGradientRadial( const PContextGradientRadial &t );
    void slotGradientConical( const PContextGradientConical &t );
    void slotFreeHand( const PContextFreeHand &t );
    void slotErase( const PContextErase &t );
    void slotSpray( const PContextSpray &t );
    void slotText( const PContextText &t );
    void slotPolygonFilled( const PContextPolygonFilled &t );
    void slotPaste( const PContextPaste &t );
    void slotGeneral( const PContextGeneral &t );

protected:
    QString                 stringFileName;             /*!< we load/save defaults here             */
    QImage *                pImage = nullptr;           /*!< tool draws on this image               */
    QPen                    pen;                        /*!< defines lines and outlines             */
    QBrush                  brush;                      /*!< defines fill details                   */
    QFont                   font;                       /*!< defines font details for drawing text  */
    PContextGradient        gradient;
    PContextGradientLinear  gradientlinear;             /*!< used by gradient fill tool             */
    PContextGradientRadial  gradientradial;             /*!< used by gradient fill tool             */
    PContextGradientConical gradientconical;            /*!< used by gradient fill tool             */
    PContextFreeHand        freehand;                   /*!< used by free hand tool                 */
    PContextSpray           spray;                      /*!< used by spray tool                     */
    PContextErase           erase;                      /*!< used by erase tool                     */
    PContextText            text;                       /*!< used by text tool                      */
    PContextPolygonFilled   polygonfilled;              /*!< used by polygon tool                   */
    PContextPaste           paste;                      /*!< used by paste                          */
    PContextGeneral         general;                    
};

#define g_Context PContext::instance()

#endif

