#ifndef iipr_ICircleFinderParams_included
#define iipr_ICircleFinderParams_included


#include "iser/ISerializable.h"

#include "iipr/iipr.h"


namespace iipr
{


/**
	Interface for the circle finder parameter.
*/
class ICircleFinderParams: virtual public iser::ISerializable
{
public:
	
	/**
		Caliper mode. Control, which caliper is used for the circle calculation.
	*/
	enum CaliperMode
	{
		/**
			The first found caliper is taked for circle calculation.
		*/
		CCM_FIRST,

		/**
			The best found caliper is taked for circle calculation.
		*/	
		CCM_BEST
	};


	/**
		Get caliper mode.
		\sa CaliperMode
	*/
	virtual int GetCaliperMode() const = 0;
	
	/**
		Set caliper mode.
		\sa CaliperMode
	*/
	virtual void SetCaliperMode(int caliperMode) = 0;

	/**
		Return \c true, if the outlier elimination is activated.
	*/
	virtual bool IsOutlierEliminationEnabled() const = 0;

	/**
		Enable the outlier elimination.
	*/
	virtual void SetOutlierEliminationEnabled(bool isOutlierEliminationEnabled = true) = 0;

	/**
		Get the minimal outlier distance.
	*/
	virtual double GetMinOutlierDistance() const = 0;

	/**
		Set minimal outlier distance
	*/
	virtual void SetMinOutlierDistance(double minOutlierDistance) = 0;

	/**
		Get the number of projection rays using for the find the circle points.
	*/
	virtual int GetRaysCount() const = 0;

	/**
		Set the number of projection rays using for the find the circle points.
	*/
	virtual void SetRaysCount(int raysCount) = 0;
};


} // namespace iipr


#endif // !iipr_ICircleFinderParams_included


