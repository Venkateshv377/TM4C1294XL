# invoke SourceDir generated makefile for empty.pem4f
empty.pem4f: .libraries,empty.pem4f
.libraries,empty.pem4f: package/cfg/empty_pem4f.xdl
	$(MAKE) -f C:\TI_RTOS\Workspace_TM4C1294XL\task/src/makefile.libs

clean::
	$(MAKE) -f C:\TI_RTOS\Workspace_TM4C1294XL\task/src/makefile.libs clean

