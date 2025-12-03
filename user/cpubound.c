#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

int
main(int argc, char *argv[])
{
  printf("CPU-bound process (PID %d) starting...\n\n", getpid());
  
  // Run 10 iterations of CPU-intensive work
  for (int i = 0; i < 10; i++) {
    // Burn CPU cycles
    for (int j = 0; j < 10000000; j++) {
      asm volatile("nop");  // Do nothing, just consume CPU
    }
    
    printf("--- Iteration %d ---\n", i + 1);
    getprocinfo();
    printf("\n");
  }
  
  printf("CPU-bound process completed!\n");
  exit(0);
}
