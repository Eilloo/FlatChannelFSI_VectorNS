#include "FlatChannelFSI_VectorNSApp.h"
#include "Moose.h"
#include "AppFactory.h"
#include "ModulesApp.h"
#include "MooseSyntax.h"

InputParameters
FlatChannelFSI_VectorNSApp::validParams()
{
  InputParameters params = MooseApp::validParams();

  return params;
}

FlatChannelFSI_VectorNSApp::FlatChannelFSI_VectorNSApp(InputParameters parameters) : MooseApp(parameters)
{
  FlatChannelFSI_VectorNSApp::registerAll(_factory, _action_factory, _syntax);
}

FlatChannelFSI_VectorNSApp::~FlatChannelFSI_VectorNSApp() {}

void
FlatChannelFSI_VectorNSApp::registerAll(Factory & f, ActionFactory & af, Syntax & syntax)
{
  ModulesApp::registerAll(f, af, syntax);
  Registry::registerObjectsTo(f, {"FlatChannelFSI_VectorNSApp"});
  Registry::registerActionsTo(af, {"FlatChannelFSI_VectorNSApp"});

  /* register custom execute flags, action syntax, etc. here */
}

void
FlatChannelFSI_VectorNSApp::registerApps()
{
  registerApp(FlatChannelFSI_VectorNSApp);
}

/***************************************************************************************************
 *********************** Dynamic Library Entry Points - DO NOT MODIFY ******************************
 **************************************************************************************************/
extern "C" void
FlatChannelFSI_VectorNSApp__registerAll(Factory & f, ActionFactory & af, Syntax & s)
{
  FlatChannelFSI_VectorNSApp::registerAll(f, af, s);
}
extern "C" void
FlatChannelFSI_VectorNSApp__registerApps()
{
  FlatChannelFSI_VectorNSApp::registerApps();
}
