#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

int
main(int argc, char *argv[])
{
  printf("I/O-bound process (PID %d) starting...\n\n", getpid());
  
  // Run 10 iterations with I/O waits
  for (int i = 0; i < 10; i++) {
    printf("--- I/O Operation %d ---\n", i + 1);
    getprocinfo();
    printf("Sleeping (simulating I/O)...\n\n");
    
    sleep(2);  // Simulate I/O wait
  }
  
  printf("I/O-bound process completed!\n");
  exit(0);
}
