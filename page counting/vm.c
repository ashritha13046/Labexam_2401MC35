// Helper function to walk the page table and translate virtual to physical address[cite: 2]
int
va2pa_helper(pde_t *pgdir, uint va)
{
  pte_t *pte;
  uint pa;

  // walkpgdir is static in vm.c, so we use it directly here
  pte = walkpgdir(pgdir, (void*)va, 0);
  if(!pte)
    return -1;
  if(!(*pte & PTE_P)) // Page not present
    return -1;
  if(!(*pte & PTE_U)) // Not accessible from user mode[cite: 2]
    return -1;

  pa = PTE_ADDR(*pte);
  return pa | (va & 0xFFF); // Frame address plus offset[cite: 2]
}