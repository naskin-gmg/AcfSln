#ifndef ifpf_CHotfolderVisualisation_included
#define ifpf_CHotfolderVisualisation_included


// ACF includes
#include "i2d/IObject2d.h"

#include "iprm/IParamsSet.h"


// AcfSln includes
#include "ifpf/IHotfolderProcessingInfo.h"


namespace ifpf
{


/**
	Interface for a hotfolder's state model.
*/
class CHotfolderVisualisation: virtual public i2d::IObject2d
{
public:
	CHotfolderVisualisation(iprm::IParamsSet* hotfolderParams, ifpf::IHotfolderProcessingInfo* hotfolderStateModelPtr);

	// reimplemented (IObject2d)
	virtual i2d::CVector2d GetCenter() const;
	virtual void MoveTo(const i2d::CVector2d& position);
};


} // namespace ifpf


#endif // !ifpf_CHotfolderVisualisation_included


