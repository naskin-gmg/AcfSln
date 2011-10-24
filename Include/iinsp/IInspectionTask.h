#ifndef iinsp_IInspectionTask_included
#define iinsp_IInspectionTask_included


// ACF includes
#include "iser/ISerializable.h"
#include "iprm/IParamsSet.h"
#include "iproc/ISupplier.h"


namespace iinsp
{


/**
	Representation of complete inspection task.
	Inspection contains of set of subtask suppliers (steps).
*/
class IInspectionTask: virtual public iser::ISerializable
{
public:
	/**
		Get number of inspection steps (subtasks).
	*/
	virtual int GetSubtasksCount() const = 0;
	/**
		Get single inspection steps (subtasks).
	*/
	virtual iproc::ISupplier* GetSubtask(int subtaskIndex) const = 0;
	/**
		Get access to global inspection parameters.
		\return	parameter set describing global parameters or \c NULL if no global parameters are supported.
	*/
	virtual iprm::IParamsSet* GetTaskParams() const = 0;
};


} // namespace iinsp


#endif // !iinsp_IInspectionTask_included


