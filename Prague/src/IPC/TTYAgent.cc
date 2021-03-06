/*$Id$
 *
 * This source file is a part of the Fresco Project.
 * Copyright (C) 1999, 2000 Stefan Seefeld <stefan@fresco.org>
 * http://www.fresco.org
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
 */
#include "Prague/IPC/ptybuf.hh"
#include "Prague/IPC/TTYAgent.hh"
#include "Prague/Sys/Tracer.hh"
#include <unistd.h>
#include <cstdlib>

using namespace Prague;

TTYAgent::TTYAgent(const std::string &cmd, IONotifier *io, EOFNotifier *eof) :
  Coprocess(cmd, io, eof)
{ }

TTYAgent::~TTYAgent()
{
    stop();
    shutdown(in|out|err);
}

void TTYAgent::start() throw(std::runtime_error)
{
    Trace trace("TTYAgent::start");
    if (pid() >= 0)
    {
        terminate();
        ptybuf *pty  = new ptybuf;
        int fd = pty->openpty();
        switch(_id = fork())
        {
          case -1:
            _id = 0;
            // SystemError("cannot fork", true);
            return;
          case  0:
            {
                // Set this (child) process to be its own process group leader
                // This allows the shell to implement job control properly
                // FIXME: portability of this function is uncertain
                setpgid(0, 0);
                int fds = pty->opentty();
                if (fds < 0) _exit(EXIT_FAILURE);
                close(fd);
                dup2(fds, fileno(stdin));
                dup2(fds, fileno(stdout));
                dup2(fds, fileno(stderr));
                if (fds > fileno(stderr)) close(fds);
                const char *argv[4];
                argv[0] = "/bin/sh";
                argv[1] = "-c";
                argv[2] = _path.c_str();
                argv[3] = 0;
                execvp ("/bin/sh", (char**) argv);
                perror("/bin/sh");
                _exit(EXIT_FAILURE);
                break;
            }
          default:
            _inbuf = _outbuf = pty;
            _errbuf = 0;
            _inbuf->async(true);
            break;
        }
    }
    _running = true;
    mask(out);
    Coprocess::start();
}

void TTYAgent::set_window_size(unsigned short columns, unsigned short rows)
{
#if defined(HAVE_IOCTL)
    struct winsize ws;
    ws.ws_col = columns;
    ws.ws_row = rows;
    ws.ws_xpixel = ws.ws_ypixel = 0;
    if (ioctl (master, TIOCSWINSZ, &ws) != 0);
#endif
}
