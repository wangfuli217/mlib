#define NDEBUG

#include <QLinkedList>
#include <QVector>
#include <QSet>
#include <QMap>
#include <QHash>
#include <QtAlgorithms>

#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <sys/types.h>
#include <sys/resource.h>
#include <sys/time.h>

using namespace std;

#include "common.h"

/********************************************************************************************/

static void test_list(size_t n)
{
  rand_init();
  QLinkedList<unsigned int> a1, a2;
  for(size_t i = 0; i < n; i++) {
    a1.push_back(rand_get() );
    a2.push_back(rand_get() );
  }
  unsigned int s = 0;
  for (QLinkedList<unsigned int>::const_iterator ci1 = a1.begin(), ci2 = a2.begin (); ci1 != a1.end(); ++ci1, ++ci2) {
    s += *ci1 * *ci2;
  }
  g_result = s;
}

/********************************************************************************************/

static void test_array(size_t n)
{
  rand_init();
  QVector<unsigned int> a1, a2;
  for(size_t i = 0; i < n; i++) {
    a1.push_back(rand_get() );
    a2.push_back(rand_get() );
  }
  unsigned int s = 0;
  for(unsigned long i = 0; i < n; i++) {
    s += a1[i] * a2[i];
  }
  g_result = s;
}

/********************************************************************************************/

/* NOTE: QSet doesn't perform full ordering. I can't find any container
   which is a set with total ordering within Qt. So we have to fake it. */
static void test_rbtree(size_t n)
{
  rand_init();
  QMap<unsigned long, QHashDummyValue> tree;

  for (size_t i = 0; i < n; i++) {
    tree[rand_get()] = QHashDummyValue();
  }
  rand_init();
  unsigned int s = 0;
  for (size_t i = 0; i < n; i++) {
    QMap<unsigned long, QHashDummyValue>::iterator it = tree.find(rand_get());
    if (it != tree.end())
      s += it.key();
  }
  g_result = s;
}

/********************************************************************************************/

static void
test_dict1(unsigned long  n)
{
  rand_init();
  QMap<unsigned long, unsigned long> dict;

  for (size_t i = 0; i < n; i++) {
    dict[rand_get()] = rand_get();
  }
  rand_init();
  unsigned int s = 0;
  for (size_t i = 0; i < n; i++) {
    QMap<unsigned long, unsigned long>::iterator it = dict.find(rand_get());
    if (it != dict.end())
      s += it.value();
  }
  g_result = s;
}

/********************************************************************************************/

static void
test_dict2(unsigned long  n)
{
  rand_init();
  QHash<unsigned long, unsigned long> dict;

  for (size_t i = 0; i < n; i++) {
    dict[rand_get()] = rand_get();
  }
  rand_init();
  unsigned int s = 0;
  for (size_t i = 0; i < n; i++) {
    QHash<unsigned long, unsigned long>::iterator it = dict.find(rand_get());
    if (it != dict.end())
      s += it.value();
  }
  g_result = s;
}

/********************************************************************************************/
struct char_array_s {
  char a[256];
  char_array_s () { a[0] = 0 ; }
  char_array_s ( const char_array_s & other) { strcpy(a, other.a); }
  bool operator==(const char_array_s &other) const { return strcmp(a, other.a) == 0; }
};

uint qHash(const char_array_s &k)
{
  size_t hash = 0;
  const char *s = k.a;
  while (*s) hash = hash * 31421 + (*s++) + 6927;
  return hash;
};

static void
test_dict_big(unsigned long  n)
{
  rand_init();
  QHash<char_array_s, char_array_s> dict;

  for (size_t i = 0; i < n; i++) {
    char_array_s s1, s2;
    sprintf(s1.a, "%u", rand_get());
    sprintf(s2.a, "%u", rand_get());
    dict[s1] = s2;
  }
  rand_init();
  unsigned int s = 0;
  for (size_t i = 0; i < n; i++) {
    char_array_s s1;
    sprintf(s1.a, "%u", rand_get());
    QHash<char_array_s, char_array_s>::iterator it = dict.find(s1);
    if (it != dict.end())
      s ++;
  }
  g_result = s;
}

/********************************************************************************************/

static void test_sort(size_t n)
{
  rand_init();
  QVector<float> a1;
  for(size_t i = 0; i < n; i++) {
    a1.push_back(rand_get() );
  }
  qSort(a1.begin(), a1.end());
  g_result = a1[0];
}

/********************************************************************************************/

int main(int argc, const char *argv[])
{
  int n = (argc > 1) ? atoi(argv[1]) : 0;
  if (n == 1)
    test_function("List   time",10000000, test_list);
  if (n == 2)
    test_function("Array  time", 100000000, test_array);
  if (n == 3)
     test_function("Rbtree time", 1000000, test_rbtree);
  if (n == 4)
    test_function("Dict(m)time", 1000000, test_dict1);
  if (n == 5)
    test_function("Dict(u)time", 1000000, test_dict2);
  if (n == 6)
    test_function("DictB  time", 1000000, test_dict_big);
  if (n == 7)
    test_function("Sort   time", 10000000, test_sort);
}
