/* 
 * tclTest.c --
 *
 *	Test driver for TCL.
 *
 * Copyright 1987-1991 Regents of the University of California
 * All rights reserved.
 *
 * Permission to use, copy, modify, and distribute this
 * software and its documentation for any purpose and without
 * fee is hereby granted, provided that the above copyright
 * notice appears in all copies.  The University of California
 * makes no representations about the suitability of this
 * software for any purpose.  It is provided "as is" without
 * express or implied warranty.
 *
 * $Id: tinytcl.c,v 1.2 2001/04/29 20:56:17 karll Exp $
 */

#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <stdlib.h>
#include "tcl.h"

//#include <alloc.h>

Tcl_Interp *interp;
Tcl_CmdBuf buffer;
char dumpFile[100];
int quitFlag = 0;

char initCmd[] =
    "puts stdout \"\nEmbedded Tcl 6.8.0\n\"";//; source tcl_sys/autoinit.tcl";

	/* ARGSUSED */
int
cmdCheckmem(clientData, interp, argc, argv)
    ClientData clientData;
    Tcl_Interp *interp;
    int argc;
    char *argv[];
{
    if (argc != 2) {
	Tcl_AppendResult(interp, "wrong # args: should be \"", argv[0],
		" fileName\"", (char *) NULL);
	return TCL_ERROR;
    }
    strcpy(dumpFile, argv[1]);
    quitFlag = 1;
    return TCL_OK;
}

	/* ARGSUSED */
int
cmdEcho(clientData, interp, argc, argv)
    ClientData clientData;
    Tcl_Interp *interp;
    int argc;
    char *argv[];
{
    int i;

    for (i = 1; ; i++) {
	if (argv[i] == NULL) {
	    if (i != argc) {
		echoError:
		sprintf(interp->result,
		    "argument list wasn't properly NULL-terminated in \"%s\" command",
		    argv[0]);
	    }
	    break;
	}
	if (i >= argc) {
	    goto echoError;
	}
	fputs(argv[i], stdout);
	if (i < (argc-1)) {
	    printf(" ");
	}
    }
    printf("\n");
    return TCL_OK;
}

int
main(int argc, char *argv[])
{
    char line[1000], *cmd;
    int result, gotPartial;
	FILE *in;
	FILE *out;
	char *filename = NULL;

    interp = Tcl_CreateInterp();
#ifdef TCL_MEM_DEBUG
    Tcl_InitMemory(interp);
#endif
    Tcl_InitDebug (interp);
    TclX_InitGeneral (interp);
//    Tcl_InitDos (interp);
    Tcl_InitReaddir (interp);

    Tcl_CreateCommand(interp, "echo", cmdEcho, (ClientData) "echo",
	    (Tcl_CmdDeleteProc *) NULL);
    Tcl_CreateCommand(interp, "checkmem", cmdCheckmem, (ClientData) 0,
	    (Tcl_CmdDeleteProc *) NULL);

    buffer = Tcl_CreateCmdBuf();
	in = stdin;
	if (argc > 1) {
		filename = argv[1];

		if (strcmp(filename, "-") != 0) {
		    in = fopen(filename, "r");

		    if (!in) {
			    fprintf(stderr, "Couldn't open %s - %m", filename);
			    exit(1);
		    }
		} else {
		    in = stdin;
		}
	}
	out = stdout;

#ifndef TCL_GENERIC_ONLY
    if (!filename) {
	result = Tcl_Eval(interp, initCmd, 0, (char **) NULL);
	if (result != TCL_OK) {
	    printf("%s\n", interp->result);
	    exit(1);
	}
    }
#endif
    gotPartial = 0;
    while (1) {
	clearerr(in);
	if (!gotPartial) {
	    if (!filename) 
		    fputs("% ", out);
	    fflush(out);
	}
	if (fgets(line, 1000, in) == NULL) {
	    if (!gotPartial) {
		exit(0);
	    }
	    line[0] = 0;
	}
	cmd = Tcl_AssembleCmd(buffer, line);
	if (cmd == NULL) {
	    gotPartial = 1;
	    continue;
	}

	gotPartial = 0;
	result = Tcl_Eval(interp, cmd, 0, (char **)NULL);
	if (result == TCL_OK) {
	    if ((*interp->result != 0) && !filename){
		printf("%s\n", interp->result);
	    }
	    if (quitFlag) {
		Tcl_DeleteInterp(interp);
		Tcl_DeleteCmdBuf(buffer);
#ifdef TCL_MEM_DEBUG
		Tcl_DumpActiveMemory(dumpFile);
#endif
		exit(0);
	    }
	} else {
	    if (result == TCL_ERROR) {
		printf("Error");
	    } else {
		printf("Error %d", result);
	    }
	    if (*interp->result != 0) {
		printf(": %s\n", interp->result);
	    } else {
		printf("\n");
	    }
	}
    }
}
