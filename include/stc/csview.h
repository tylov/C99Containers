/* MIT License
 *
 * Copyright (c) 2021 Tyge Løvset, NORCE, www.norceresearch.no
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */
#ifndef CSVIEW_H_INCLUDED
#define CSVIEW_H_INCLUDED

#include "cstr.h"

typedef                 struct { const char* str; size_t size; } csview;
typedef                 struct { const char *ref; } csview_iter_t;
typedef                 char csview_value_t;

#define                 csview_null  c_make(csview){"", 0}
#define                 csview_npos  cstr_npos
#define                 csview_ARG(sv)  (int)(sv).size, (sv).str

#define                 c_sv(literal) csview_lit(literal)
#define                 cstr_sv(s) csview_from_s(s)

STC_API csview          csview_substr(csview sv, intptr_t pos, size_t n);
STC_API csview          csview_slice(csview sv, intptr_t p1, intptr_t p2);
STC_API csview          csview_first_token(csview sv, csview sep);
STC_API csview          csview_next_token(csview sv, csview sep, csview tok);

#define                 csview_lit(literal) \
                            c_make(csview){literal, sizeof c_make(strlit_t){literal} - 1}
STC_INLINE csview       csview_from(const char* str)
                            { return c_make(csview){str, strlen(str)}; }
STC_INLINE csview       csview_from_n(const char* str, size_t n)
                            { return c_make(csview){str, n}; }
STC_INLINE csview       csview_from_s(cstr s)
                            { return c_make(csview){s.str, _cstr_rep(&s)->size}; }
STC_INLINE size_t       csview_size(csview sv) { return sv.size; }
STC_INLINE size_t       csview_length(csview sv) { return sv.size; }
STC_INLINE bool         csview_empty(csview sv) { return sv.size == 0; }
STC_INLINE char         csview_front(csview sv) { return sv.str[0]; }
STC_INLINE char         csview_back(csview sv) { return sv.str[sv.size - 1]; }

STC_INLINE void         csview_clear(csview* self) { *self = csview_null; }

#define                 csview_hash(sv)  c_default_hash((sv).str, (sv).size)
STC_INLINE bool         csview_equals(csview sv, csview sv2)
                            { return sv.size == sv2.size && !memcmp(sv.str, sv2.str, sv.size); }
STC_INLINE size_t       csview_find(csview sv, csview needle)
                            { char* res = c_strnstrn(sv.str, needle.str, sv.size, needle.size);
                              return res ? res - sv.str : cstr_npos; }
STC_INLINE bool         csview_contains(csview sv, csview needle)
                            { return c_strnstrn(sv.str, needle.str, sv.size, needle.size) != NULL; }
STC_INLINE bool         csview_starts_with(csview sv, csview sub)
                            { if (sub.size > sv.size) return false;
                              return !memcmp(sv.str, sub.str, sub.size); }
STC_INLINE bool         csview_ends_with(csview sv, csview sub)
                            { if (sub.size > sv.size) return false;
                              return !memcmp(sv.str + sv.size - sub.size, sub.str, sub.size); }
STC_INLINE csview_iter_t csview_begin(const csview* self)
                            { return c_make(csview_iter_t){self->str}; }
STC_INLINE csview_iter_t csview_end(const csview* self)
                            { return c_make(csview_iter_t){self->str + self->size}; }
STC_INLINE void          csview_next(csview_iter_t* it) { ++it->ref; }


/* cstr interaction with csview: */

STC_API    cstr         cstr_from_replace_all(csview sv, csview find, csview replace);

STC_INLINE cstr         cstr_from_v(csview sv)
                            { return cstr_from_n(sv.str, sv.size); }
STC_INLINE csview       cstr_to_v(const cstr* self)
                            { return c_make(csview){self->str, _cstr_rep(self)->size}; }
STC_INLINE csview       cstr_substr(cstr s, intptr_t pos, size_t n) 
                            { return csview_substr(csview_from_s(s), pos, n); }
STC_INLINE csview       cstr_slice(cstr s, intptr_t p1, intptr_t p2)
                            { return csview_slice(csview_from_s(s), p1, p2); }
STC_INLINE cstr*        cstr_assign_v(cstr* self, csview sv)
                            { return cstr_assign_n(self, sv.str, sv.size); }
STC_INLINE cstr*        cstr_append_v(cstr* self, csview sv)
                            { return cstr_append_n(self, sv.str, sv.size); }
STC_INLINE void         cstr_insert_v(cstr* self, size_t pos, csview sv)
                            { cstr_replace_n(self, pos, 0, sv.str, sv.size); }
STC_INLINE void         cstr_replace_v(cstr* self, size_t pos, size_t len, csview sv)
                            { cstr_replace_n(self, pos, len, sv.str, sv.size); }
STC_INLINE void         cstr_replace_all_v(cstr* self, csview find, csview replace)
                            { cstr_take(self, cstr_from_replace_all(cstr_sv(*self), find, replace)); }
STC_INLINE bool         cstr_equals_v(cstr s, csview sv)
                            { return sv.size == cstr_size(s) && !memcmp(s.str, sv.str, sv.size); }
STC_INLINE size_t       cstr_find_v(cstr s, csview needle)
                            { char* res = c_strnstrn(s.str, needle.str, cstr_size(s), needle.size);
                              return res ? res - s.str : cstr_npos; }
STC_INLINE bool         cstr_contains_v(cstr s, csview needle)
                            { return c_strnstrn(s.str, needle.str, cstr_size(s), needle.size) != NULL; }
STC_INLINE bool         cstr_starts_with_v(cstr s, csview sub)
                            { if (sub.size > cstr_size(s)) return false;
                              return !memcmp(s.str, sub.str, sub.size); }
STC_INLINE bool         cstr_ends_with_v(cstr s, csview sub)
                            { if (sub.size > cstr_size(s)) return false;
                              return !memcmp(s.str + cstr_size(s) - sub.size, sub.str, sub.size); }

/* ---- Adaptor functions ---- */

#define                 csview_compare_ref(xp, yp)  strcmp((xp)->str, (yp)->str)
STC_INLINE bool         csview_equals_ref(const csview* a, const csview* b)
                            { return a->size == b->size && !memcmp(a->str, b->str, a->size); }
#define                 csview_hash_ref(xp, none)  c_default_hash((xp)->str, (xp)->size)

/* ---- Associative cstr-containers with csview emplace/lookup API ---- */

#define using_csmap_strvkey(...) c_MACRO_OVERLOAD(using_csmap_strvkey, __VA_ARGS__)

#define using_csmap_strvkey_2(X, Mapped) \
            using_csmap_strvkey_4(X, Mapped, c_default_del, c_default_fromraw)
#define using_csmap_strvkey_3(X, Mapped, mappedDel) \
            using_csmap_strvkey_4(X, Mapped, mappedDel, c_no_clone)
#define using_csmap_strvkey_4(X, Mapped, mappedDel, mappedClone) \
            using_csmap_strvkey_7(X, Mapped, mappedDel, mappedClone, c_default_toraw, Mapped, c_true)
#define using_csmap_strvkey_7(X, Mapped, mappedDel, mappedFromRaw, mappedToRaw, RawMapped, defTypes) \
            _c_using_aatree(csmap_##X, csmap_, cstr, Mapped, csview_compare_ref, \
                            mappedDel, mappedFromRaw, mappedToRaw, RawMapped, defTypes, \
                            cstr_del, cstr_from_v, cstr_to_v, csview)

#define using_csmap_strv() \
            _c_using_aatree(csmap_strv, csmap_, cstr, cstr, csview_compare_ref, \
                           cstr_del, cstr_from_v, cstr_to_v, csview, c_true, \
                           cstr_del, cstr_from_v, cstr_to_v, csview)
#define using_csset_strv() \
            _c_using_aatree(csset_strv, csset_, cstr, cstr, csview_compare_ref, \
                            @@, @@, @@, void, c_true, cstr_del, cstr_from_v, cstr_to_v, csview)


#define using_cmap_strvkey(...) c_MACRO_OVERLOAD(using_cmap_strvkey, __VA_ARGS__)

#define using_cmap_strvkey_2(X, Mapped) \
            using_cmap_strvkey_4(X, Mapped, c_default_del, c_default_fromraw)
#define using_cmap_strvkey_3(X, Mapped, mappedDel) \
            using_cmap_strvkey_4(X, Mapped, mappedDel, c_no_clone)
#define using_cmap_strvkey_4(X, Mapped, mappedDel, mappedClone) \
            using_cmap_strvkey_7(X, Mapped, mappedDel, mappedClone, c_default_toraw, Mapped, c_true)
#define using_cmap_strvkey_7(X, Mapped, mappedDel, mappedFromRaw, mappedToRaw, RawMapped, defTypes) \
            _c_using_chash(cmap_##X, cmap_, cstr, Mapped, csview_equals_ref, csview_hash_ref, \
                           mappedDel, mappedFromRaw, mappedToRaw, RawMapped, defTypes, \
                           cstr_del, cstr_from_v, cstr_to_v, csview)

#define using_cmap_strv() \
            _c_using_chash(cmap_strv, cmap_, cstr, cstr, csview_equals_ref, csview_hash_ref, \
                           cstr_del, cstr_from_v, cstr_to_v, csview, c_true, \
                           cstr_del, cstr_from_v, cstr_to_v, csview)
#define using_cset_strv() \
            _c_using_chash(cset_strv, cset_, cstr, cstr, csview_equals_ref, csview_hash_ref, \
                           @@, @@, @@, void, c_true, cstr_del, cstr_from_v, cstr_to_v, csview)

/* -------------------------- IMPLEMENTATION ------------------------- */

#if !defined(STC_HEADER) || defined(STC_IMPLEMENTATION)

STC_DEF csview
csview_substr(csview sv, intptr_t pos, size_t n) {
    if (pos < 0) { pos += sv.size; if (pos < 0) pos = 0; }
    if (pos > sv.size) pos = sv.size; if (pos + n > sv.size) n = sv.size - pos;
    sv.str += pos, sv.size = n; return sv;
}

STC_DEF csview
csview_slice(csview sv, intptr_t p1, intptr_t p2) {
    if (p1 < 0) { p1 += sv.size; if (p1 < 0) p1 = 0; }
    if (p2 < 0) p2 += sv.size; if (p2 > sv.size) p2 = sv.size;
    sv.str += p1, sv.size = p2 > p1 ? p2 - p1 : 0; return sv; 
}

STC_DEF csview
csview_first_token(csview sv, csview sep) {
    const char* res = c_strnstrn(sv.str, sep.str, sv.size, sep.size); 
    return c_make(csview){sv.str, (res ? res - sv.str : sv.size)};
}

STC_DEF csview
csview_next_token(csview sv, csview sep, csview tok) {
    if (&tok.str[tok.size] == &sv.str[sv.size])
        return c_make(csview){&sv.str[sv.size], 0};
    tok.str += tok.size + sep.size;
    size_t n = sv.size - (tok.str - sv.str);
    const char* res = c_strnstrn(tok.str, sep.str, n, sep.size);
    tok.size = res ? res - tok.str : n;
    return tok;
}

STC_DEF cstr
cstr_from_replace_all(csview sv, csview find, csview replace) {
    cstr out = cstr_null;
    size_t from = 0, pos; char* res;
    if (find.size)
        while ((res = c_strnstrn(sv.str + from, find.str, sv.size - from, find.size))) {
            pos = res - sv.str;
            cstr_append_n(&out, sv.str + from, pos - from );
            cstr_append_v(&out, replace);
            from = pos + find.size;
        }
    cstr_append_n(&out, sv.str + from, sv.size - from);
    return out;
}

#endif
#endif