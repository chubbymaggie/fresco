/*+P
 * This file is part of OffiX,
 * a C++ API for the X Window System and Unix
 * Copyright (C) 1995-98  Stefan Seefeld
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Library General Public
 * License as published by the Free Software Foundation; either
 * version 2 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Library General Public License for more details.
 *
 * You should have received a copy of the GNU Library General Public
 * License along with this library; if not, write to the
 * Free Software Foundation, Inc., 675 Mass Ave, Cambridge,
 * MA 02139, USA.
 -P*/
static char *rcsid = "$Id$";

#include "Prague/IPC/PipeAgent.hh"
#include "Prague/IPC/pipebuf.hh"

/* @Method{void PipeAgent::start()}
 *
 * @Description{start child process if we're not listening to stdin}
 */
void PipeAgent::start()
{
  if (pid >= 0)
    {
      terminate();
      pipebuf *pin  = new pipebuf(ios::out); // the stdin for the child is an output stream for the parent...
      pipebuf *pout = new pipebuf(ios::in);  // the stdout for the child is an input stream for the parent...
      pipebuf *perr = new pipebuf(ios::in);  // the stderr for the child is an input stream for the parent...
      int fin = pin->open();
      int fout = pout->open();
      int ferr = perr->open();
      if (fin == -1 || fout == -1 || ferr == -1) { Error("communication setup failed", true); return;}
      switch(pid = fork())
	{
	case -1:
	  pid = 0;
	  SystemError("cannot fork", true);
	  return;
	case  0:
	  dup2(fin, fileno(stdin)); close(fin);
	  dup2(fout, fileno(stdout)); close(fout);
	  dup2(ferr, fileno(stderr)); close(ferr);
	  const char *argv[4];
	  argv[0] = "/bin/sh";
	  argv[1] = "-c";
	  argv[2] = path.c_str();
	  argv[3] = 0;
	  execvp ("/bin/sh", (char**) argv);
	  perror("/bin/sh");
	  _exit(EXIT_FAILURE);
	  break;
	default:
 	  inbuf = pin; close(fin);
 	  outbuf = pout; close(fout);
 	  errbuf = perr; close(ferr);
	  if (bound)
	    {
	      inbuf->setnonblocking();
	      outbuf->setnonblocking();
	      errbuf->setnonblocking();
	    }
 	  break;
	}
    }
  if (bound) initTimer();
  setactive();
};