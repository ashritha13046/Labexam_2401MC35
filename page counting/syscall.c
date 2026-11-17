// Declare extern prototypes
extern int sys_freepages(void);
extern int sys_va2pa(void);

// Add to static-array of syscall pointers
[SYS_freepages] sys_freepages,
[SYS_va2pa]     sys_va2pa,