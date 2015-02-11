/*
   For more information, please see: http://software.sci.utah.edu

   The MIT License

   Copyright (c) 2012 Scientific Computing and Imaging Institute,
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

#include <Interface/Modules/Visualization/CreateBasicColorMapDialog.h>
#include <Core/Algorithms/Base/AlgorithmVariableNames.h>
#include <Modules/Visualization/CreateBasicColorMap.h>

using namespace SCIRun::Gui;
using namespace SCIRun::Dataflow::Networks;
using namespace SCIRun::Core::Algorithms;
using namespace SCIRun::Core::Datatypes;



CreateBasicColorMapDialog::CreateBasicColorMapDialog(const std::string& name, ModuleStateHandle state,
  QWidget* parent /* = 0 */)
  : ModuleDialogGeneric(state, parent)
{
  setupUi(this);
  setWindowTitle(QString::fromStdString(name));
  ColorMap cm("Rainbow");
  previewColorMap_->setStyleSheet(buildGradientString(cm));
  
  typedef SCIRun::Modules::Visualization::CreateBasicColorMap CreateBasicColorMapModule;
  resolutionSpin_->setValue(256);
  shiftSpin_->setValue(0.0);
  invertCheck_->setChecked(false);
  
  addComboBoxManager(colorMapNameComboBox_, CreateBasicColorMapModule::ColorMapName);
  addSpinBoxManager(resolutionSpin_, CreateBasicColorMapModule::ColorMapResolution);
  addDoubleSpinBoxManager(shiftSpin_, CreateBasicColorMapModule::ColorMapShift);
  addCheckBoxManager(invertCheck_, CreateBasicColorMapModule::ColorMapInvert);
  
  connect(colorMapNameComboBox_,SIGNAL(currentIndexChanged(QString)),this,SLOT(updateColorMapPreview(QString)));
  connect(shiftSpin_,SIGNAL(valueChanged(double)),this,SLOT(setShiftSlider(double)));
  connect(resolutionSpin_,SIGNAL(valueChanged(int)),this,SLOT(setResolutionSlider(int)));
  connect(shiftSlider_,SIGNAL(valueChanged(int)),this,SLOT(setShiftText(int)));
  connect(resolutionSlider_,SIGNAL(valueChanged(int)),this,SLOT(setResolutionText(int)));
  connect(invertCheck_,SIGNAL(toggled(bool)),this,SLOT(onInvertCheck(bool)));
}

void CreateBasicColorMapDialog::pull()
{
  pull_newVersionToReplaceOld();
}

void CreateBasicColorMapDialog::updateColorMapPreview(QString s) {
    ColorMap cm(s.toStdString(),resolutionSlider_->value(),
                static_cast<double>(shiftSlider_->value()) / 100.,
                invertCheck_->isChecked());
    previewColorMap_->setStyleSheet(buildGradientString(cm));
}


const QString CreateBasicColorMapDialog::buildGradientString(ColorMap cm) {
    std::stringstream ss;
    ss << "background-color: qlineargradient(spread:pad, x1:0, y1:0, x2:1, y2:0,";
    for (double i = 0.001; i < 1.0; i +=0.001) {
        ss << " stop:" << i;
        ss << " rgba(";
        ColorRGB c = cm.getColorMapVal(i);
        ss << int(255.*c.r()) << ", " << int(255.*c.g()) << ", " << int(255.*c.b()) << ", 255),";
    }
    ss << ");";
    std::string str = ss.str();
    return QString::fromStdString(ss.str());
}


void CreateBasicColorMapDialog::setShiftSlider(double d) {
    shiftSlider_->setValue(static_cast<int>(d * 100.));
    updateColorMapPreview(colorMapNameComboBox_->currentText());
}

void CreateBasicColorMapDialog::setResolutionSlider(int i) {
    resolutionSlider_->setValue(i);
    updateColorMapPreview(colorMapNameComboBox_->currentText());
}

void CreateBasicColorMapDialog::setShiftText(int i) {
    shiftSpin_->setValue(static_cast<double>(i) / 100.);
    updateColorMapPreview(colorMapNameComboBox_->currentText());
}

void CreateBasicColorMapDialog::setResolutionText(int i) {
    resolutionSpin_->setValue(i);
    updateColorMapPreview(colorMapNameComboBox_->currentText());
}

void CreateBasicColorMapDialog::onInvertCheck(bool b) {
    updateColorMapPreview(colorMapNameComboBox_->currentText());
}





