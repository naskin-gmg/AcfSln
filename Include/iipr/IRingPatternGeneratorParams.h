#ifndef iipr_IRingPatternGeneratorParams_included
#define iipr_IRingPatternGeneratorParams_included


// ACF includes
#include "iser/ISerializable.h"


namespace iipr
{


/**
	Common interface for parameters of the ring pattern generators.
*/
class IRingPatternGeneratorParams: virtual public iser::ISerializable
{
public:
	enum PatternType
	{
		/**
			Ring pattern is generated using a fixed wave length.
		*/
		PT_FIXED_WAVELENGTH,

		/**
			Ring pattern is generated using discending wave length.
		*/
		PT_DYNAMIC_WAVELENGTH
	};

	/**
		Get used pattern generation mode.
		\sa PatternType
	*/
	virtual int GetPatternMode() const = 0;
	
	/**
		Set pattern generation mode.
		\sa PatternType
	*/
	virtual void SetPatternMode(int patternMode) = 0;
	
	/**
		Get used pattern wavelength.
	*/
	virtual double GetWaveLength() const = 0;

	/**
		Set pattern wavelength.
	*/
	virtual void SetWaveLength(double waveLength) = 0;
};


} // namespace iipr


#endif // !iipr_IRingPatternGeneratorParams_included


