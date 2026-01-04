#include "MapDefinitionDialog.h"
#include <QFormLayout>
#include <QDialogButtonBox>
#include <QMessageBox>
#include <QHBoxLayout>
#include <sstream>
#include <iomanip>

namespace WinMMM10 {

MapDefinitionDialog::MapDefinitionDialog(QWidget* parent)
    : QDialog(parent)
{
    setWindowTitle("Map Definition");
    setModal(true);
    
    auto* layout = new QFormLayout(this);
    
    m_nameEdit = new QLineEdit();
    layout->addRow("Name:", m_nameEdit);
    
    m_addressEdit = new QLineEdit();
    m_addressEdit->setPlaceholderText("0x00000000");
    layout->addRow("Address:", m_addressEdit);
    
    m_typeCombo = new QComboBox();
    m_typeCombo->addItem("2D Map");
    m_typeCombo->addItem("3D Map");
    connect(m_typeCombo, QOverload<int>::of(&QComboBox::currentIndexChanged), 
            this, &MapDefinitionDialog::onMapTypeChanged);
    layout->addRow("Type:", m_typeCombo);
    
    m_rowsSpin = new QSpinBox();
    m_rowsSpin->setMinimum(1);
    m_rowsSpin->setMaximum(1000);
    m_rowsSpin->setValue(1);
    layout->addRow("Rows:", m_rowsSpin);
    
    m_columnsSpin = new QSpinBox();
    m_columnsSpin->setMinimum(1);
    m_columnsSpin->setMaximum(1000);
    m_columnsSpin->setValue(16);
    layout->addRow("Columns:", m_columnsSpin);
    
    m_dataTypeCombo = new QComboBox();
    m_dataTypeCombo->addItem("uint8", 1);
    m_dataTypeCombo->addItem("uint16", 2);
    m_dataTypeCombo->addItem("int16", 3);
    m_dataTypeCombo->addItem("float", 4);
    m_dataTypeCombo->setCurrentIndex(1);
    layout->addRow("Data Type:", m_dataTypeCombo);
    
    m_factorSpin = new QDoubleSpinBox();
    m_factorSpin->setDecimals(6);
    m_factorSpin->setMinimum(0.000001);
    m_factorSpin->setMaximum(1000000.0);
    m_factorSpin->setValue(1.0);
    layout->addRow("Factor:", m_factorSpin);
    
    m_offsetSpin = new QDoubleSpinBox();
    m_offsetSpin->setDecimals(6);
    m_offsetSpin->setMinimum(-1000000.0);
    m_offsetSpin->setMaximum(1000000.0);
    m_offsetSpin->setValue(0.0);
    layout->addRow("Offset:", m_offsetSpin);
    
    m_unitEdit = new QLineEdit();
    layout->addRow("Unit:", m_unitEdit);
    
    auto* buttonBox = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel);
    connect(buttonBox, &QDialogButtonBox::accepted, this, &MapDefinitionDialog::accept);
    connect(buttonBox, &QDialogButtonBox::rejected, this, &QDialog::reject);
    layout->addRow(buttonBox);
    
    onMapTypeChanged(0);
}

void MapDefinitionDialog::onMapTypeChanged(int index) {
    bool is3D = (index == 1);
    m_rowsSpin->setEnabled(is3D);
    m_rowsSpin->setValue(is3D ? 16 : 1);
}

void MapDefinitionDialog::setMapDefinition(const MapDefinition& map) {
    m_nameEdit->setText(QString::fromStdString(map.name()));
    
    std::ostringstream oss;
    oss << "0x" << std::hex << std::uppercase << std::setfill('0') << std::setw(8) << map.address();
    m_addressEdit->setText(QString::fromStdString(oss.str()));
    
    m_typeCombo->setCurrentIndex(map.type() == MapType::Map3D ? 1 : 0);
    m_rowsSpin->setValue(static_cast<int>(map.rows()));
    m_columnsSpin->setValue(static_cast<int>(map.columns()));
    
    for (int i = 0; i < m_dataTypeCombo->count(); ++i) {
        if (m_dataTypeCombo->itemData(i).toInt() == map.dataType()) {
            m_dataTypeCombo->setCurrentIndex(i);
            break;
        }
    }
    
    m_factorSpin->setValue(map.factor());
    m_offsetSpin->setValue(map.offset());
    m_unitEdit->setText(QString::fromStdString(map.unit()));
}

MapDefinition MapDefinitionDialog::getMapDefinition() const {
    MapDefinition map;
    
    map.setName(m_nameEdit->text().toStdString());
    
    QString addrText = m_addressEdit->text();
    bool ok;
    size_t address = 0;
    if (addrText.startsWith("0x") || addrText.startsWith("0X")) {
        address = addrText.toULongLong(&ok, 16);
    } else {
        address = addrText.toULongLong(&ok, 16);
        if (!ok) {
            address = addrText.toULongLong(&ok, 10);
        }
    }
    if (ok) {
        map.setAddress(address);
    }
    
    map.setType(m_typeCombo->currentIndex() == 1 ? MapType::Map3D : MapType::Map2D);
    map.setRows(static_cast<size_t>(m_rowsSpin->value()));
    map.setColumns(static_cast<size_t>(m_columnsSpin->value()));
    map.setDataType(static_cast<uint16_t>(m_dataTypeCombo->currentData().toInt()));
    map.setFactor(m_factorSpin->value());
    map.setOffset(m_offsetSpin->value());
    map.setUnit(m_unitEdit->text().toStdString());
    
    return map;
}

MapDefinition MapDefinitionDialog::getMapDefinition(QWidget* parent, const MapDefinition* existing) {
    MapDefinitionDialog dialog(parent);
    if (existing) {
        dialog.setMapDefinition(*existing);
    }
    
    if (dialog.exec() == QDialog::Accepted) {
        return dialog.getMapDefinition();
    }
    
    return MapDefinition();
}

void MapDefinitionDialog::accept() {
    // Validate
    if (m_nameEdit->text().isEmpty()) {
        QMessageBox::warning(this, "Validation", "Please enter a map name.");
        return;
    }
    
    QString addrText = m_addressEdit->text();
    bool ok;
    if (addrText.startsWith("0x") || addrText.startsWith("0X")) {
        addrText.toULongLong(&ok, 16);
    } else {
        addrText.toULongLong(&ok, 16);
        if (!ok) {
            addrText.toULongLong(&ok, 10);
        }
    }
    if (!ok) {
        QMessageBox::warning(this, "Validation", "Please enter a valid address.");
        return;
    }
    
    QDialog::accept();
}

} // namespace WinMMM10

