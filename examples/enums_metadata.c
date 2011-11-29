
#include <stdio.h>
#include <stdlib.h>
#include <reslib.h>

TYPEDEF_ENUM (color_t,
              (BLACK, = 17),
              (RED),
              (GREEN,   = 2),
              (YELLOW,  = 3),
              (BLUE,    = 4),
              MAGENTA,
              CYAN,              
              WHITE
              )

int main()
{
  char const name[] = "color_t";
  rl_td_t const* td = rl_get_td_by_name(name);
  
  if (!td)
  {
    fprintf(stderr,
            "error: can't obtain type information for '%s'\n",
            name);
    return EXIT_FAILURE;
  }
  
  size_t const num_constants = td->fields.size / sizeof(td->fields.data[0]);
  
  for (size_t i = 0; i < num_constants; ++i)
  {
    rl_fd_t const* fd = &td->fields.data[i];
    printf("%u: %s = %lli\n", i, fd->name, fd->param.enum_value);
  }
  
  return EXIT_SUCCESS;
}