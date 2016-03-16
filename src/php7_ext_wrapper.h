/* php7 extension wrapper
 * for compatibility, these wrapper functions are designed for old php version.
 */

#ifndef _PHP7_EXT_WRAPPER_H
#define _PHP7_EXT_WRAPPER_H

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include <php.h>

#ifdef ZTS
#include "TSRM.h"
#endif

#include <SAPI.h>
#include <php_ini.h>

#if PHP_MAJOR_VERSION < 7
typedef int zend_size_t;
#define ZEND_RETURN_STRING(s, dup) RETURN_STRING(s, dup)
#define ZEND_RETURN_STRINGL(s, l, dup) RETURN_STRINGL(s, l, dup)
#define ZEND_RETURN_STRINGL_CALLBACK(s, l, callback)  \
	do { \
		RETVAL_STRINGL(s, l, 1);  \
		callback(s); /* generally for free the pointer */   \
		return;   \
	} while (0)

#define ZEND_TYPE_OF(z)  (z)->type
#define ZEND_IS_BOOL(z) (ZEND_TYPE_OF(z) == IS_BOOL)
#define ZEND_IS_TRUE(z) ((z)->value.lval != 0)
#define Z_CE_P(z)  ((zend_class_entry *)(z))

//#define zend_get_object_wrapper(obj) zend_object_store_get_object(obj)

#define zend_hash_update_wrapper   zend_hash_update
#define zend_call_user_function_wrapper call_user_function

#define zend_add_assoc_long_ex(z, key, key_len, n) \
	add_assoc_long_ex(z, key, key_len, n)

#define zend_add_assoc_stringl_ex(z, key, key_len, str, length, dup) \
	add_assoc_stringl_ex(z, key, key_len, str, length, dup)

#define zend_add_assoc_zval_ex(z, key, key_len, value) \
	add_assoc_zval_ex(z, key, key_len, value)

#define zend_add_assoc_bool_ex(z, key, key_len, b) \
	add_assoc_bool_ex(z, key, key_len, b)

#define zend_add_index_stringl(z, index, value, length, dup) \
	add_index_stringl(z, index, value, length, dup)

#define zend_add_index_string(z, index, value, dup) \
	add_index_string(z, index, value, dup)

#define zend_add_assoc_stringl(z, key, str, length, dup) \
	add_assoc_stringl(z, key, str, length, dup)

#define zend_add_assoc_string(z, key, str, dup) \
	add_assoc_string(z, key, str, dup)

#define zend_add_next_index_stringl(z, str, length, dup) \
	add_next_index_stringl(z, str, length, dup)

#define zend_add_next_index_string(z, str, dup) \
	add_next_index_string(z, str, dup)

static inline int zend_hash_find_wrapper(HashTable *ht, char *key, int key_len,
        zval **value)
{
	zval **pp;

	pp = NULL;
	if (zend_hash_find(ht, key, key_len, (void **)&pp) == SUCCESS)
	{
		*value = *pp;
		return SUCCESS;
	}
	else
	{
		*value = NULL;
		return FAILURE;
	}
}

static inline int zend_hash_index_find_wrapper(HashTable *ht, int index, zval **value)
{
    zval **pp = NULL;
    if (zend_hash_index_find(ht, index, (void**)&pp) == SUCCESS) {
        *value =  *pp;
        return SUCCESS;
    } else {
        *value = NULL;
        return FAILURE;
    }
}

static inline int zend_get_configuration_directive_wrapper(char *name, int len,
        zval **value)
{
	return zend_get_configuration_directive(name, len, *value);
}

#else

typedef size_t zend_size_t;
#define ZEND_RETURN_STRING(s, dup) RETURN_STRING(s)
#define ZEND_RETURN_STRINGL(s, l, dup) RETURN_STRINGL(s, l)
#define ZEND_RETURN_STRINGL_CALLBACK(s, l, callback)  \
	do { \
		RETVAL_STRINGL(s, l);  \
		callback(s);    \
		return;   \
	} while (0)

#define ZEND_TYPE_OF(z)  Z_TYPE_P(z)
#define ZEND_IS_BOOL(z) (Z_TYPE_P(z) == IS_TRUE || Z_TYPE_P(z) == IS_FALSE)
#define ZEND_IS_TRUE(z) (Z_TYPE_P(z) == IS_TRUE)
#define Z_STRVAL_PP(s)   Z_STRVAL_P(*s)
#define Z_STRLEN_PP(s)   Z_STRLEN_P(*s)
//#define zend_get_object_wrapper(obj) (void *)((char *)(Z_OBJ_P(obj)) - XtOffsetOf(php_fdfs_t, zo))

#define MAKE_STD_ZVAL(p) zval _stack_zval_##p; p = &(_stack_zval_##p)
#define ALLOC_INIT_ZVAL(p) MAKE_STD_ZVAL(p)
#define INIT_ZVAL(z)

#define zend_add_assoc_long_ex(z, key, key_len, n) \
	add_assoc_long_ex(z, key, key_len - 1, n)

#define zend_add_assoc_stringl_ex(z, key, key_len, str, length, dup) \
	add_assoc_stringl_ex(z, key, key_len - 1, str, length)

#define zend_add_assoc_zval_ex(z, key, key_len, value) \
	add_assoc_zval_ex(z, key, key_len - 1, value)

#define zend_add_assoc_bool_ex(z, key, key_len, b) \
	add_assoc_bool_ex(z, key, key_len - 1, b)

#define zend_add_index_stringl(z, index, value, length, dup) \
	add_index_stringl(z, index, value, length)

#define zend_add_index_string(z, index, value, dup) \
	add_index_string(z, index, value)

#define zend_add_assoc_stringl(z, key, str, length, dup) \
	add_assoc_stringl(z, key, str, length)

#define zend_add_assoc_string(z, key, str, dup) \
	add_assoc_string(z, key, str)

#define zend_add_next_index_stringl(z, str, length, dup) \
	add_next_index_stringl(z, str, length)

#define zend_add_next_index_string(z, str, dup) \
	add_next_index_string(z, str)

static inline int zend_hash_find_wrapper(HashTable *ht, char *key, int key_len,
        zval **value)
{
	zval zkey;
	ZVAL_STRINGL(&zkey, key, key_len - 1);
	*value = zend_hash_find(ht, Z_STR(zkey));
	return (*value != NULL ? SUCCESS : FAILURE);
}
static inline int zend_hash_index_find_wrapper(HashTable *ht, int index,
        zval **value)
{
    *value = zend_hash_index_find(ht, index);
	return (*value != NULL ? SUCCESS : FAILURE);
}
static inline int zend_hash_update_wrapper(HashTable *ht, char *k, int len,
        void *val, int size, void *ptr)
{
	zval key;
	ZVAL_STRINGL(&key, k, len - 1);
	return zend_hash_update(ht, Z_STR(key), val) ? SUCCESS : FAILURE;
}

static inline int zend_call_user_function_wrapper(HashTable *function_table,
        zval *object, zval *function_name, zval *retval_ptr,
        uint32_t param_count, zval **params TSRMLS_DC)
{
	int i;
	zval real_params[64];

	if (param_count > 64)
	{
		return FAILURE;
	}

	for(i=0; i<param_count; i++)
	{
		real_params[i] = *params[i];
	}
	return call_user_function(function_table, object, function_name, retval_ptr,
			param_count, real_params);
}

static inline int zend_get_configuration_directive_wrapper(char *name, int len,
        zval **value)
{
	zval key;
	ZVAL_STRINGL(&key, name, len - 1);
	*value = zend_get_configuration_directive(Z_STR(key));
	return (*value != NULL ? SUCCESS : FAILURE);
}

#endif

#endif
