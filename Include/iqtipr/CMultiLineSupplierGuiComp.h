#ifndef iqtipr_CMultiLineSupplierGuiComp_included
#define iqtipr_CMultiLineSupplierGuiComp_included


// ACF includes
#include <i2d/CPolyline.h>
#include <iprm/ISelectionParam.h>
#include <imod/CMultiModelDispatcherBase.h>

// ACF-Solutions includes
#include <iqtinsp/TSupplierGuiCompBase.h>

#include <GeneratedFiles/iqtipr/ui_CMultiLineSupplierGuiComp.h>


namespace iqtipr
{


class CMultiLineSupplierGuiComp: public iqtinsp::TSupplierGuiCompBase<Ui::CMultiLineSupplierGuiComp>
{
	Q_OBJECT

public:
	typedef iqtinsp::TSupplierGuiCompBase<Ui::CMultiLineSupplierGuiComp> BaseClass;

	I_BEGIN_COMPONENT(CMultiLineSupplierGuiComp);
		I_REGISTER_SUBELEMENT(LineSelection);
		I_REGISTER_SUBELEMENT_INTERFACE(LineSelection, iprm::ISelectionParam, ExtractLineSelection);
	I_END_COMPONENT;

	CMultiLineSupplierGuiComp();

protected Q_SLOTS:
	void on_TestButton_clicked();

protected:
	// reimplemented (iqtgui::TGuiObserverWrap)
	virtual void UpdateGui(const istd::IChangeable::ChangeSet& changeSet) override;

	// reimplemented (iqtinsp::TSupplierGuiCompBase)
	virtual void OnSupplierParamsChanged() override;
	virtual QWidget* GetParamsWidget() const override;

	// reimplemented (iqt2d::TViewExtenderCompBase)
	virtual void CreateShapes(int sceneId, Shapes& result) override;

	// reimplemented (iqtgui::CGuiComponentBase)
	virtual void OnGuiHidden() override;

protected:
	class LineSelection: public iprm::ISelectionParam
	{
	public:
		LineSelection();

		// reimplemented (iprm::ISelectionParam)
		virtual const iprm::IOptionsList* GetSelectionConstraints() const override;
		virtual int GetSelectedOptionIndex() const override;
		virtual bool SetSelectedOptionIndex(int index) override;
		virtual iprm::ISelectionParam* GetSubselection(int index) const override;

		// reimplemented (iser::ISerializable)
		virtual bool Serialize(iser::IArchive& archive) override;

	protected:
		int m_selectedLineIndex;
	};

	class CShape:
				public iview::CShapeBase,
				protected imod::CMultiModelDispatcherBase
	{
	public:
		typedef iview::CShapeBase BaseClass;
		typedef imod::CMultiModelDispatcherBase BaseClass2;

		CShape(iprm::ISelectionParam& selection);
		virtual ~CShape();

		// reimplemented (iview::IVisualizable)
		virtual void Draw(QPainter& drawContext) const override;

		// reimplemented (imod::CMultiModelDispatcherBase)
		virtual void OnModelChanged(int modelId, const istd::IChangeable::ChangeSet& changeSet) override;

	protected:
		// reimplemented (iview::CShapeBase)
		virtual i2d::CRect CalcBoundingBox() const override;

	private:
		iprm::ISelectionParam& m_lineSelection;
	};

	imod::TModelWrap<LineSelection> m_lineSelection;

	static iprm::ISelectionParam* ExtractLineSelection(CMultiLineSupplierGuiComp& component)
	{
		return &component.m_lineSelection;
	}
};


} // namespace iqtipr


#endif // !iqtipr_CMultiLineSupplierGuiComp_included


