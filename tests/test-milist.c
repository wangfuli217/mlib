/*
 * MLIB - TEST module
 *
 * Copyright (c) 2017, Patrick Pelissier
 * All rights reserved.
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 * + Redistributions of source code must retain the above copyright
 *   notice, this list of conditions and the following disclaimer.
 * + Redistributions in binary form must reproduce the above copyright
 *   notice, this list of conditions and the following disclaimer in the
 *   documentation and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE REGENTS AND CONTRIBUTORS ``AS IS'' AND ANY
 * EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE REGENTS AND CONTRIBUTORS BE LIABLE FOR ANY
 * DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
 * ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/
#include <stdio.h>

#include "m-i-list.h"

typedef struct test_s {
  int n;
  ILIST_INTERFACE (ilist_tname, struct test_s);
} test_t;

#include "coverage.h"
START_COVERAGE
ILIST_DEF(ilist_tname, test_t)
END_COVERAGE
     
static void test(void)
{
  test_t x1, x2, x3;
  ilist_tname_t list;

  x1.n = 1;
  x2.n = 2;
  x3.n = 3;
  
  ilist_tname_init(list);
  
  assert (ilist_tname_size (list) == 0);
  ilist_tname_push_back (list, &x3);
  assert (ilist_tname_size (list) == 1);
  ilist_tname_push_front (list, &x1);
  assert (ilist_tname_size (list) == 2);
  ilist_tname_push_after (&x1, &x2);
  assert (ilist_tname_size (list) == 3);
  assert (ilist_tname_front (list)->n == 1);
  assert (ilist_tname_back (list)->n == 3);

  int n = 1;
  for M_EACH(item, list, ILIST_OPLIST(ilist_tname)) {
      assert (n == item->n);
      n++;
    }
  assert (n == 4);
  ilist_tname_it_t it1, it2;
  ilist_tname_it_last(it1, list);
  assert (!ilist_tname_end_p(it1));
  assert (ilist_tname_last_p(it1));
  ilist_tname_it_end(it2, list);
  assert (ilist_tname_end_p(it2));
  assert (ilist_tname_last_p(it2));
  assert (!ilist_tname_it_equal_p(it1, it2));
  for(ilist_tname_it_set(it2, it1) ; !ilist_tname_end_p(it2) ; ilist_tname_previous(it2)) {
    const test_t *item = ilist_tname_cref(it2);
    n--;
    assert (n == item->n);
  }
  assert (n == 1);
  ilist_tname_it_set(it2, it1);
  assert (ilist_tname_it_equal_p(it1, it2));

  for M_EACH(item, list, ILIST_OPLIST(ilist_tname)) {
      if (item->n == 2)
        ilist_tname_unlink(item);
    }
  assert (ilist_tname_size(list) == 2);
  assert (&x1 == ilist_tname_pop_front(list));
  assert (ilist_tname_size(list) == 1);
  assert (&x3 == ilist_tname_pop_back(list));
  assert (ilist_tname_size(list) == 0);
  assert (ilist_tname_empty_p(list));
  
  for M_EACH(item, list, ILIST_OPLIST(ilist_tname)) {
      (void)item;
      assert(false);
    }

  ilist_tname_clear(list);
}

int main(void)
{
  test();
  exit(0);
}
