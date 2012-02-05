#ifndef _REGRESSION_H_
#define _REGRESSION_H_

#include <check.h>
#include <string.h>

#include <reslib.h>

#ifdef HAVE_BISON_FLEX
#define BISON_FLEX_METHODS XML1, SCM, CINIT, JSON,
#else /* HAVE_BISON_FLEX */
#define BISON_FLEX_METHODS
#endif /* HAVE_BISON_FLEX */

#ifdef HAVE_LIBXML2
#define LIBXML2_METHODS XML2,
#else /* HAVE_LIBXML2 */
#define LIBXML2_METHODS
#endif /* HAVE_LIBXML2 */

#define TEST_METHODS LIBXML2_METHODS BISON_FLEX_METHODS XDR

extern Suite * suite;

#define MR_START_TEST(NAME, ...)					\
  static void NAME (int);						\
  static inline void __attribute__ ((constructor)) init_ ## NAME (void) \
  {									\
    if (NULL == suite)							\
      suite = suite_create ("main");					\
    if (suite)								\
      {									\
	TCase * tcase = tcase_create ("" __VA_ARGS__);			\
	if (tcase)							\
	  {								\
	    tcase_add_test (tcase, NAME);				\
	    suite_add_tcase (suite, tcase);				\
	  }								\
      }									\
  };									\
  START_TEST (NAME)

#define SERIALIZE_METHOD MR_SAVE_CINIT

#define MEM_CMP(TYPE, X, Y, ...) memcmp (X, Y, sizeof (TYPE))
#define CMP_SERIALIAZED(TYPE, X, Y, ...) ({				\
      char * x_ = SERIALIZE_METHOD (TYPE, X);				\
      char * y_ = SERIALIZE_METHOD (TYPE, Y);				\
      int xy_cmp = !0;							\
      if (x_ && y_)							\
	{								\
	  xy_cmp = strcmp (x_, y_);					\
	  if (xy_cmp)							\
	    printf (#TYPE " %s = %s;\n"					\
		    #TYPE " %s = %s;\n", &#X[1], x_, &#Y[1], y_);	\
	}								\
      if (x_)								\
	MR_FREE (x_);							\
      if (y_)								\
	MR_FREE (y_);							\
      xy_cmp;								\
    })

#define MR_IS__EQ__ 0

#define ASSERT_SAVE_LOAD(METHOD, TYPE, X, ...)		\
  MR_IF_ELSE (MR_PASTE2 (SKIP_METHOD_, METHOD))		\
  (ASSERT_SAVE_LOAD_(METHOD, TYPE, X, __VA_ARGS__)) (memcmp (X, X, 1))

#define ASSERT_SAVE_LOAD_(METHOD, TYPE, X, TYPE_CMP, ...)		\
  MR_IF_ELSE (MR_PASTE3 (MR_IS__EQ_, TYPE_CMP, _))			\
  (ASSERT_SAVE_LOAD__ (METHOD, TYPE, X, TYPE_CMP, __VA_ARGS__))		\
  (ASSERT_SAVE_LOAD__ (METHOD, TYPE, X, CMP_SERIALIAZED, __VA_ARGS__))

#define ASSERT_SAVE_LOAD__(METHOD, TYPE, X, TYPE_CMP, ...) ({		\
      mr_rarray_t serialized = MR_SAVE_ ## METHOD ## _RA (TYPE, X);	\
      int orig_eq_restored;						\
      TYPE METHOD ## _restored;						\
      if (0 == MR_LOAD_ ## METHOD ## _RA (TYPE, &serialized, &METHOD ## _restored)) \
	ck_abort_msg ("load for method " #METHOD " on type " #TYPE " failed"); \
      if (serialized.data)						\
	MR_FREE (serialized.data);					\
      orig_eq_restored = (0 == TYPE_CMP (TYPE, X, &METHOD ## _restored, __VA_ARGS__)); \
      ck_assert_msg (orig_eq_restored,					\
		     "restored value mismatched original for method " #METHOD " on type " #TYPE); \
      MR_FREE_RECURSIVELY (TYPE, &METHOD ## _restored);			\
    })

#define ASSERT_SAVE_LOAD_TYPE(METHOD, TYPE, VALUE, ...) ({		\
      TYPE x = { VALUE };						\
      ASSERT_SAVE_LOAD (METHOD, TYPE, &x, __VA_ARGS__);			\
    })

#define ASSERT_GET_TEST(TEST, ...) TEST
#define ASSERT_GET_ARGS(TEST, ...) __VA_ARGS__
#define ASSERT_ITERATOR(ARGS, METHOD, I) ASSERT_ITERATOR_ (ASSERT_GET_TEST ARGS, METHOD, ASSERT_GET_ARGS ARGS)
#define ASSERT_ITERATOR_(TEST, METHOD, ...) TEST (METHOD, __VA_ARGS__)
#define SERIAL(NAME, I, REC, X) REC; X
#define ALL_METHODS(...) MR_FOR ((__VA_ARGS__), MR_NARG (TEST_METHODS), SERIAL, ASSERT_ITERATOR, TEST_METHODS)

#define SCALAR_CMP(TYPE, X, Y, ...) (*(X) != *(Y))
#define STRUCT_X_CMP(TYPE, X, Y, ...) ((X)->x != (Y)->x)

#define MAIN(...)							\
  Suite * suite = NULL;							\
  int main (int argc, char * argv[])					\
  {									\
    int number_failed;							\
    SRunner * srunner = srunner_create (suite);				\
    __VA_ARGS__;							\
    srunner_run_all (srunner, CK_ENV);					\
    number_failed = srunner_ntests_failed (srunner);			\
    srunner_free (srunner);						\
    return ((number_failed == 0) ? EXIT_SUCCESS : EXIT_FAILURE);	\
  }									\
  
#endif /* _REGRESSION_H_ */
