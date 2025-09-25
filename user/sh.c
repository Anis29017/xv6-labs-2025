// Simple shell for xv6 (user/sh.c)

#include "kernel/types.h"
#include "kernel/stat.h"
#include "kernel/fcntl.h"
#include "user/user.h"

// Command types
#define EXEC  1
#define REDIR 2
#define PIPE  3
#define LIST  4
#define BACK  5

struct cmd {
  int type;
};

struct execcmd {
  int type;
  char *argv[10];
};

struct redircmd {
  int type;
  struct cmd *cmd;
  char *file;
  int mode;
  int fd;
};

struct pipecmd {
  int type;
  struct cmd *left;
  struct cmd *right;
};

struct listcmd {
  int type;
  struct cmd *left;
  struct cmd *right;
};

struct backcmd {
  int type;
  struct cmd *cmd;
};

// -------------------------------------------------------------

// panic helper
void
panic(char *s)
{
  fprintf(2, "panic: %s\n", s);
  exit(1);
}

// check if fd is console
int
isconsole(int fd)
{
  struct stat st;
  if(fstat(fd, &st) < 0)
    return 0;
  return st.type == T_DEVICE;
}

// -------------------------------------------------------------
// Parsing stubs (real parser is longer, provided in sh.c normally)
// For assignment, you likely don’t need full parsing – just basic.

struct cmd*
parsecmd(char *s)
{
  static struct execcmd cmd;
  cmd.type = EXEC;
  cmd.argv[0] = s;
  cmd.argv[1] = 0;
  return (struct cmd*)&cmd;
}

// -------------------------------------------------------------

void
runcmd(struct cmd *cmd)
{
  if(cmd == 0)
    exit(0);

  struct execcmd *ecmd;

  switch(cmd->type){
  case EXEC:
    ecmd = (struct execcmd*)cmd;
    if(ecmd->argv[0] == 0)
      exit(0);
    exec(ecmd->argv[0], ecmd->argv);
    fprintf(2, "exec %s failed\n", ecmd->argv[0]);
    break;
  }
  exit(0);
}

// -------------------------------------------------------------

// Read a command
int
getcmd(char *buf, int nbuf)
{
  if(isconsole(0))
    fprintf(2, "$ ");

  memset(buf, 0, nbuf);
  gets(buf, nbuf);

  if(buf[0] == 0) // EOF
    return -1;
  return 0;
}

// -------------------------------------------------------------

int
main(void)
{
  static char buf[100];

  while(getcmd(buf, sizeof(buf)) >= 0){
    if(buf[0] == 0)
      continue;

    // built-in cd
    if(buf[0]=='c' && buf[1]=='d' && buf[2]==' '){
      buf[strlen(buf)-1] = 0; // chop \n
      if(chdir(buf+3) < 0)
        fprintf(2, "cannot cd %s\n", buf+3);
      continue;
    }

    if(fork() == 0)
      runcmd(parsecmd(buf));
    wait(0);
  }
  exit(0);
}
