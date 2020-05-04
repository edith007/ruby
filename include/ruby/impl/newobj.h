#ifndef RUBY3_NEWOBJ_H                               /*-*-C++-*-vi:se ft=cpp:*/
#define RUBY3_NEWOBJ_H
/**
 * @file
 * @author     Ruby developers <ruby-core@ruby-lang.org>
 * @copyright  This  file  is   a  part  of  the   programming  language  Ruby.
 *             Permission  is hereby  granted,  to  either redistribute  and/or
 *             modify this file, provided that  the conditions mentioned in the
 *             file COPYING are met.  Consult the file for details.
 * @warning    Symbols   prefixed   with   either  `RUBY3`   or   `ruby3`   are
 *             implementation details.   Don't take  them as canon.  They could
 *             rapidly appear then vanish.  The name (path) of this header file
 *             is also an  implementation detail.  Do not expect  it to persist
 *             at the place it is now.  Developers are free to move it anywhere
 *             anytime at will.
 * @note       To  ruby-core:  remember  that   this  header  can  be  possibly
 *             recursively included  from extension  libraries written  in C++.
 *             Do not  expect for  instance `__VA_ARGS__` is  always available.
 *             We assume C99  for ruby itself but we don't  assume languages of
 *             extension libraries. They could be written in C++98.
 * @brief      Defines #NEWOBJ.
 */
#include "ruby/impl/cast.h"
#include "ruby/impl/core/rbasic.h"
#include "ruby/impl/dllexport.h"
#include "ruby/impl/fl_type.h"
#include "ruby/impl/special_consts.h"
#include "ruby/impl/value.h"
#include "ruby/assert.h"

#define RB_NEWOBJ(obj,type) type *(obj) = RUBY3_CAST((type *)rb_newobj())
#define RB_NEWOBJ_OF(obj,type,klass,flags) type *(obj) = RUBY3_CAST((type *)rb_newobj_of(klass, flags))

#define NEWOBJ     RB_NEWOBJ
#define NEWOBJ_OF  RB_NEWOBJ_OF /* core has special NEWOBJ_OF() in internal.h */
#define OBJSETUP   rb_obj_setup /* use NEWOBJ_OF instead of NEWOBJ()+OBJSETUP() */
#define CLONESETUP rb_clone_setup
#define DUPSETUP   rb_dup_setup

RUBY3_SYMBOL_EXPORT_BEGIN()
VALUE rb_newobj(void);
VALUE rb_newobj_of(VALUE, VALUE);
VALUE rb_obj_setup(VALUE obj, VALUE klass, VALUE type);
VALUE rb_obj_class(VALUE);
VALUE rb_singleton_class_clone(VALUE);
void rb_singleton_class_attached(VALUE,VALUE);
void rb_copy_generic_ivar(VALUE,VALUE);
RUBY3_SYMBOL_EXPORT_END()

static inline void
rb_clone_setup(VALUE clone, VALUE obj)
{
    RUBY3_ASSERT_OR_ASSUME(! RB_SPECIAL_CONST_P(obj));
    RUBY3_ASSERT_OR_ASSUME(! RB_SPECIAL_CONST_P(clone));

    const VALUE flags = RUBY_FL_PROMOTED0 | RUBY_FL_PROMOTED1 | RUBY_FL_FINALIZE;
    rb_obj_setup(clone, rb_singleton_class_clone(obj),
                 RB_FL_TEST_RAW(obj, ~flags));
    rb_singleton_class_attached(RBASIC_CLASS(clone), clone);
    if (RB_FL_TEST(obj, RUBY_FL_EXIVAR)) rb_copy_generic_ivar(clone, obj);
}

static inline void
rb_dup_setup(VALUE dup, VALUE obj)
{
    RUBY3_ASSERT_OR_ASSUME(! RB_SPECIAL_CONST_P(obj));
    RUBY3_ASSERT_OR_ASSUME(! RB_SPECIAL_CONST_P(dup));

    rb_obj_setup(dup, rb_obj_class(obj), RB_FL_TEST_RAW(obj, RUBY_FL_DUPPED));
    if (RB_FL_TEST(obj, RUBY_FL_EXIVAR)) rb_copy_generic_ivar(dup, obj);
}

#endif /* RUBY3_NEWOBJ_H */
