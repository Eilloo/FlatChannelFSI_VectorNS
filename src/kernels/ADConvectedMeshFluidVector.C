//* Licensing

#include "ADConvectedMeshFluidVector.h"
#include <iostream>

registerMooseObject("FsiApp", ADConvectedMeshFluidVector);

InputParameters
ADConvectedMeshFluidVector::validParams()
{
  InputParameters params = ADVectorKernelValue::validParams();
  params.addClassDescription(
      "Corrects the convective derivative for situations in which the fluid mesh is dynamic.");
  params.addRequiredCoupledVar("disp_x", "The x displacement");
  params.addCoupledVar("disp_y", "The y displacement");
  params.addCoupledVar("disp_z", "The z displacement");
  params.addParam<MaterialPropertyName>("rho_name", "rho", "The name of the density");
  return params;
}

ADConvectedMeshFluidVector::ADConvectedMeshFluidVector(const InputParameters & parameters)
  : ADVectorKernelValue(parameters),
    _disp_x_dot(coupledDot("disp_x")),
    _d_disp_x_dot(coupledDotDu("disp_x")),
    _disp_x_id(coupled("disp_x")),
    _disp_y_dot(isCoupled("disp_y") ? coupledDot("disp_y") : _zero),
    _d_disp_y_dot(isCoupled("disp_y") ? coupledDotDu("disp_y") : _zero),
    _disp_y_id(coupled("disp_y")),
    _disp_z_dot(isCoupled("disp_z") ? coupledDot("disp_z") : _zero),
    _d_disp_z_dot(isCoupled("disp_z") ? coupledDotDu("disp_z") : _zero),
    _disp_z_id(coupled("disp_z")),
    _rho(getADMaterialProperty<Real>("rho_name"))
{
}

ADRealVectorValue
ADConvectedMeshFluidVector::precomputeQpResidual()
{
  ADReal x_comp = ADRealVectorValue(_disp_x_dot[_qp], _disp_y_dot[_qp], _disp_z_dot[_qp]) * ADRealVectorValue(_grad_u[_qp](0, 0), _grad_u[_qp](0, 1), _grad_u[_qp](0, 2));
  ADReal y_comp = ADRealVectorValue(_disp_x_dot[_qp], _disp_y_dot[_qp], _disp_z_dot[_qp]) * ADRealVectorValue(_grad_u[_qp](1, 0), _grad_u[_qp](1, 1), _grad_u[_qp](1, 2));
  ADReal z_comp = ADRealVectorValue(_disp_x_dot[_qp], _disp_y_dot[_qp], _disp_z_dot[_qp]) * ADRealVectorValue(_grad_u[_qp](2, 0), _grad_u[_qp](2, 1), _grad_u[_qp](2, 2));
  return -_rho[_qp] * ADRealVectorValue(x_comp, y_comp, z_comp);
}