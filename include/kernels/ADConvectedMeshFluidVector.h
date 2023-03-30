//* Licensing

#pragma once

#include "ADKernelValue.h"


class ADConvectedMeshFluidVector : public ADVectorKernelValue
{
public:
  static InputParameters validParams();

  ADConvectedMeshFluidVector(const InputParameters & parameters);

protected:
  virtual ADRealVectorValue precomputeQpResidual() override;

  const VariableValue & _disp_x_dot;
  const VariableValue & _d_disp_x_dot;
  const unsigned int _disp_x_id;
  const VariableValue & _disp_y_dot;
  const VariableValue & _d_disp_y_dot;
  const unsigned int _disp_y_id;
  const VariableValue & _disp_z_dot;
  const VariableValue & _d_disp_z_dot;
  const unsigned int _disp_z_id;

  const ADMaterialProperty<Real> & _rho;
};
