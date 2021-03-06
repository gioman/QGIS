/************************************************************************
 * This file has been generated automatically from                      *
 *                                                                      *
 * src/core/effects/qgscoloreffect.h                                    *
 *                                                                      *
 * Do not edit manually ! Edit header and run scripts/sipify.pl again   *
 ************************************************************************/




class QgsColorEffect : QgsPaintEffect
{
%Docstring
 A paint effect which alters the colors (e.g., brightness, contrast) in a
 source picture.

.. versionadded:: 2.9
%End

%TypeHeaderCode
#include "qgscoloreffect.h"
%End
  public:

    static QgsPaintEffect *create( const QgsStringMap &map ) /Factory/;
%Docstring
 Creates a new QgsColorEffect effect from a properties string map.
 \param map encoded properties string map
 :return: new QgsColorEffect
 :rtype: QgsPaintEffect
%End

    QgsColorEffect();

    virtual QString type() const;
    virtual QgsStringMap properties() const;
    virtual void readProperties( const QgsStringMap &props );
    virtual QgsColorEffect *clone() const /Factory/;

    void setBrightness( int brightness );
%Docstring
 Sets the brightness modification for the effect.
 \param brightness Valid values are between -255 and 255, where 0 represents
 no change, negative values indicate darkening and positive values indicate
 lightening
.. seealso:: setBrightness
%End

    int brightness() const;
%Docstring
 Returns the brightness modification for the effect.
 :return: brightness value. Values are between -255 and 255, where 0 represents
 no change, negative values indicate darkening and positive values indicate
 lightening
.. seealso:: setBrightness
 :rtype: int
%End

    void setContrast( int contrast );
%Docstring
 Sets the contrast modification for the effect.
 \param contrast Valid values are between -100 and 100, where 0 represents
 no change, negative values indicate less contrast and positive values indicate
 greater contrast
.. seealso:: setContrast
%End

    int contrast() const;
%Docstring
 Returns the contrast modification for the effect.
 :return: contrast value. Values are between -100 and 100, where 0 represents
 no change, negative values indicate less contrast and positive values indicate
 greater contrast
.. seealso:: setContrast
 :rtype: int
%End

    void setSaturation( double saturation );
%Docstring
 Sets the saturation modification for the effect.
 \param saturation Valid values are between 0 and 2.0, where 1.0 represents
 no change, 0.0 represents totally desaturated (grayscale), and positive values indicate
 greater saturation
.. seealso:: saturation
%End

    double saturation() const;
%Docstring
 Returns the saturation modification for the effect.
 :return: saturation value. Values are between 0 and 2.0, where 1.0 represents
 no change, 0.0 represents totally desaturated (grayscale), and positive values indicate
 greater saturation
.. seealso:: setSaturation
 :rtype: float
%End

    void setGrayscaleMode( QgsImageOperation::GrayscaleMode grayscaleMode );
%Docstring
 Sets whether the effect should convert a picture to grayscale.
 \param grayscaleMode method for grayscale conversion
.. seealso:: grayscaleMode
%End

    QgsImageOperation::GrayscaleMode grayscaleMode() const;
%Docstring
 Returns whether the effect will convert a picture to grayscale.
 :return: method for grayscale conversion
.. seealso:: setGrayscaleMode
 :rtype: QgsImageOperation.GrayscaleMode
%End

    void setColorizeOn( bool colorizeOn );
%Docstring
 Sets whether the effect should colorize a picture.
 \param colorizeOn set to true to enable colorization
.. seealso:: colorizeOn
.. seealso:: setColorizeColor
.. seealso:: setColorizeStrength
%End

    bool colorizeOn() const;
%Docstring
 Returns whether the effect will colorize a picture.
 :return: true if colorization is enableds
.. seealso:: setColorizeOn
.. seealso:: colorizeColor
.. seealso:: colorizeStrength
 :rtype: bool
%End

    void setColorizeColor( const QColor &colorizeColor );
%Docstring
 Sets the color used for colorizing a picture. This is only used if
  setColorizeOn() is set to true.
 \param colorizeColor colorization color
.. seealso:: colorizeColor
.. seealso:: setColorizeOn
.. seealso:: setColorizeStrength
%End

    QColor colorizeColor() const;
%Docstring
 Returns the color used for colorizing a picture. This is only used if
 colorizeOn() is set to true.
 :return: colorization color
.. seealso:: setColorizeColor
.. seealso:: colorizeOn
.. seealso:: colorizeStrength
 :rtype: QColor
%End

    void setColorizeStrength( int colorizeStrength );
%Docstring
 Sets the strength for colorizing a picture. This is only used if
  setColorizeOn() is set to true.
 \param colorizeStrength colorization strength, between 0 and 100
.. seealso:: colorizeStrength
.. seealso:: setColorizeOn
.. seealso:: setColorizeColor
%End

    int colorizeStrength() const;
%Docstring
 Returns the strength used for colorizing a picture. This is only used if
 setColorizeOn() is set to true.
 :return: colorization strength, between 0 and 100
.. seealso:: setColorizeStrengths
.. seealso:: colorizeOn
.. seealso:: colorizeColor
 :rtype: int
%End

    void setTransparency( const double transparency );
%Docstring
 Sets the transparency for the effect
 \param transparency double between 0 and 1 inclusive, where 0 is fully opaque
 and 1 is fully transparent
.. seealso:: transparency
%End

    double transparency() const;
%Docstring
 Returns the transparency for the effect
 :return: transparency value between 0 and 1 inclusive, where 0 is fully opaque
 and 1 is fully transparent
.. seealso:: setTransparency
 :rtype: float
%End

    void setBlendMode( const QPainter::CompositionMode mode );
%Docstring
 Sets the blend mode for the effect
 \param mode blend mode used for drawing the effect on to a destination
 paint device
.. seealso:: blendMode
%End

    QPainter::CompositionMode blendMode() const;
%Docstring
 Returns the blend mode for the effect
 :return: blend mode used for drawing the effect on to a destination
 paint device
.. seealso:: setBlendMode
 :rtype: QPainter.CompositionMode
%End

  protected:

    virtual void draw( QgsRenderContext &context );

};


/************************************************************************
 * This file has been generated automatically from                      *
 *                                                                      *
 * src/core/effects/qgscoloreffect.h                                    *
 *                                                                      *
 * Do not edit manually ! Edit header and run scripts/sipify.pl again   *
 ************************************************************************/
