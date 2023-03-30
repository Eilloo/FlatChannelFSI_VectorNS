//* This file is part of the MOOSE framework
//* https://www.mooseframework.org
//*
//* All rights reserved, see COPYRIGHT for full restrictions
//* https://github.com/idaholab/moose/blob/master/COPYRIGHT
//*
//* Licensed under LGPL 2.1, please see LICENSE for details
//* https://www.gnu.org/licenses/lgpl-2.1.html
#include "FlatChannelFSI_VectorNSTestApp.h"
#include "FlatChannelFSI_VectorNSApp.h"
#include "Moose.h"
#include "AppFactory.h"
#include "MooseSyntax.h"
#include "ModulesApp.h"

InputParameters
FlatChannelFSI_VectorNSTestApp::validParams()
{
  InputParameters params = FlatChannelFSI_VectorNSApp::validParams();
  return params;
}

FlatChannelFSI_VectorNSTestApp::FlatChannelFSI_VectorNSTestApp(InputParameters parameters) : MooseApp(parameters)
{
  FlatChannelFSI_VectorNSTestApp::registerAll(
      _factory, _action_factory, _syntax, getParam<bool>("allow_test_objects"));
}

FlatChannelFSI_VectorNSTestApp::~FlatChannelFSI_VectorNSTestApp() {}

void
FlatChannelFSI_VectorNSTestApp::registerAll(Factory & f, ActionFactory & af, Syntax & s, bool use_test_objs)
{
  FlatChannelFSI_VectorNSApp::registerAll(f, af, s);
  if (use_test_objs)
  {
    Registry::registerObjectsTo(f, {"FlatChannelFSI_VectorNSTestApp"});
    Registry::registerActionsTo(af, {"FlatChannelFSI_VectorNSTestApp"});
  }
}

void
FlatChannelFSI_VectorNSTestApp::registerApps()
{
  registerApp(FlatChannelFSI_VectorNSApp);
  registerApp(FlatChannelFSI_VectorNSTestApp);
}

/***************************************************************************************************
 *********************** Dynamic Library Entry Points - DO NOT MODIFY ******************************
 **************************************************************************************************/
// External entry point for dynamic application loading
extern "C" void
FlatChannelFSI_VectorNSTestApp__registerAll(Factory & f, ActionFactory & af, Syntax & s)
{
  FlatChannelFSI_VectorNSTestApp::registerAll(f, af, s);
}
extern "C" void
FlatChannelFSI_VectorNSTestApp__registerApps()
{
  FlatChannelFSI_VectorNSTestApp::registerApps();
}
