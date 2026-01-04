#pragma once

#include <QDialog>
#include <QLineEdit>
#include <QSpinBox>
#include <QDoubleSpinBox>
#include <QComboBox>
#include <QPushButton>
#include "../maps/MapDefinition.h"

namespace WinMMM10 {

class MapDefinitionDialog : public QDialog {
    Q_OBJECT

public:
    explicit MapDefinitionDialog(QWidget* parent = nullptr);
    ~MapDefinitionDialog() override = default;
    
    void setMapDefinition(const MapDefinition& map);
    MapDefinition getMapDefinition() const;
    
    static MapDefinition getMapDefinition(QWidget* parent, const MapDefinition* existing = nullptr);

private slots:
    void onMapTypeChanged(int index);
    void accept() override;

private:
    QLineEdit* m_nameEdit{nullptr};
    QLineEdit* m_addressEdit{nullptr};
    QComboBox* m_typeCombo{nullptr};
    QSpinBox* m_rowsSpin{nullptr};
    QSpinBox* m_columnsSpin{nullptr};
    QComboBox* m_dataTypeCombo{nullptr};
    QDoubleSpinBox* m_factorSpin{nullptr};
    QDoubleSpinBox* m_offsetSpin{nullptr};
    QLineEdit* m_unitEdit{nullptr};
};

} // namespace WinMMM10

