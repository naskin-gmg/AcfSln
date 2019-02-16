#ifndef icam_CMultiSourceSnapBitmapSupplierComp_included
#define icam_CMultiSourceSnapBitmapSupplierComp_included


// ACF includes
#include <iprm/IOptionsList.h>

// ACF-Solutions includes
#include <icam/IBitmapAcquisition.h>
#include <icam/CSnapBitmapSupplierCompBase.h>


namespace icam
{


/**
	This component allows image acquisition based on a camera selection.
	Multiple independent camera implementations can be registered and switched at runtime.
	Each camera can store its parameters in the Parameter Manager accessed from the supplier's parameter set via 'CameraManagerParamId' attribute.
*/
class CMultiSourceSnapBitmapSupplierComp: public CSnapBitmapSupplierCompBase
{
public:
	typedef CSnapBitmapSupplierCompBase BaseClass;

	I_BEGIN_COMPONENT(CMultiSourceSnapBitmapSupplierComp);
		I_REGISTER_SUBELEMENT(CameraList);
		I_REGISTER_SUBELEMENT_INTERFACE(CameraList, iprm::IOptionsList, ExtractCameraList);
		I_REGISTER_SUBELEMENT_INTERFACE(CameraList, istd::IChangeable, ExtractCameraList);
		I_REGISTER_SUBELEMENT_INTERFACE(CameraList, imod::IModel, ExtractCameraList);
		I_ASSIGN(m_bitmapCompFact, "BitmapFactory", "Use to create bitmap object", true, "BitmapFactory");
		I_ASSIGN_MULTI_0(m_cameraListCompPtr, "Cameras", "List of cameras used for snap", true);
		I_ASSIGN_MULTI_0(m_cameraIdsAttrPtr, "CameraIds", "List of camera IDs", true);
		I_ASSIGN_MULTI_0(m_cameraNamesAttrPtr, "CameraNames", "List of camera names", true);
		I_ASSIGN(m_sourceSelectionParamIdAttrPtr, "CameraSelectorId", "ID of the source camera selector parameter in the supplier's parameter set", true, "CameraSelectorId");
		I_ASSIGN(m_cameraManagerParamIdAttrPtr, "CameraManagerParamId", "ID of the camera parameter manager in the supplier's parameter set", true, "CameraManagerParamId");
	I_END_COMPONENT;

	CMultiSourceSnapBitmapSupplierComp();

protected:
	// reimplemented (CSnapBitmapSupplierCompBase)
	virtual iimg::IBitmap* CreateBitmap() const;
	virtual int DoSnap(const iprm::IParamsSet* snapParamsPtr, iimg::IBitmap& snapBitmap) const;

	// reimplemented (iinsp::TSupplierCompWrap)
	virtual bool InitializeWork();

private:
	class CameraList: virtual public iprm::IOptionsList
	{
	public:
		CameraList();

		void SetParent(CMultiSourceSnapBitmapSupplierComp* parentPtr);

		// reimplemented (iprm::IOptionsList)
		virtual int GetOptionsFlags() const;
		virtual int GetOptionsCount() const;
		virtual QString GetOptionName(int index) const;
		virtual QString GetOptionDescription(int index) const;
		virtual QByteArray GetOptionId(int index) const;
		virtual bool IsOptionEnabled(int index) const;
		
	private:
		CMultiSourceSnapBitmapSupplierComp* m_parentPtr;
	};

	template <class InterfaceType>
	static InterfaceType* ExtractCameraList(CMultiSourceSnapBitmapSupplierComp& component)
	{
		return &component.m_cameraList;
	}

	IBitmapAcquisition* GetSelectedCamera(int* slectedIndexPtr = NULL) const;

private:
	I_FACT(iimg::IBitmap, m_bitmapCompFact);
	I_MULTIREF(IBitmapAcquisition, m_cameraListCompPtr);
	I_MULTIATTR(QByteArray, m_cameraIdsAttrPtr);
	I_MULTITEXTATTR(m_cameraNamesAttrPtr);
	I_ATTR(QByteArray, m_sourceSelectionParamIdAttrPtr);
	I_ATTR(QByteArray, m_cameraManagerParamIdAttrPtr);

	imod::TModelWrap<CameraList> m_cameraList;
};


} // namespace icam


#endif // !icam_CMultiSourceSnapBitmapSupplierComp_included


