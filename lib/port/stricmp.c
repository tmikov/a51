int _stricmp ( const char * dst, const char * src  )
{
  int f,l;

  do
  {
    if ( ((f = (unsigned char)(*(dst++))) >= 'A') && (f <= 'Z') )
      f -= ('A' - 'a');

    if ( ((l = (unsigned char)(*(src++))) >= 'A') && (l <= 'Z') )
      l -= ('A' - 'a');
  }
  while ( f && (f == l) );

  return f - l;
}
