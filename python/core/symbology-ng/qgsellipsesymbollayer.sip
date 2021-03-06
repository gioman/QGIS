class QgsEllipseSymbolLayer : QgsMarkerSymbolLayer
{
%TypeHeaderCode
#include <qgsellipsesymbollayer.h>
%End

  public:
    QgsEllipseSymbolLayer();
    ~QgsEllipseSymbolLayer();

    static QgsSymbolLayer *create( const QgsStringMap &properties = QgsStringMap() ) /Factory/;
    static QgsSymbolLayer *createFromSld( QDomElement &element ) /Factory/;

    void renderPoint( QPointF point, QgsSymbolRenderContext &context );
    QString layerType() const;
    void startRender( QgsSymbolRenderContext &context );
    void stopRender( QgsSymbolRenderContext &context );
    virtual QgsEllipseSymbolLayer *clone() const /Factory/;
    QgsStringMap properties() const;

    void toSld( QDomDocument &doc, QDomElement &element, const QgsStringMap &props ) const;
    void writeSldMarker( QDomDocument &doc, QDomElement &element, const QgsStringMap &props ) const;

    bool writeDxf( QgsDxfExport &e, double mmMapUnitScaleFactor, const QString &layerName, QgsSymbolRenderContext &context, QPointF shift = QPointF( 0.0, 0.0 ) ) const;

    void setSymbolName( const QString &name );
    QString symbolName() const;

    void setSymbolWidth( double w );
    double symbolWidth() const;

    void setSymbolHeight( double h );
    double symbolHeight() const;

    Qt::PenStyle strokeStyle() const;
    void setStrokeStyle( Qt::PenStyle strokeStyle );

    /** Get stroke join style.
     * @note added in 2.16 */
    Qt::PenJoinStyle penJoinStyle() const;
    /** Set stroke join style.
     * @note added in 2.16 */
    void setPenJoinStyle( Qt::PenJoinStyle style );

    void setStrokeWidth( double w );
    double strokeWidth() const;

    void setFillColor( const QColor &c );
    QColor fillColor() const;

    void setStrokeColor( const QColor &c );
    QColor strokeColor() const;

    /** Sets the units for the symbol's width.
     * @param unit symbol units
     * @see symbolWidthUnit()
     * @see setSymbolHeightUnit()
    */
    void setSymbolWidthUnit( QgsUnitTypes::RenderUnit unit );

    /** Returns the units for the symbol's width.
     * @see setSymbolWidthUnit()
     * @see symbolHeightUnit()
    */
    QgsUnitTypes::RenderUnit symbolWidthUnit() const;

    void setSymbolWidthMapUnitScale( const QgsMapUnitScale &scale);
    const QgsMapUnitScale &symbolWidthMapUnitScale() const;

    /** Sets the units for the symbol's height.
     * @param unit symbol units
     * @see symbolHeightUnit()
     * @see setSymbolWidthUnit()
    */
    void setSymbolHeightUnit( QgsUnitTypes::RenderUnit unit );

    /** Returns the units for the symbol's height.
     * @see setSymbolHeightUnit()
     * @see symbolWidthUnit()
    */
    QgsUnitTypes::RenderUnit symbolHeightUnit() const;

    void setSymbolHeightMapUnitScale( const QgsMapUnitScale &scale);
    const QgsMapUnitScale &symbolHeightMapUnitScale() const;

    /** Sets the units for the symbol's stroke width.
     * @param unit symbol units
     * @see strokeWidthUnit()
    */
    void setStrokeWidthUnit( QgsUnitTypes::RenderUnit unit );

    /** Returns the units for the symbol's stroke width.
     * @see setStrokeWidthUnit()
    */
    QgsUnitTypes::RenderUnit strokeWidthUnit() const;

    void setStrokeWidthMapUnitScale( const QgsMapUnitScale &scale);
    const QgsMapUnitScale &strokeWidthMapUnitScale() const;

    void setOutputUnit( QgsUnitTypes::RenderUnit unit );
    QgsUnitTypes::RenderUnit outputUnit() const;

    void setMapUnitScale( const QgsMapUnitScale &scale );
    QgsMapUnitScale mapUnitScale() const;

    QRectF bounds( QPointF point, QgsSymbolRenderContext &context );
};
