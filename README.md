FlatChannelFSI_VectorNS
=====

The additional interface kernels are intended to apply the same physics as those used in the original input file (included for reference). The only difference is that the fluid velocity is a vector variable.
This also required a modified 'ConvectedMesh' kernel, which is included. However, to isolate the interface kernels, this can be disabled and the simulation should still solve.
The vector version also uses exclusively AD kernels and materials. Despite this, 'NEWTON' does not solve, whereas 'PJFNK' does. This is the behaviour I am trying to understand.
