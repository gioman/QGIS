/***************************************************************************
 qgssymbol.h
 ---------------------
 begin                : November 2009
 copyright            : (C) 2009 by Martin Dobias
 email                : wonder dot sk at gmail dot com
 ***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#ifndef QGSSYMBOL_H
#define QGSSYMBOL_H

#include "qgis_core.h"
#include "qgis.h"
#include <QList>
#include <QMap>
#include "qgsmapunitscale.h"
#include "qgspointv2.h"
#include "qgsfeature.h"
#include "qgsfields.h"
#include "qgsrendercontext.h"
#include "qgsproperty.h"

class QColor;
class QImage;
class QPainter;
class QSize;
class QPointF;
class QPolygonF;

class QDomDocument;
class QDomElement;
//class

class QgsFields;
class QgsSymbolLayer;
class QgsRenderContext;
class QgsVectorLayer;
class QgsPaintEffect;
class QgsMarkerSymbolLayer;
class QgsLineSymbolLayer;
class QgsFillSymbolLayer;
class QgsSymbolRenderContext;
class QgsFeatureRenderer;
class QgsCurve;
class QgsPolygonV2;
class QgsExpressionContext;

typedef QList<QgsSymbolLayer *> QgsSymbolLayerList;

/** \ingroup core
 * \class QgsSymbol
 */
class CORE_EXPORT QgsSymbol
{
    friend class QgsFeatureRenderer;

  public:

    /**
     * Type of the symbol
     */
    enum SymbolType
    {
      Marker, //!< Marker symbol
      Line,   //!< Line symbol
      Fill,   //!< Fill symbol
      Hybrid  //!< Hybrid symbol
    };

    /**
     * Scale method
     */
    enum ScaleMethod
    {
      ScaleArea,     //!< Calculate scale by the area
      ScaleDiameter  //!< Calculate scale by the diameter
    };


    //! Flags controlling behavior of symbols during rendering
    enum RenderHint
    {
      DynamicRotation = 2, //!< Rotation of symbol may be changed during rendering and symbol should not be cached
    };
    Q_DECLARE_FLAGS( RenderHints, RenderHint )

    virtual ~QgsSymbol();

    //! return new default symbol for specified geometry type
    static QgsSymbol *defaultSymbol( QgsWkbTypes::GeometryType geomType ) SIP_FACTORY;

    SymbolType type() const { return mType; }

    // symbol layers handling

    /** Returns list of symbol layers contained in the symbol.
     * \returns symbol layers list
     * \since QGIS 2.7
     * \see symbolLayer
     * \see symbolLayerCount
     */
    QgsSymbolLayerList symbolLayers() { return mLayers; }

    /** Returns a specific symbol layers contained in the symbol.
     * \param layer layer number
     * \returns corresponding symbol layer
     * \since QGIS 2.7
     * \see symbolLayers
     * \see symbolLayerCount
     */
    QgsSymbolLayer *symbolLayer( int layer );

    /** Returns total number of symbol layers contained in the symbol.
     * \returns count of symbol layers
     * \since QGIS 2.7
     * \see symbolLayers
     * \see symbolLayer
     */
    int symbolLayerCount() { return mLayers.count(); }

    /**
     * Insert symbol layer to specified index
     * Ownership will be transferred.
     * \param index The index at which the layer should be added
     * \param layer The symbol layer to add
     * \returns True if the layer is added, False if the index or the layer is bad
     */
    bool insertSymbolLayer( int index, QgsSymbolLayer *layer SIP_TRANSFER );

    /**
     * Append symbol layer at the end of the list
     * Ownership will be transferred.
     * \param layer The layer to add
     * \returns True if the layer is added, False if the layer is bad
     */
    bool appendSymbolLayer( QgsSymbolLayer *layer SIP_TRANSFER );

    //! delete symbol layer at specified index
    bool deleteSymbolLayer( int index );

    /**
     * Remove symbol layer from the list and return pointer to it.
     * Ownership is handed to the caller.
     * \param index The index of the layer to remove
     * \returns A pointer to the removed layer
     */
    QgsSymbolLayer *takeSymbolLayer( int index ) SIP_TRANSFERBACK;

    //! delete layer at specified index and set a new one
    bool changeSymbolLayer( int index, QgsSymbolLayer *layer SIP_TRANSFER );

    /** Begins the rendering process for the symbol. This must be called before renderFeature(),
     * and should be followed by a call to stopRender().
     * \param context render context which symbol will be drawn using
     * \param fields fields for features to be rendered (usually the associated
     * vector layer's fields). Required for correct calculation of data defined
     * overrides.
     * \see stopRender()
     */
    void startRender( QgsRenderContext &context, const QgsFields &fields = QgsFields() );

    /** Ends the rendering process. This should be called after rendering all desired features.
     * \param context render context, must match the context specified when startRender()
     * was called.
     * \see startRender()
     */
    void stopRender( QgsRenderContext &context );

    void setColor( const QColor &color );
    QColor color() const;

    //! Draw icon of the symbol that occupyies area given by size using the painter.
    //! Optionally custom context may be given in order to get rendering of symbols that use map units right.
    //! \since QGIS 2.6
    void drawPreviewIcon( QPainter *painter, QSize size, QgsRenderContext *customContext = nullptr );

    //! export symbol as image format. PNG and SVG supported
    void exportImage( const QString &path, const QString &format, QSize size );

    //! Generate symbol as image
    QImage asImage( QSize size, QgsRenderContext *customContext = nullptr );

    /** Returns a large (roughly 100x100 pixel) preview image for the symbol.
     * \param expressionContext optional expression context, for evaluation of
     * data defined symbol properties
     */
    QImage bigSymbolPreviewImage( QgsExpressionContext *expressionContext = nullptr );

    QString dump() const;

    /**
     * Get a deep copy of this symbol.
     * Needs to be reimplemented by subclasses.
     * Ownership is transferred to the caller.
     */
    virtual QgsSymbol *clone() const = 0 SIP_FACTORY;

    void toSld( QDomDocument &doc, QDomElement &element, QgsStringMap props ) const;

    /** Returns the units to use for sizes and widths within the symbol. Individual
     * symbol layer definitions will interpret this in different ways, e.g., a marker symbol
     * may use it to specify the units for the marker size, while a line symbol
     * may use it to specify the units for the line width.
     * \returns output unit, or QgsUnitTypes::RenderUnknownUnit if the symbol contains mixed units
     * \see setOutputUnit()
     */
    QgsUnitTypes::RenderUnit outputUnit() const;

    /** Sets the units to use for sizes and widths within the symbol. Individual
     * symbol definitions will interpret this in different ways, e.g., a marker symbol
     * may use it to specify the units for the marker size, while a line symbol
     * may use it to specify the units for the line width.
     * \param unit output units
     * \see outputUnit()
     */
    void setOutputUnit( QgsUnitTypes::RenderUnit unit );

    QgsMapUnitScale mapUnitScale() const;
    void setMapUnitScale( const QgsMapUnitScale &scale );

    //! Get alpha transparency 1 for opaque, 0 for invisible
    qreal alpha() const { return mAlpha; }
    //! Set alpha transparency 1 for opaque, 0 for invisible
    void setAlpha( qreal alpha ) { mAlpha = alpha; }

    /** Sets rendering hint flags for the symbol.
     * \see renderHints()
     */
    void setRenderHints( RenderHints hints ) { mRenderHints = hints; }

    /** Returns the rendering hint flags for the symbol.
     * \see setRenderHints()
     */
    RenderHints renderHints() const { return mRenderHints; }

    /** Sets whether features drawn by the symbol should be clipped to the render context's
     * extent. If this option is enabled then features which are partially outside the extent
     * will be clipped. This speeds up rendering of the feature, but may have undesirable
     * side effects for certain symbol types.
     * \param clipFeaturesToExtent set to true to enable clipping (defaults to true)
     * \since QGIS 2.9
     * \see clipFeaturesToExtent
     */
    void setClipFeaturesToExtent( bool clipFeaturesToExtent ) { mClipFeaturesToExtent = clipFeaturesToExtent; }

    /** Returns whether features drawn by the symbol will be clipped to the render context's
     * extent. If this option is enabled then features which are partially outside the extent
     * will be clipped. This speeds up rendering of the feature, but may have undesirable
     * side effects for certain symbol types.
     * \returns true if features will be clipped
     * \since QGIS 2.9
     * \see setClipFeaturesToExtent
     */
    bool clipFeaturesToExtent() const { return mClipFeaturesToExtent; }

    /**
     * Return a list of attributes required to render this feature.
     * This should include any attributes required by the symbology including
     * the ones required by expressions.
     */
    QSet<QString> usedAttributes( const QgsRenderContext &context ) const;

    /** Returns whether the symbol utilizes any data defined properties.
     * \since QGIS 2.12
     */
    bool hasDataDefinedProperties() const;

    //! \note the layer will be NULL after stopRender
    void setLayer( const QgsVectorLayer *layer ) { mLayer = layer; }
    const QgsVectorLayer *layer() const { return mLayer; }

    /**
     * Render a feature. Before calling this the startRender() method should be called to initialize
     * the rendering process. After rendering all features stopRender() must be called.
     */
    void renderFeature( const QgsFeature &feature, QgsRenderContext &context, int layer = -1, bool selected = false, bool drawVertexMarker = false, int currentVertexMarkerType = 0, int currentVertexMarkerSize = 0 );

    /**
     * Returns the symbol render context. Only valid between startRender and stopRender calls.
     *
     * \returns The symbol render context
     */
    QgsSymbolRenderContext *symbolRenderContext();

  protected:
    QgsSymbol( SymbolType type, const QgsSymbolLayerList &layers SIP_TRANSFER ); // can't be instantiated

    /**
     * Creates a point in screen coordinates from a QgsPointV2 in map coordinates
     */
    static inline QPointF _getPoint( QgsRenderContext &context, const QgsPointV2 &point )
    {
      QPointF pt;
      if ( context.coordinateTransform().isValid() )
      {
        double x = point.x();
        double y = point.y();
        double z = 0.0;
        context.coordinateTransform().transformInPlace( x, y, z );
        pt = QPointF( x, y );

      }
      else
        pt = point.toQPointF();

      context.mapToPixel().transformInPlace( pt.rx(), pt.ry() );
      return pt;
    }

    /**
     * Creates a line string in screen coordinates from a QgsCurve in map coordinates
     */
    static QPolygonF _getLineString( QgsRenderContext &context, const QgsCurve &curve, bool clipToExtent = true );

    /**
     * Creates a polygon ring in screen coordinates from a QgsCurve in map coordinates
     */
    static QPolygonF _getPolygonRing( QgsRenderContext &context, const QgsCurve &curve, bool clipToExtent );

    /**
     * Creates a polygon in screen coordinates from a QgsPolygon in map coordinates
     */
    static void _getPolygon( QPolygonF &pts, QList<QPolygonF> &holes, QgsRenderContext &context, const QgsPolygonV2 &polygon, bool clipToExtent = true );

    /**
     * Retrieve a cloned list of all layers that make up this symbol.
     * Ownership is transferred to the caller.
     */
    QgsSymbolLayerList cloneLayers() const SIP_FACTORY;

    /**
     * Renders a context using a particular symbol layer without passing in a
     * geometry. This is used as fallback, if the symbol being rendered is not
     * compatible with the specified layer. In such a case, this method can be
     * called and will call the layer's rendering method anyway but the
     * geometry passed to the layer will be empty.
     * This is required for layers that generate their own geometry from other
     * information in the rendering context.
     */
    void renderUsingLayer( QgsSymbolLayer *layer, QgsSymbolRenderContext &context );

    //! Render editing vertex marker at specified point
    //! \since QGIS 2.16
    void renderVertexMarker( QPointF pt, QgsRenderContext &context, int currentVertexMarkerType, int currentVertexMarkerSize );

    SymbolType mType;
    QgsSymbolLayerList mLayers;

    //! Symbol opacity (in the range 0 - 1)
    qreal mAlpha;

    RenderHints mRenderHints;
    bool mClipFeaturesToExtent;

    const QgsVectorLayer *mLayer; //current vectorlayer

  private:
    //! Initialized in startRender, destroyed in stopRender
    std::unique_ptr< QgsSymbolRenderContext > mSymbolRenderContext;

    Q_DISABLE_COPY( QgsSymbol )

};

Q_DECLARE_OPERATORS_FOR_FLAGS( QgsSymbol::RenderHints )

///////////////////////

/** \ingroup core
 * \class QgsSymbolRenderContext
 */
class CORE_EXPORT QgsSymbolRenderContext
{
  public:

    /** Constructor for QgsSymbolRenderContext
     * \param c
     * \param u
     * \param alpha
     * \param selected set to true if symbol should be drawn in a "selected" state
     * \param renderHints flags controlling rendering behavior
     * \param f
     * \param fields
     * \param mapUnitScale
     */
    QgsSymbolRenderContext( QgsRenderContext &c, QgsUnitTypes::RenderUnit u, qreal alpha = 1.0, bool selected = false, QgsSymbol::RenderHints renderHints = 0, const QgsFeature *f = nullptr, const QgsFields &fields = QgsFields(), const QgsMapUnitScale &mapUnitScale = QgsMapUnitScale() );

    QgsRenderContext &renderContext() { return mRenderContext; }
    const QgsRenderContext &renderContext() const { return mRenderContext; }

    /** Sets the original value variable value for data defined symbology
     * \param value value for original value variable. This usually represents the symbol property value
     * before any data defined overrides have been applied.
     * \since QGIS 2.12
     */
    void setOriginalValueVariable( const QVariant &value );

    //! Returns the output unit for the context
    QgsUnitTypes::RenderUnit outputUnit() const { return mOutputUnit; }

    //! Sets the output unit for the context
    void setOutputUnit( QgsUnitTypes::RenderUnit u ) { mOutputUnit = u; }

    QgsMapUnitScale mapUnitScale() const { return mMapUnitScale; }
    void setMapUnitScale( const QgsMapUnitScale &scale ) { mMapUnitScale = scale; }

    //! Get alpha transparency 1 for opaque, 0 for invisible
    qreal alpha() const { return mAlpha; }
    //! Set alpha transparency 1 for opaque, 0 for invisible
    void setAlpha( qreal alpha ) { mAlpha = alpha; }

    bool selected() const { return mSelected; }
    void setSelected( bool selected ) { mSelected = selected; }

    /** Returns the rendering hint flags for the symbol.
     * \see setRenderHints()
     */
    QgsSymbol::RenderHints renderHints() const { return mRenderHints; }

    /** Sets rendering hint flags for the symbol.
     * \see renderHints()
     */
    void setRenderHints( QgsSymbol::RenderHints hints ) { mRenderHints = hints; }

    void setFeature( const QgsFeature *f ) { mFeature = f; }
    //! Current feature being rendered - may be null
    const QgsFeature *feature() const { return mFeature; }

    /**
     * Sets the geometry type for the original feature geometry being rendered.
     * \see originalGeometryType()
     * \since QGIS 3.0
     */
    void setOriginalGeometryType( QgsWkbTypes::GeometryType type ) { mOriginalGeometryType = type; }

    /**
     * Returns the geometry type for the original feature geometry being rendered. This can be
     * useful if symbol layers alter their appearance based on geometry type - eg offsetting a
     * simple line style will look different if the simple line is rendering a polygon feature
     * (a closed buffer) vs a line feature (an unclosed offset line).
     * \see originalGeometryType()
     * \since QGIS 3.0
     */
    QgsWkbTypes::GeometryType originalGeometryType() const { return mOriginalGeometryType; }

    //! Fields of the layer. Currently only available in startRender() calls
    //! to allow symbols with data-defined properties prepare the expressions
    //! (other times fields() returns null)
    //! \since QGIS 2.4
    QgsFields fields() const { return mFields; }

    /** Part count of current geometry
     * \since QGIS 2.16
     */
    int geometryPartCount() const { return mGeometryPartCount; }

    /** Sets the part count of current geometry
     * \since QGIS 2.16
     */
    void setGeometryPartCount( int count ) { mGeometryPartCount = count; }

    /** Part number of current geometry
     * \since QGIS 2.16
     */
    int geometryPartNum() const { return mGeometryPartNum; }

    /** Sets the part number of current geometry
     * \since QGIS 2.16
     */
    void setGeometryPartNum( int num ) { mGeometryPartNum = num; }

    double outputLineWidth( double width ) const;
    double outputPixelSize( double size ) const;

    // workaround for sip 4.7. Don't use assignment - will fail with assertion error
    QgsSymbolRenderContext &operator=( const QgsSymbolRenderContext & );

    /**
     * This scope is always available when a symbol of this type is being rendered.
     *
     * \returns An expression scope for details about this symbol
     */
    QgsExpressionContextScope *expressionContextScope();

    /**
     * Set an expression scope for this symbol.
     *
     * Will take ownership.
     *
     * \param contextScope An expression scope for details about this symbol
     */
    void setExpressionContextScope( QgsExpressionContextScope *contextScope SIP_TRANSFER );

  private:
    QgsRenderContext &mRenderContext;
    std::unique_ptr< QgsExpressionContextScope > mExpressionContextScope;
    QgsUnitTypes::RenderUnit mOutputUnit;
    QgsMapUnitScale mMapUnitScale;
    qreal mAlpha;
    bool mSelected;
    QgsSymbol::RenderHints mRenderHints;
    const QgsFeature *mFeature; //current feature
    QgsFields mFields;
    int mGeometryPartCount;
    int mGeometryPartNum;
    QgsWkbTypes::GeometryType mOriginalGeometryType = QgsWkbTypes::UnknownGeometry;


    QgsSymbolRenderContext( const QgsSymbolRenderContext &rh );
};



//////////////////////


/** \ingroup core
 * \class QgsMarkerSymbol
 */
class CORE_EXPORT QgsMarkerSymbol : public QgsSymbol
{
  public:

    /** Create a marker symbol with one symbol layer: SimpleMarker with specified properties.
     * This is a convenience method for easier creation of marker symbols.
     */
    static QgsMarkerSymbol *createSimple( const QgsStringMap &properties ) SIP_FACTORY;

    QgsMarkerSymbol( const QgsSymbolLayerList &layers SIP_TRANSFER = QgsSymbolLayerList() );

    /** Sets the angle for the whole symbol. Individual symbol layer sizes
     * will be rotated to maintain their current relative angle to the whole symbol angle.
     * \param symbolAngle new symbol angle
     * \see angle()
     */
    void setAngle( double symbolAngle );

    /** Returns the marker angle for the whole symbol. Note that for symbols with
     * multiple symbol layers, this will correspond just to the angle of
     * the first symbol layer.
     * \since QGIS 2.16
     * \see setAngle()
     */
    double angle() const;

    /** Set data defined angle for whole symbol (including all symbol layers).
     * \since QGIS 3.0
     * \see dataDefinedAngle()
     */
    void setDataDefinedAngle( const QgsProperty &property );

    /** Returns data defined angle for whole symbol (including all symbol layers).
     * \returns data defined angle, or invalid property if angle is not set
     * at the marker level.
     * \since QGIS 3.0
     * \see setDataDefinedAngle()
     */
    QgsProperty dataDefinedAngle() const;

    /** Sets the line angle modification for the symbol's angle. This angle is added to
     * the marker's rotation and data defined rotation before rendering the symbol, and
     * is usually used for orienting symbols to match a line's angle.
     * \param lineAngle Angle in degrees, valid values are between 0 and 360
     * \since QGIS 2.9
     */
    void setLineAngle( double lineAngle );

    /** Sets the size for the whole symbol. Individual symbol layer sizes
     * will be scaled to maintain their current relative size to the whole symbol size.
     * \param size new symbol size
     * \see size()
     * \see setSizeUnit()
     * \see setSizeMapUnitScale()
     */
    void setSize( double size );

    /** Returns the size for the whole symbol, which is the maximum size of
     * all marker symbol layers in the symbol.
     * \see setSize()
     * \see sizeUnit()
     * \see sizeMapUnitScale()
     */
    double size() const;

    /** Sets the size units for the whole symbol (including all symbol layers).
     * \param unit size units
     * \since QGIS 2.16
     * \see sizeUnit()
     * \see setSizeMapUnitScale()
     * \see setSize()
     */
    void setSizeUnit( QgsUnitTypes::RenderUnit unit );

    /** Returns the size units for the whole symbol (including all symbol layers).
     * \returns size units, or mixed units if symbol layers have different units
     * \since QGIS 2.16
     * \see setSizeUnit()
     * \see sizeMapUnitScale()
     * \see size()
     */
    QgsUnitTypes::RenderUnit sizeUnit() const;

    /** Sets the size map unit scale for the whole symbol (including all symbol layers).
     * \param scale map unit scale
     * \since QGIS 2.16
     * \see sizeMapUnitScale()
     * \see setSizeUnit()
     * \see setSize()
     */
    void setSizeMapUnitScale( const QgsMapUnitScale &scale );

    /** Returns the size map unit scale for the whole symbol. Note that for symbols with
     * multiple symbol layers, this will correspond just to the map unit scale
     * for the first symbol layer.
     * \since QGIS 2.16
     * \see setSizeMapUnitScale()
     * \see sizeUnit()
     * \see size()
     */
    QgsMapUnitScale sizeMapUnitScale() const;

    /** Set data defined size for whole symbol (including all symbol layers).
     * \since QGIS 3.0
     * \see dataDefinedSize()
     */
    void setDataDefinedSize( const QgsProperty &property );

    /** Returns data defined size for whole symbol (including all symbol layers).
     * \returns data defined size, or invalid property if size is not set
     * at the marker level.
     * \since QGIS 3.0
     * \see setDataDefinedSize
     */
    QgsProperty dataDefinedSize() const;

    void setScaleMethod( QgsSymbol::ScaleMethod scaleMethod );
    ScaleMethod scaleMethod();

    void renderPoint( QPointF point, const QgsFeature *f, QgsRenderContext &context, int layer = -1, bool selected = false );

    /** Returns the approximate bounding box of the marker symbol, which includes the bounding box
     * of all symbol layers for the symbol. It is recommended to use this method only between startRender()
     * and stopRender() calls, or data defined rotation and offset will not be correctly calculated.
     * \param point location of rendered point in painter units
     * \param context render context
     * \param feature feature being rendered at point (optional). If not specified, the bounds calculation will not
     * include data defined parameters such as offset and rotation
     * \returns approximate symbol bounds, in painter units
     * \since QGIS 2.14
    */
    QRectF bounds( QPointF point, QgsRenderContext &context, const QgsFeature &feature = QgsFeature() ) const;

    virtual QgsMarkerSymbol *clone() const override SIP_FACTORY;

  private:

    void renderPointUsingLayer( QgsMarkerSymbolLayer *layer, QPointF point, QgsSymbolRenderContext &context );

};


/** \ingroup core
 * \class QgsLineSymbol
 */
class CORE_EXPORT QgsLineSymbol : public QgsSymbol
{
  public:

    /** Create a line symbol with one symbol layer: SimpleLine with specified properties.
     * This is a convenience method for easier creation of line symbols.
     */
    static QgsLineSymbol *createSimple( const QgsStringMap &properties ) SIP_FACTORY;

    QgsLineSymbol( const QgsSymbolLayerList &layers SIP_TRANSFER = QgsSymbolLayerList() );

    void setWidth( double width );
    double width() const;

    /** Set data defined width for whole symbol (including all symbol layers).
     * \see dataDefinedWidth()
     * \since QGIS 3.0
     */
    void setDataDefinedWidth( const QgsProperty &property );

    /** Returns data defined width for whole symbol (including all symbol layers).
     * \returns data defined width, or invalid property if size is not set
     * at the line level. Caller takes responsibility for deleting the returned object.
     * \since QGIS 3.0
     * \see setDataDefinedWidth
     */
    QgsProperty dataDefinedWidth() const;

    void renderPolyline( const QPolygonF &points, const QgsFeature *f, QgsRenderContext &context, int layer = -1, bool selected = false );

    virtual QgsLineSymbol *clone() const override SIP_FACTORY;

  private:

    void renderPolylineUsingLayer( QgsLineSymbolLayer *layer, const QPolygonF &points, QgsSymbolRenderContext &context );

};


/** \ingroup core
 * \class QgsFillSymbol
 */
class CORE_EXPORT QgsFillSymbol : public QgsSymbol
{
  public:

    /** Create a fill symbol with one symbol layer: SimpleFill with specified properties.
     * This is a convenience method for easier creation of fill symbols.
     */
    static QgsFillSymbol *createSimple( const QgsStringMap &properties ) SIP_FACTORY;

    QgsFillSymbol( const QgsSymbolLayerList &layers SIP_TRANSFER = QgsSymbolLayerList() );
    void setAngle( double angle );
    void renderPolygon( const QPolygonF &points, QList<QPolygonF> *rings, const QgsFeature *f, QgsRenderContext &context, int layer = -1, bool selected = false );

    virtual QgsFillSymbol *clone() const override SIP_FACTORY;

  private:

    void renderPolygonUsingLayer( QgsSymbolLayer *layer, const QPolygonF &points, QList<QPolygonF> *rings, QgsSymbolRenderContext &context );
    //! Calculates the bounds of a polygon including rings
    QRectF polygonBounds( const QPolygonF &points, const QList<QPolygonF> *rings ) const;
    //! Translates the rings in a polygon by a set distance
    QList<QPolygonF> *translateRings( const QList<QPolygonF> *rings, double dx, double dy ) const;
};

#endif

