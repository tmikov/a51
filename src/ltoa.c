char * ultoa ( unsigned long num, char * res, int radix )
{
  unsigned i;
  static const char digs[] = "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ";
  char buf[64];


  // Generate in a temp buffer
  i = 0;
  do
  {
    buf[i++] = digs[num % (unsigned)radix];
  }
  while (num /= (unsigned)radix);

  // Copy to result buffer
  do
    *res++ = buf[--i];
  while (i);

  *res = 0;
  return res;
};

char * ltoa ( long num, char * res, int radix )
{
  if (radix == 10 && num < 0)
  {
    *res++ = '-';
    num = -num;
  }
  return ultoa( num, res, (unsigned)radix );
};

char * itoa ( int num, char * res, int radix )
{
  if (radix == 10 && num < 0)
  {
    *res++ = '-';
    num = -num;
  }
  return ultoa( num, res, (unsigned)radix );
};

