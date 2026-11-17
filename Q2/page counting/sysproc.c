int
sys_freepages(void)
{
  return freepages();
}

int
sys_va2pa(void)
{
  int va;
  if(argint(0, &va) < 0)
    return -1;
  return va2pa_helper(myproc()->pgdir, (uint)va);
}