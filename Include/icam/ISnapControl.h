#ifndef icam_ISnapControl_included
#define icam_ISnapControl_included


// ACF includes
#include "istd/IPolymorphic.h"


namespace icam
{


class ISnapControl: public istd::IPolymorphic
{
public:
	enum SnapDirection
	{
		SD_FORWARD,
		SD_BACK
	};

	virtual void SetSnapDirection(int snapDirection) = 0;
	virtual int GetSnapDirection() const = 0;
};


} // namespace icam


#endif // !icam_ISnapControl_included
