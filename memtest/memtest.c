#include "types.h"
#include "stat.h"
#include "user.h"

int global_var = 42; // Global variable for testing virtual-to-physical address translation[cite: 2]

int
main(int argc, char *argv[])
{
  printf(1, "--- Starting Memory Test (memtest) ---\n");

  // (i) freepages() at start-up[cite: 2]
  int fp_start = freepages();
  printf(1, "1. Initial free pages: %d\n", fp_start);

  // (ii) freepages() after sbrk(10 * 4096)[cite: 2]
  char *p = sbrk(10 * 4096);
  if(p == (char*)-1) {
    printf(1, "sbrk failed\n");
    exit();
  }
  int fp_sbrk = freepages();
  printf(1, "2. Free pages after sbrk(10 * 4096): %d (Diff: %d)\n", fp_sbrk, fp_start - fp_sbrk);

  // (iii) Virtual and physical address of a global variable[cite: 2]
  uint g_va = (uint)&global_var;
  int g_pa = va2pa(g_va);
  printf(1, "3. Global variable -> Virtual: 0x%x, Physical: 0x%x\n", g_va, g_pa);

  // (iv) Result of va2pa(0x7FFFFFFF)[cite: 2]
  int invalid_pa = va2pa(0x7FFFFFFF);
  printf(1, "4. va2pa(0x7FFFFFFF) result: %d (Expected: -1)\n", invalid_pa);

  // (v) Fork test for copy-on-fork/copy-on-write physical isolation[cite: 2]
  int pid = fork();
  if(pid < 0) {
    printf(1, "fork failed\n");
    exit();
  }

  if(pid == 0) {
    // Child process
    int child_g_pa = va2pa(g_va);
    printf(1, "5. [Child] Global variable -> Virtual: 0x%x, Physical: 0x%x\n", g_va, child_g_pa);
    exit();
  } else {
    // Parent process
    wait(); // Wait for child to exit
    int parent_g_pa = va2pa(g_va);
    printf(1, "5. [Parent] Global variable -> Virtual: 0x%x, Physical: 0x%x\n", g_va, parent_g_pa);

    // freepages() after fork and after child exit/reap[cite: 2]
    int fp_end = freepages();
    printf(1, "6. Final free pages after child exit & wait: %d\n", fp_end);
    printf(1, "--- Memory Test Completed ---\n");
    exit();
  }
}