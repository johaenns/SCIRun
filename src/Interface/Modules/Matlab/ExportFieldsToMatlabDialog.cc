/*
   For more information, please see: http://software.sci.utah.edu

   The MIT License

   Copyright (c) 2015 Scientific Computing and Imaging Institute,
   University of Utah.

   License for the specific language governing rights and limitations under
   Permission is hereby granted, free of charge, to any person obtaining a
   copy of this software and associated documentation files (the "Software"),
   to deal in the Software without restriction, including without limitation
   the rights to use, copy, modify, merge, publish, distribute, sublicense,
   and/or sell copies of the Software, and to permit persons to whom the
   Software is furnished to do so, subject to the following conditions:

   The above copyright notice and this permission notice shall be included
   in all copies or substantial portions of the Software.

   THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS
   OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
   FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL
   THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
   LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
   FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
   DEALINGS IN THE SOFTWARE.
*/

#include <Interface/Modules/Matlab/ExportFieldsToMatlabDialog.h>
#include <Core/Algorithms/Base/AlgorithmVariableNames.h>
#include <Modules/Legacy/Matlab/DataIO/ExportFieldsToMatlab.h>
#include <QFileDialog>
#include <boost/regex.hpp>
#include <boost/lexical_cast.hpp>

using namespace SCIRun::Gui;
using namespace SCIRun::Dataflow::Networks;
using namespace SCIRun::Core::Algorithms;
using namespace SCIRun::Core::Algorithms::Matlab;

ExportFieldsToMatlabDialog::ExportFieldsToMatlabDialog(const std::string& name, ModuleStateHandle state,
  QWidget* parent /* = 0 */)
  : ModuleDialogGeneric(state, parent)
{
  setupUi(this);
  setWindowTitle(QString::fromStdString(name));
  fixSize();

  WidgetStyleMixin::tableHeaderStyle(tableWidget);
  addLineEditManager(fileNameLineEdit_, Variables::Filename);
  connect(openFileButton_, SIGNAL(clicked()), this, SLOT(saveFile()));
  connect(fileNameLineEdit_, SIGNAL(editingFinished()), this, SLOT(pushFileNameToState()));
  connect(fileNameLineEdit_, SIGNAL(returnPressed()), this, SLOT(pushFileNameToState()));
}

void ExportFieldsToMatlabDialog::updateFromPortChange(int, const std::string& portName, DynamicPortChange type)
{
  if (type == DynamicPortChange::INITIAL_PORT_CONSTRUCTION)
    return;

  static const std::string typeName = "Field";
  const int lineEditColumn = 1;
  syncTableRowsWithDynamicPort(portName, typeName, tableWidget, lineEditColumn, type, TableItemMakerMap(), 
  {
    { 2, [this]() { return makeInputArrayTypeComboBoxItem(); } }
  });
}

QComboBox* ExportFieldsToMatlabDialog::makeInputArrayTypeComboBoxItem() const
{
  QStringList bcList;
  bcList << "numeric array" << "struct array";
  auto bcBox = new QComboBox();
  bcBox->addItems(bcList);
  bcBox->setCurrentIndex(bcBox->findText(QString::fromStdString(toStringVector(state_->getValue(Parameters::FieldFormats).toVector())[tableWidget->rowCount() - 1])));
  connect(bcBox, SIGNAL(currentIndexChanged(int)), this, SLOT(pushArrayType()));
  return bcBox;
}

void ExportFieldsToMatlabDialog::pushArrayType()
{
  auto types = makeHomogeneousVariableList([this](size_t i) { return qobject_cast<QComboBox*>(tableWidget->cellWidget(i, 2))->currentText().toStdString(); }, tableWidget->rowCount());
  state_->setValue(Parameters::FieldFormats, types);
}

void ExportFieldsToMatlabDialog::saveFile()
{
  auto file = QFileDialog::getSaveFileName(this, "Save Matlab File", dialogDirectory(), "*.mat");
  if (file.length() > 0)
  {
    fileNameLineEdit_->setText(file);
    updateRecentFile(file);
    pushFileNameToState();
  }
}

void ExportFieldsToMatlabDialog::pushFileNameToState()
{
  state_->setValue(Variables::Filename, fileNameLineEdit_->text().trimmed().toStdString());
}
