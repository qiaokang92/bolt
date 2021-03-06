#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <klee/klee.h>
#include "lib/containers/double-chain.h"

// TODO: double check that this model is enough for the NAT scenario

#define ALLOW(chain) klee_allow_access((chain), sizeof(struct DoubleChain))
#define DENY(chain) klee_forbid_access((chain), sizeof(struct DoubleChain), "allocated_chain_do_not_dereference")

struct DoubleChain
{
  int dchain_out_of_space;
  int new_index;
  int is_index_allocated;
};

char *prefix; /* For tracing */

__attribute__((noinline)) int dchain_allocate(int index_range, struct DoubleChain **chain_out)
{
  klee_trace_ret();

  // TODO not needed if malloc can fail
  int is_dchain_allocated = klee_int("is_dchain_allocated");
  *chain_out = malloc(sizeof(struct DoubleChain));

  klee_possibly_havoc(*chain_out, sizeof(struct DoubleChain), "chain_out");

  if (is_dchain_allocated && *chain_out != NULL)
  {
    memset(*chain_out, 0, sizeof(struct DoubleChain));
    (*chain_out)->new_index = klee_int("new_index");
    klee_assume(0 <= (*chain_out)->new_index);
    klee_assume((*chain_out)->new_index < index_range);
    (*chain_out)->is_index_allocated = 0;
    (*chain_out)->dchain_out_of_space = klee_int("dchain_out_of_space");
    TRACE_VAL((uint32_t)(*chain_out), "dchain", _u32)
    DENY(*chain_out);
    return 1;
  }
  else
  {
    return 0;
  }
}

__attribute__((noinline)) int dchain_allocate_new_index(struct DoubleChain *chain, int *index_out,
                                                        time_t time)
{
  klee_trace_ret();
  TRACE_VAL((uint32_t)chain, "dchain", _u32)

  ALLOW(chain);
  TRACE_VAR(chain->dchain_out_of_space, "dchain_out_of_space")
  if (chain->dchain_out_of_space)
  {
    DENY(chain);
    return 0;
  }

  klee_assert(!(chain->is_index_allocated));
  *index_out = chain->new_index;
  chain->is_index_allocated = 1;
  DENY(chain);
  return 1;
}

__attribute__((noinline)) int dchain_rejuvenate_index(struct DoubleChain *chain, int index,
                                                      time_t time)
{
  klee_trace_ret();
  klee_assert(chain != NULL);
  return 1;
}

__attribute__((noinline)) int dchain_expire_one_index(struct DoubleChain *chain,
                                                      int *index_out, time_t time)
{
  klee_trace_ret();

  klee_assert(chain != NULL);
  klee_assert(0 && "not supported in this model");
  return 0;
}

__attribute__((noinline)) int dchain_is_index_allocated(struct DoubleChain *chain, int index)
{
  klee_trace_ret();

  klee_assert(chain != NULL);
  return klee_int("dchain_is_index_allocated");
}

void dchain_make_space(struct DoubleChain *chain)
{
  //Do not trace. this function is internal for the Expirator model.
  ALLOW(chain);
  klee_assert(chain->is_index_allocated == 0);
  //Do not trace internal stub control functions.
  chain->dchain_out_of_space = 0;
  DENY(chain);
}

void dchain_reset(struct DoubleChain *chain, int index_range)
{
  //Do not trace. This function is an internal knob of the model.
  ALLOW(chain);
  chain->new_index = klee_int("new_index");
  klee_assume(0 <= chain->new_index);
  klee_assume(chain->new_index < index_range);
  chain->is_index_allocated = 0;
  chain->dchain_out_of_space = klee_int("dchain_out_of_space");
  DENY(chain);
}
