/* -*- C -*- */
/* I hate this bloody country. Smash. */
/* This file is part of ResLib project */

#include <stdio.h>
#include <stdlib.h>

#include <reslib.h>

TYPEDEF_STRUCT (list_t,	(list_t *, next))
TYPEDEF_STRUCT (array_t, RARRAY (list_t, rarray))

#define ARRAY_SIZE (2 * 160 * 1024)

static array_t data = {
  .rarray = {
    .alloc_size = -1,
    .size = ARRAY_SIZE * sizeof (list_t),
    .data = (list_t[]){[0 ... ARRAY_SIZE - 1] = {.next = NULL}},
  }
};

int
main (void)
{
  rl_ra_rl_ptrdes_t ptrs;
  int i;

  for (i = 0; i < ARRAY_SIZE - 1; ++i)
    data.rarray.data[i].next = &data.rarray.data[i + 1];
  data.rarray.data[ARRAY_SIZE - 1].next = &data.rarray.data[ARRAY_SIZE - 1];

  char * str = RL_SAVE_XML1 (list_t, &data.rarray.data[0]);
  if (str)
    {
      list_t list = RL_LOAD_XML1 (list_t, str);
      RL_FREE_RECURSIVELY (list_t, &list);
      RL_FREE (str);
    }

  for (i = 1; i < ARRAY_SIZE; ++i)
    data.rarray.data[i].next = &data.rarray.data[i - 1];
  data.rarray.data[0].next = &data.rarray.data[0];
  
  ptrs = RL_SAVE_RL (array_t, &data);
  if (ptrs.ra.data)
    RL_FREE (ptrs.ra.data);

  return (EXIT_SUCCESS);
}
