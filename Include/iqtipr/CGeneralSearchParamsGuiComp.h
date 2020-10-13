#ifndef iqtipr_CGeneralSearchParamsGuiComp_included
#define iqtipr_CGeneralSearchParamsGuiComp_included


// ACF includes
#include <iqtgui/TDesignerGuiObserverCompBase.h>

// ACF-Solutions
#include <iipr/ISearchParams.h>

// Generated
#include <GeneratedFiles/iqtipr/ui_CGeneralSearchParamsGuiComp.h>


namespace iqtipr
{


class CGeneralSearchParamsGuiComp:
			public iqtgui::TDesignerGuiObserverCompBase<
						Ui::CGeneralSearchParamsGuiComp,
						iipr::ISearchParams>
{
	Q_OBJECT
public:
	typedef iqtgui::TDesignerGuiObserverCompBase<
				Ui::CGeneralSearchParamsGuiComp,
				iipr::ISearchParams> BaseClass;

	I_BEGIN_COMPONENT(CGeneralSearchParamsGuiComp);
	I_END_COMPONENT;

protected:
	// reimplemented (iqtgui::TGuiObserverWrap)
	virtual void OnGuiModelAttached();
	virtual void UpdateModel() const;
	virtual void UpdateGui(const istd::IChangeable::ChangeSet& changeSet);

	// reimplemented (iqtgui::CGuiComponentBase)
	virtual void OnGuiRetranslate();

protected Q_SLOTS:
	void OnParamsChanged();
	void on_RotationCB_toggled(bool);
	void on_ScalingCB_toggled(bool);
};


} // namespace iqtipr


#endif // !iqtipr_CGeneralSearchParamsGuiComp_included
