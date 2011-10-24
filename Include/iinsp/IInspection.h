#ifndef iinsp_IInspection_included
#define iinsp_IInspection_included


// ACF includes
#include "iser/ISerializable.h"
#include "iprm/IParamsSet.h"


namespace iinsp
{


class IInspectionTask;


/**
	Representation of inspection (set of inspection tasks).
*/
class IInspection: virtual public iser::ISerializable
{
public:
	/**
		Get number of inspection tasks.
	*/
	virtual int GetTasksCount() const = 0;

	/**
		Get single inspection task.
	*/
	virtual IInspectionTask* GetTask(int subtaskIndex) const = 0;

	/**
		Get access to global inspection parameters.
		\return	parameter set describing global parameters or \c NULL if no global parameters are supported.
	*/
	virtual iprm::IParamsSet* GetInspectionParams() const = 0;
};


} // namespace iinsp


#endif // !iinsp_IInspection_included


