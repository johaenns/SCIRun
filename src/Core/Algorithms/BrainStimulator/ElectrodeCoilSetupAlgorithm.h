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

#ifndef ALGORITHMS_MATH_ElectrodeCoilSetupAlgorithm_H
#define ALGORITHMS_MATH_ElectrodeCoilSetupAlgorithm_H

#include <Core/Algorithms/Base/AlgorithmBase.h>
#include <Core/Algorithms/BrainStimulator/share.h>

//////////////////////////////////////////////////////////////////////////
/// @todo MORITZ
//////////////////////////////////////////////////////////////////////////

namespace SCIRun {
namespace Core {
namespace Algorithms {
namespace BrainStimulator {
  
  ALGORITHM_PARAMETER_DECL(TableValues);
  ALGORITHM_PARAMETER_DECL(ProtoTypeInputCheckbox);
  ALGORITHM_PARAMETER_DECL(AllInputsTDCS);
  ALGORITHM_PARAMETER_DECL(ProtoTypeInputComboBox);
  ALGORITHM_PARAMETER_DECL(NumberOfPrototypes);
  ALGORITHM_PARAMETER_DECL(ElectrodethicknessCheckBox);
  ALGORITHM_PARAMETER_DECL(ElectrodethicknessSpinBox);
  
  class SCISHARE ElectrodeCoilSetupAlgorithm : public AlgorithmBase
  {
  public:
    ElectrodeCoilSetupAlgorithm();
    AlgorithmOutput run_generic(const AlgorithmInput& input) const;
    static const AlgorithmOutputName ELECTRODES_FIELD;
    static const AlgorithmInputName SCALP_SURF; 
    static const AlgorithmInputName LOCATIONS;
    static const AlgorithmInputName ELECTRODECOILPROTOTYPES;    
    static const AlgorithmOutputName ELECTRODE_SPONGE_LOCATION_AVR;
    static const AlgorithmOutputName COILS_FIELD;
      
    boost::tuple<VariableHandle, Datatypes::DenseMatrixHandle, FieldHandle, FieldHandle> run(const FieldHandle scalp, const Datatypes::DenseMatrixHandle locations, const std::vector<FieldHandle>& elc_coil_proto) const;
    static const int number_of_columns = 10; /// number of GUI columns

    static const AlgorithmParameterName columnNames[number_of_columns];

  private:
    static const int unknown_stim_type; /// first Stimulation type
    static const int tDCS_stim_type; /// second  ... 
    static const int TMS_stim_type; /// third  ... 
    
    Datatypes::DenseMatrixHandle make_rotation_matrix_around_axis(double angle, std::vector<double>& axis_vector) const;
    Datatypes::DenseMatrixHandle make_rotation_matrix(const double angle, const std::vector<double>& normal) const;
    boost::tuple<Datatypes::DenseMatrixHandle, FieldHandle> make_tdcs_electrodes(FieldHandle scalp, const std::vector<FieldHandle>& elc_coil_proto, const std::vector<double>& elc_prototyp_map, const std::vector<double>& elc_x, const std::vector<double>& elc_y, const std::vector<double>& elc_z, const std::vector<double>& elc_angle_rotation, const std::vector<double>& elc_thickness) const;            
    FieldHandle make_tms(FieldHandle scalp, const std::vector<FieldHandle>& elc_coil_proto, const std::vector<double>& coil_prototyp_map, const std::vector<double>& coil_x, const std::vector<double>& coil_y, const std::vector<double>& coil_z, const std::vector<double>& coil_angle_rotation, const std::vector<double>& coil_nx, const std::vector<double>& coil_ny, const std::vector<double>& coil_nz) const;        
    VariableHandle fill_table(FieldHandle scalp, Datatypes::DenseMatrixHandle locations, const std::vector<FieldHandle>& input) const;
  };

}}}}

#endif
