#include "minixml.h"
char djy_getc (djyfs_file *fp)
{
	char ch;
	djyfs_fread((void*)&ch, (ptu32_t)1, (ptu32_t)1, fp);
	return ch;
}
u32 djy_putc (char ch, djyfs_file *fp)
{
	return djyfs_fwrite((void*)&ch, (ptu32_t)1, (ptu32_t)1, fp);
}
void *m_realloc (void *before, u32 size)
{
	void *result = NULL;

	result = m_malloc(size, 0);
	if (result == NULL)
	{
		return NULL;
	}
	memcpy(result, before, size); //这里处理的不妥啊
	m_free(before);
	return result;
}
//移植到djyos,要用到的类型 
//djy_getc
//djy_putc
//#include "djyos_transplant.h"

/************** Begin file config.h ***************************************/
/*
 * "$Id: config.h.in,v 1.3 2003/09/28 12:44:39 mike Exp $"
 *
 * Configuration file for mini-XML, a small XML-like file parsing library.
 *
 * Copyright 2003 by Michael Sweet.
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Library General Public
 * License as published by the Free Software Foundation; either
 * version 2, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 */

/*
 * Include necessary headers...
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#include <ctype.h>

/*
 * Do we have the snprintf() and vsnprintf() functions?
 */

#undef HAVE_SNPRINTF
#undef HAVE_VSNPRINTF

/*
 * Do we have the strXXX() functions?
 */

#undef HAVE_STRDUP

/*
 * Define prototypes for string functions as needed...
 */

#  ifndef HAVE_STRDUP
char *mxml_strdup(const char *);
#    define strdup mxml_strdup
#  endif /* !HAVE_STRDUP */

char *mxml_strdupf(const char * , va_list);

#  ifndef HAVE_VSNPRINTF
int mxml_vsnprintf(char * , size_t , const char * , va_list);
#    define vsnprintf mxml_vsnprintf
#  endif /* !HAVE_VSNPRINTF */

/*
 * End of "$Id: config.h.in,v 1.3 2003/09/28 12:44:39 mike Exp $".
 */
/************** End of config.h *****************************************/
/************** Begin file mxml.h ***************************************/
/*
 * "$Id: mxml.h,v 1.16 2003/12/18 04:16:37 mike Exp $"
 *
 * Header file for mini-XML, a small XML-like file parsing library.
 *
 * Copyright 2003 by Michael Sweet.
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Library General Public
 * License as published by the Free Software Foundation; either
 * version 2, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 */

/*
 * Prevent multiple inclusion...
 */

#ifndef _mxml_h_
#  define _mxml_h_

/*
 * Include necessary headers...
 */

#  include <stdio.h>
#  include <stdlib.h>
#  include <string.h>
#  include <ctype.h>
#  include <errno.h>

/*
 * Constants...
 */

#  define MXML_WRAP		72	/* Wrap XML output at this column position */
#  define MXML_TAB		8	/* Tabs every N columns */

#  define MXML_NO_CALLBACK	0	/* Don't use a type callback */
#  define MXML_INTEGER_CALLBACK	mxml_integer_cb
/* Treat all data as integers */
#  define MXML_OPAQUE_CALLBACK	mxml_opaque_cb
/* Treat all data as opaque */
#  define MXML_REAL_CALLBACK	mxml_real_cb
/* Treat all data as real numbers */
#  define MXML_TEXT_CALLBACK	0	/* Treat all data as text */

#  define MXML_NO_PARENT	0	/* No parent for the node */

#  define MXML_DESCEND		1	/* Descend when finding/walking */
#  define MXML_NO_DESCEND	0	/* Don't descend when finding/walking */
#  define MXML_DESCEND_FIRST	-1	/* Descend for first find */

#  define MXML_WS_BEFORE_OPEN	0	/* Callback for before open tag */
#  define MXML_WS_AFTER_OPEN	1	/* Callback for after open tag */
#  define MXML_WS_BEFORE_CLOSE	2	/* Callback for before close tag */
#  define MXML_WS_AFTER_CLOSE	3	/* Callback for after close tag */

#  define MXML_ADD_BEFORE	0	/* Add node before specified node */
#  define MXML_ADD_AFTER	1	/* Add node after specified node */
#  define MXML_ADD_TO_PARENT	NULL	/* Add node relative to parent */

/*
 * Data types...
 */

typedef enum mxml_type_e /**** The XML node type. ****/
{
	MXML_ELEMENT, /* XML element with attributes */
	MXML_INTEGER, /* Integer value */
	MXML_OPAQUE, /* Opaque string */
	MXML_REAL, /* Real value */
	MXML_TEXT /* Text fragment */
} mxml_type_t;

typedef struct mxml_attr_s /**** An XML element attribute value. ****/
{
	char *name; /* Attribute name */
	char *value; /* Attribute value */
} mxml_attr_t;

typedef struct mxml_value_s /**** An XML element value. ****/
{
	char *name; /* Name of element */
	int num_attrs; /* Number of attributes */
	mxml_attr_t *attrs; /* Attributes */
} mxml_element_t;

typedef struct mxml_text_s /**** An XML text value. ****/
{
	int whitespace; /* Leading whitespace? */
	char *string; /* Fragment string */
} mxml_text_t;

typedef union mxml_value_u /**** An XML node value. ****/
{
	mxml_element_t element; /* Element */
	int integer; /* Integer number */
	char *opaque; /* Opaque string */
	double real; /* Real number */
	mxml_text_t text; /* Text fragment */
} mxml_value_t;

typedef struct mxml_node_s /**** An XML node. ****/
{
	mxml_type_t type; /* Node type */
	struct mxml_node_s *next; /* Next node under same parent */
	struct mxml_node_s *prev; /* Previous node under same parent */
	struct mxml_node_s *parent; /* Parent node */
	struct mxml_node_s *child; /* First child node */
	struct mxml_node_s *last_child; /* Last child node */
	mxml_value_t value; /* Node value */
} mxml_node_t;

/*
 * C++ support...
 */

#  ifdef __cplusplus
"C"
{
#  endif /* __cplusplus */

/*
 * Prototypes...
 */

void mxmlAdd(mxml_node_t *parent , int where , mxml_node_t *child , mxml_node_t *node);
void mxmlDelete(mxml_node_t *node);
const char *mxmlElementGetAttr(mxml_node_t *node , const char *name);
void mxmlElementSetAttr(mxml_node_t *node , const char *name , const char *value);
const char *mxmlEntityGetName(int val);
int mxmlEntityGetValue(const char *name);
mxml_node_t *mxmlFindElement(mxml_node_t *node , mxml_node_t *top , const char *name , const char *attr ,
		const char *value , int descend);
mxml_node_t *mxmlLoadFile(mxml_node_t *top , djyfs_file *fp , mxml_type_t (*cb)(mxml_node_t *));
mxml_node_t *mxmlLoadString(mxml_node_t *top , const char *s , mxml_type_t (*cb)(mxml_node_t *));
mxml_node_t *mxmlNewElement(mxml_node_t *parent , const char *name);
mxml_node_t *mxmlNewInteger(mxml_node_t *parent , int integer);
mxml_node_t *mxmlNewOpaque(mxml_node_t *parent , const char *opaque);
mxml_node_t *mxmlNewReal(mxml_node_t *parent , double real);
mxml_node_t *mxmlNewText(mxml_node_t *parent , int whitespace , const char *string);
mxml_node_t *mxmlNewTextf(mxml_node_t *parent , int whitespace , const char *format , ...)
#    ifdef __GNUC__
__attribute__ ((__format__ (__printf__, 3, 4)))
#    endif /* __GNUC__ */
;
void mxmlRemove(mxml_node_t *node);
char *mxmlSaveAllocString(mxml_node_t *node , int (*cb)(mxml_node_t * , int));
int mxmlSaveFile(mxml_node_t *node , djyfs_file *fp , int (*cb)(mxml_node_t * , int));
int mxmlSaveString(mxml_node_t *node , char *buffer , int bufsize , int (*cb)(mxml_node_t * , int));
int mxmlSetElement(mxml_node_t *node , const char *name);
void mxmlSetErrorCallback(void (*cb)(const char *));
int mxmlSetInteger(mxml_node_t *node , int integer);
int mxmlSetOpaque(mxml_node_t *node , const char *opaque);
int mxmlSetReal(mxml_node_t *node , double real);
int mxmlSetText(mxml_node_t *node , int whitespace , const char *string);
int mxmlSetTextf(mxml_node_t *node , int whitespace , const char *format , ...)
#    ifdef __GNUC__
__attribute__ ((__format__ (__printf__, 3, 4)))
#    endif /* __GNUC__ */
;
mxml_node_t *mxmlWalkNext(mxml_node_t *node , mxml_node_t *top , int descend);
mxml_node_t *mxmlWalkPrev(mxml_node_t *node , mxml_node_t *top , int descend);

/*
 * Private functions...
 */
//下面四个函数为用户自定义函数,所以用extern修饰 
void mxml_error(const char *format , ...);
mxml_type_t mxml_integer_cb(mxml_node_t *node);
mxml_type_t mxml_opaque_cb(mxml_node_t *node);
mxml_type_t mxml_real_cb(mxml_node_t *node);

/*
 * C++ support...
 */

#  ifdef __cplusplus
}
#  endif /* __cplusplus */
#endif /* !_mxml_h_ */

/*
 * End of "$Id: mxml.h,v 1.16 2003/12/18 04:16:37 mike Exp $".
 */
/************** End of mxml.h *****************************************/
/************** Begin file mxml-attr.c ***************************************/
/*
 * "$Id: mxml-attr.c,v 1.6 2003/12/03 03:59:04 mike Exp $"
 *
 * Attribute support code for mini-XML, a small XML-like file parsing library.
 *
 * Copyright 2003 by Michael Sweet.
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Library General Public
 * License as published by the Free Software Foundation; either
 * version 2, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * Contents:
 *
 *   mxmlElementGetAttr() - Get an attribute.
 *   mxmlElementSetAttr() - Set an attribute.
 */

/*
 * Include necessary headers...
 */

//#include "config.h"
//#include "mxml.h"

/*
 * 'mxmlElementGetAttr()' - Get an attribute.
 *
 * This function returns NULL if the node is not an element or the
 * named attribute does not exist.
 */

const char * /* O - Attribute value or NULL */
mxmlElementGetAttr(mxml_node_t *node , /* I - Element node */
const char *name) /* I - Name of attribute */
{
	int i; /* Looping var */
	mxml_attr_t *attr; /* Current attribute */

#ifdef MINIXML_DEBUG
	djy_printf("mxmlElementGetAttr(node=%p, name=\"%s\")\r\n",
			node, name ? name : "(null)");
#endif /* DEBUG */

	/*
	 * Range check input...
	 */

	if (!node || node->type != MXML_ELEMENT || !name)
		return (NULL);

	/*
	 * Look for the attribute...
	 */

	for( i = node->value.element.num_attrs, attr = node->value.element.attrs; i > 0 ; i--, attr++ )
		if (!strcmp(attr->name , name))
			return (attr->value);

	/*
	 * Didn't find attribute, so return NULL...
	 */

	return (NULL);
}

/*
 * 'mxmlElementSetAttr()' - Set an attribute.
 *
 * If the named attribute already exists, the value of the attribute
 * is replaced by the new string value. The string value is copied
 * into the element node. This function does nothing if the node is
 * not an element.
 */

void mxmlElementSetAttr(mxml_node_t *node , /* I - Element node */
const char *name , /* I - Name of attribute */
const char *value) /* I - Attribute value */
{
	int i; /* Looping var */
	mxml_attr_t *attr; /* New attribute */

#ifdef MINIXML_DEBUG
	djy_printf("mxmlElementSetAttr(node=%p, name=\"%s\", value=\"%s\")\r\n",
			node, name ? name : "(null)", value ? value : "(null)");
#endif /* DEBUG */

	/*
	 * Range check input...
	 */

	if (!node || node->type != MXML_ELEMENT || !name)
		return;

	/*
	 * Look for the attribute...
	 */

	for( i = node->value.element.num_attrs, attr = node->value.element.attrs; i > 0 ; i--, attr++ )
		if (!strcmp(attr->name , name))
		{
			/*
			 * Replace the attribute value and return...
			 */

			m_free(attr->value);

			if (value)
				attr->value = strdup(value);
			else
				attr->value = NULL;

			return;
		}

	/*
	 * Attribute not found, so add a new one...
	 */

	if (node->value.element.num_attrs == 0)
		attr = m_malloc(sizeof(mxml_attr_t), 0);
	else
		attr = m_realloc(node->value.element.attrs , (node->value.element.num_attrs + 1) * sizeof(mxml_attr_t));

	if (!attr)
	{
		mxml_error("Unable to allocate memory for attribute '%s' in element %s!" , name , node->value.element.name);
		return;
	}

	node->value.element.attrs = attr;
	attr += node->value.element.num_attrs; //将指针移到最后一个元素 

	attr->name = strdup(name);
	if (value)
		attr->value = strdup(value);
	else
		attr->value = NULL;
	//属性名字赋值失败 或者 属性值赋值失败在value存在的情况下 
	if (!attr->name || (!attr->value && value))
	{
		if (attr->name)
			m_free(attr->name);

		if (attr->value)
			m_free(attr->value);

		mxml_error("Unable to allocate memory for attribute '%s' in element %s!" , name , node->value.element.name);

		return;
	}

	node->value.element.num_attrs++;
}

/*
 * End of "$Id: mxml-attr.c,v 1.6 2003/12/03 03:59:04 mike Exp $".
 */
/************** End of mxml-attr.c *****************************************/
/************** Begin file mxml-entity.c ***************************************/
/*
 * "$Id: mxml-entity.c,v 1.1 2003/12/18 04:16:37 mike Exp $"
 *
 * Character entity support code for mini-XML, a small XML-like
 * file parsing library.
 *
 * Copyright 2003 by Michael Sweet.
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Library General Public
 * License as published by the Free Software Foundation; either
 * version 2, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * Contents:
 *
 *   mxmlEntityGetName()  - Get the name that corresponds to the character
 *                          value.
 *   mxmlEntityGetValue() - Get the character corresponding to a named entity.
 */

/*
 * Include necessary headers...
 */

//#include "config.h"
//#include "mxml.h"

/*
 * 'mxmlEntityGetName()' - Get the name that corresponds to the character value.
 *
 * If val does not need to be represented by a named entity, NULL is returned.
 */

const char * /* O - Entity name or NULL */
mxmlEntityGetName(int val) /* I - Character value */
{
	switch (val)
	{
	case '&' :
		return ("amp");

	case '<' :
		return ("lt");

	case '>' :
		return ("gt");

	case '\"' :
		return ("quot");

	default:
		return (NULL);
	}
}

/*
 * 'mxmlEntityGetValue()' - Get the character corresponding to a named entity.
 *
 * The entity name can also be a numeric constant. -1 is returned if the
 * name is not known.
 */

int /* O - Character value or -1 on error */
mxmlEntityGetValue(const char *name) /* I - Entity name */
{
	int diff, /* Difference between names */
	current, /* Current entity in search */
	first, /* First entity in search */
	last; /* Last entity in search */
	static const struct
	{
		const char *name; /* Entity name */
		int val; /* Character value */
	} entities[] =
	{
	{ "AElig", 198 },
	{ "Aacute", 193 },
	{ "Acirc", 194 },
	{ "Agrave", 192 },
	{ "Alpha", 913 },
	{ "Aring", 197 },
	{ "Atilde", 195 },
	{ "Auml", 196 },
	{ "Beta", 914 },
	{ "Ccedil", 199 },
	{ "Chi", 935 },
	{ "Dagger", 8225 },
	{ "Delta", 916 },
	{ "Dstrok", 208 },
	{ "ETH", 208 },
	{ "Eacute", 201 },
	{ "Ecirc", 202 },
	{ "Egrave", 200 },
	{ "Epsilon", 917 },
	{ "Eta", 919 },
	{ "Euml", 203 },
	{ "Gamma", 915 },
	{ "Iacute", 205 },
	{ "Icirc", 206 },
	{ "Igrave", 204 },
	{ "Iota", 921 },
	{ "Iuml", 207 },
	{ "Kappa", 922 },
	{ "Lambda", 923 },
	{ "Mu", 924 },
	{ "Ntilde", 209 },
	{ "Nu", 925 },
	{ "OElig", 338 },
	{ "Oacute", 211 },
	{ "Ocirc", 212 },
	{ "Ograve", 210 },
	{ "Omega", 937 },
	{ "Omicron", 927 },
	{ "Oslash", 216 },
	{ "Otilde", 213 },
	{ "Ouml", 214 },
	{ "Phi", 934 },
	{ "Pi", 928 },
	{ "Prime", 8243 },
	{ "Psi", 936 },
	{ "Rho", 929 },
	{ "Scaron", 352 },
	{ "Sigma", 931 },
	{ "THORN", 222 },
	{ "Tau", 932 },
	{ "Theta", 920 },
	{ "Uacute", 218 },
	{ "Ucirc", 219 },
	{ "Ugrave", 217 },
	{ "Upsilon", 933 },
	{ "Uuml", 220 },
	{ "Xi", 926 },
	{ "Yacute", 221 },
	{ "Yuml", 376 },
	{ "Zeta", 918 },
	{ "aacute", 225 },
	{ "acirc", 226 },
	{ "acute", 180 },
	{ "aelig", 230 },
	{ "agrave", 224 },
	{ "alefsym", 8501 },
	{ "alpha", 945 },
	{ "amp", '&' },
	{ "and", 8743 },
	{ "ang", 8736 },
	{ "aring", 229 },
	{ "asymp", 8776 },
	{ "atilde", 227 },
	{ "auml", 228 },
	{ "bdquo", 8222 },
	{ "beta", 946 },
	{ "brkbar", 166 },
	{ "brvbar", 166 },
	{ "bull", 8226 },
	{ "cap", 8745 },
	{ "ccedil", 231 },
	{ "cedil", 184 },
	{ "cent", 162 },
	{ "chi", 967 },
	{ "circ", 710 },
	{ "clubs", 9827 },
	{ "cong", 8773 },
	{ "copy", 169 },
	{ "crarr", 8629 },
	{ "cup", 8746 },
	{ "curren", 164 },
	{ "dArr", 8659 },
	{ "dagger", 8224 },
	{ "darr", 8595 },
	{ "deg", 176 },
	{ "delta", 948 },
	{ "diams", 9830 },
	{ "die", 168 },
	{ "divide", 247 },
	{ "eacute", 233 },
	{ "ecirc", 234 },
	{ "egrave", 232 },
	{ "empty", 8709 },
	{ "emsp", 8195 },
	{ "ensp", 8194 },
	{ "epsilon", 949 },
	{ "equiv", 8801 },
	{ "eta", 951 },
	{ "eth", 240 },
	{ "euml", 235 },
	{ "euro", 8364 },
	{ "exist", 8707 },
	{ "fnof", 402 },
	{ "forall", 8704 },
	{ "frac12", 189 },
	{ "frac14", 188 },
	{ "frac34", 190 },
	{ "frasl", 8260 },
	{ "gamma", 947 },
	{ "ge", 8805 },
	{ "gt", '>' },
	{ "hArr", 8660 },
	{ "harr", 8596 },
	{ "hearts", 9829 },
	{ "hellip", 8230 },
	{ "hibar", 175 },
	{ "iacute", 237 },
	{ "icirc", 238 },
	{ "iexcl", 161 },
	{ "igrave", 236 },
	{ "image", 8465 },
	{ "infin", 8734 },
	{ "int", 8747 },
	{ "iota", 953 },
	{ "iquest", 191 },
	{ "isin", 8712 },
	{ "iuml", 239 },
	{ "kappa", 954 },
	{ "lArr", 8656 },
	{ "lambda", 955 },
	{ "lang", 9001 },
	{ "laquo", 171 },
	{ "larr", 8592 },
	{ "lceil", 8968 },
	{ "ldquo", 8220 },
	{ "le", 8804 },
	{ "lfloor", 8970 },
	{ "lowast", 8727 },
	{ "loz", 9674 },
	{ "lrm", 8206 },
	{ "lsaquo", 8249 },
	{ "lsquo", 8216 },
	{ "lt", '<' },
	{ "macr", 175 },
	{ "mdash", 8212 },
	{ "micro", 181 },
	{ "middot", 183 },
	{ "minus", 8722 },
	{ "mu", 956 },
	{ "nabla", 8711 },
	{ "nbsp", 160 },
	{ "ndash", 8211 },
	{ "ne", 8800 },
	{ "ni", 8715 },
	{ "not", 172 },
	{ "notin", 8713 },
	{ "nsub", 8836 },
	{ "ntilde", 241 },
	{ "nu", 957 },
	{ "oacute", 243 },
	{ "ocirc", 244 },
	{ "oelig", 339 },
	{ "ograve", 242 },
	{ "oline", 8254 },
	{ "omega", 969 },
	{ "omicron", 959 },
	{ "oplus", 8853 },
	{ "or", 8744 },
	{ "ordf", 170 },
	{ "ordm", 186 },
	{ "oslash", 248 },
	{ "otilde", 245 },
	{ "otimes", 8855 },
	{ "ouml", 246 },
	{ "para", 182 },
	{ "part", 8706 },
	{ "permil", 8240 },
	{ "perp", 8869 },
	{ "phi", 966 },
	{ "pi", 960 },
	{ "piv", 982 },
	{ "plusmn", 177 },
	{ "pound", 163 },
	{ "prime", 8242 },
	{ "prod", 8719 },
	{ "prop", 8733 },
	{ "psi", 968 },
	{ "quot", '\"' },
	{ "rArr", 8658 },
	{ "radic", 8730 },
	{ "rang", 9002 },
	{ "raquo", 187 },
	{ "rarr", 8594 },
	{ "rceil", 8969 },
	{ "rdquo", 8221 },
	{ "real", 8476 },
	{ "reg", 174 },
	{ "rfloor", 8971 },
	{ "rho", 961 },
	{ "rlm", 8207 },
	{ "rsaquo", 8250 },
	{ "rsquo", 8217 },
	{ "sbquo", 8218 },
	{ "scaron", 353 },
	{ "sdot", 8901 },
	{ "sect", 167 },
	{ "shy", 173 },
	{ "sigma", 963 },
	{ "sigmaf", 962 },
	{ "sim", 8764 },
	{ "spades", 9824 },
	{ "sub", 8834 },
	{ "sube", 8838 },
	{ "sum", 8721 },
	{ "sup", 8835 },
	{ "sup1", 185 },
	{ "sup2", 178 },
	{ "sup3", 179 },
	{ "supe", 8839 },
	{ "szlig", 223 },
	{ "tau", 964 },
	{ "there4", 8756 },
	{ "theta", 952 },
	{ "thetasym", 977 },
	{ "thinsp", 8201 },
	{ "thorn", 254 },
	{ "tilde", 732 },
	{ "times", 215 },
	{ "trade", 8482 },
	{ "uArr", 8657 },
	{ "uacute", 250 },
	{ "uarr", 8593 },
	{ "ucirc", 251 },
	{ "ugrave", 249 },
	{ "uml", 168 },
	{ "upsih", 978 },
	{ "upsilon", 965 },
	{ "uuml", 252 },
	{ "weierp", 8472 },
	{ "xi", 958 },
	{ "yacute", 253 },
	{ "yen", 165 },
	{ "yuml", 255 },
	{ "zeta", 950 },
	{ "zwj", 8205 },
	{ "zwnj", 8204 } };
	//下面是个典型的二分查找算法 
	first = 0;
	last = (int) (sizeof(entities) / sizeof(entities[0]) - 1);

	while (last > first)
	{
		current = (first + last) / 2;

		if ((diff = strcmp(name , entities[current].name)) == 0)
			return (entities[current].val);
		else
			if (diff < 0)
				last = current;
			else
				first = current;
	}

	current = (first + last) / 2;

	if (!strcmp(name , entities[current].name))
		return (entities[current].val);
	else
		return (-1);
}

/*
 * End of "$Id: mxml-entity.c,v 1.1 2003/12/18 04:16:37 mike Exp $".
 */
/************** End of mxml-entity.c *****************************************/
/************** Begin file mxml-file.c ***************************************/
/*
 * "$Id: mxml-file.c,v 1.27 2003/12/21 15:01:15 mike Exp $"
 *
 * File loading code for mini-XML, a small XML-like file parsing library.
 *
 * Copyright 2003 by Michael Sweet.
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Library General Public
 * License as published by the Free Software Foundation; either
 * version 2, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * Contents:
 *
 *   mxmlLoadFile()         - Load a file into an XML node tree.
 *   mxmlLoadString()       - Load a string into an XML node tree.
 *   mxmlSaveAllocString()  - Save an XML node tree to an allocated string.
 *   mxmlSaveFile()         - Save an XML tree to a file.
 *   mxmlSaveString()       - Save an XML node tree to a string.
 *   mxmlSetErrorCallback() - Set the error message callback.
 *   mxml_add_char()        - Add a character to a buffer, expanding as needed.
 *   mxml_get_entity()      - Get the character corresponding to an entity...
 *   mxml_file_getc()       - Get a character from a file.
 *   mxml_file_putc()       - Write a character to a file.
 *   mxml_load_data()       - Load data into an XML node tree.
 *   mxml_parse_element()   - Parse an element for any attributes...
 *   mxml_string_getc()     - Get a character from a string.
 *   mxml_write_name()      - Write a name string.
 *   mxml_write_node()      - Save an XML node to a file.
 *   mxml_write_string()    - Write a string, escaping & and < as needed.
 *   mxml_write_ws()        - Do whitespace callback...
 */

/*
 * Include necessary headers...
 */

//#include "config.h"
//#include "mxml.h"

/*
 * Global error handler...
 */

void (*mxml_error_cb)(const char *);

/*
 * Local functions...
 */

static int mxml_add_char(int ch , char **ptr , char **buffer , int *bufsize);
static int mxml_get_entity(mxml_node_t *parent , void *p , int (*getc_cb)(void *));
static int mxml_file_getc(void *p);
static int mxml_file_putc(int ch , void *p);
static mxml_node_t *mxml_load_data(mxml_node_t *top , void *p , mxml_type_t (*cb)(mxml_node_t *) ,
		int (*getc_cb)(void *));
static int mxml_parse_element(mxml_node_t *node , void *p , int (*getc_cb)(void *));
static int mxml_string_getc(void *p);
static int mxml_string_putc(int ch , void *p);
static int mxml_write_name(const char *s , void *p , int (*putc_cb)(int , void *));
static int mxml_write_node(mxml_node_t *node , void *p , int (*cb)(mxml_node_t * , int) , int col ,
		int (*putc_cb)(int , void *));
static int mxml_write_string(const char *s , void *p , int (*putc_cb)(int , void *));
static int mxml_write_ws(mxml_node_t *node , void *p , int (*cb)(mxml_node_t * , int) , int ws , int col ,
		int (*putc_cb)(int , void *));

/*
 * 'mxmlLoadFile()' - Load a file into an XML node tree.
 *
 * The nodes in the specified file are added to the specified top node.
 * If no top node is provided, the XML file MUST be well-formed with a
 * single parent node like <?xml> for the entire file. The callback
 * function returns the value type that should be used for child nodes.
 * If MXML_NO_CALLBACK is specified then all child nodes will be either
 * MXML_ELEMENT or MXML_TEXT nodes.
 *
 * The constants MXML_INTEGER_CALLBACK, MXML_OPAQUE_CALLBACK,
 * MXML_REAL_CALLBACK, and MXML_TEXT_CALLBACK are defined for loading
 * child nodes of the specified type.
 */

mxml_node_t * /* O - First node or NULL if the file could not be read. */
mxmlLoadFile(mxml_node_t *top , /* I - Top node */
             djyfs_file *fp , /* I - File to read from */
             mxml_type_t (*cb)(mxml_node_t *node))
/* I - Callback function or MXML_NO_CALLBACK */
{
	return (mxml_load_data(top , fp , cb , mxml_file_getc));
}

/*
 * 'mxmlLoadString()' - Load a string into an XML node tree.
 *
 * The nodes in the specified string are added to the specified top node.
 * If no top node is provided, the XML string MUST be well-formed with a
 * single parent node like <?xml> for the entire string. The callback
 * function returns the value type that should be used for child nodes.
 * If MXML_NO_CALLBACK is specified then all child nodes will be either
 * MXML_ELEMENT or MXML_TEXT nodes.
 *
 * The constants MXML_INTEGER_CALLBACK, MXML_OPAQUE_CALLBACK,
 * MXML_REAL_CALLBACK, and MXML_TEXT_CALLBACK are defined for loading
 * child nodes of the specified type.
 */

mxml_node_t * /* O - First node or NULL if the string has errors. */
mxmlLoadString(mxml_node_t *top , /* I - Top node */
			   const char *s , /* I - String to load */
			   mxml_type_t (*cb)(mxml_node_t *node))
/* I - Callback function or MXML_NO_CALLBACK */
{
	return (mxml_load_data(top , &s , cb , mxml_string_getc));
}

/*
 * 'mxmlSaveAllocString()' - Save an XML node tree to an allocated string.
 *
 * This function returns a pointer to a string containing the textual
 * representation of the XML node tree.  The string should be freed
 * using the free() function when you are done with it.  NULL is returned
 * if the node would produce an empty string or if the string cannot be
 * allocated.
 */

char * /* O - Allocated string or NULL */
mxmlSaveAllocString(mxml_node_t *node , /* I - Node to write */
                    int (*cb)(mxml_node_t *node , int ws))
/* I - Whitespace callback or MXML_NO_CALLBACK */
{
	int bytes; /* Required bytes */
	char buffer[8192]; /* Temporary buffer */
	char *s; /* Allocated string */

	/*
	 * Write the node to the temporary buffer...
	 */

	bytes = mxmlSaveString(node , buffer , sizeof(buffer) , cb);

	if (bytes <= 0)
		return (NULL);

	if (bytes < (int) (sizeof(buffer) - 1))
	{
		/*
		 * Node fit inside the buffer, so just duplicate that string and
		 * return...
		 */

		return (strdup(buffer));
	}

	/*
	 * Allocate a buffer of the required size and save the node to the
	 * new buffer...
	 */
	//这里调用了堆内存分配,使用时候要注意,释放内存 
	if ((s = m_malloc(bytes + 1, 0)) == NULL)
		return (NULL);

	mxmlSaveString(node , s , bytes + 1 , cb);

	/*
	 * Return the allocated string...
	 */

	return (s);
}

/*
 * 'mxmlSaveFile()' - Save an XML tree to a file.
 *
 * The callback argument specifies a function that returns a whitespace
 * character or nul (0) before and after each element. If MXML_NO_CALLBACK
 * is specified, whitespace will only be added before MXML_TEXT nodes
 * with leading whitespace and before attribute names inside opening
 * element tags.
 */

int /* O - 0 on success, -1 on error. */
mxmlSaveFile(mxml_node_t *node , /* I - Node to write */
             djyfs_file *fp , /* I - File to write to */
             int (*cb)(mxml_node_t *node , int ws))
/* I - Whitespace callback or MXML_NO_CALLBACK */
{
	int col; /* Final column */

	/*
	 * Write the node...
	 */

	if ((col = mxml_write_node(node , fp , cb , 0 , mxml_file_putc)) < 0)
		return (-1);

	if (col > 0)
		if (djy_putc('\n', fp) < 0)
			return (-1);

	/*
	 * Return 0 (success)...
	 */

	return (0);
}

/*
 * 'mxmlSaveString()' - Save an XML node tree to a string.
 *
 * This function returns the total number of bytes that would be
 * required for the string but only copies (bufsize - 1) characters
 * into the specified buffer.
 */

int /* O - Size of string */
mxmlSaveString(mxml_node_t *node , /* I - Node to write */
               char *buffer , /* I - String buffer */
               int bufsize , /* I - Size of string buffer */
               int (*cb)(mxml_node_t *node , int ws))
/* I - Whitespace callback or MXML_NO_CALLBACK */
{
	int col; /* Final column */
	char *ptr[2]; /* Pointers for putc_cb */

	/*
	 * Write the node...
	 */

	ptr[0] = buffer;
	ptr[1] = buffer + bufsize;

	if ((col = mxml_write_node(node , ptr , cb , 0 , mxml_string_putc)) < 0)
		return (-1);

	if (col > 0)
		mxml_string_putc('\n' , ptr);

	/*
	 * Nul-terminate the buffer...
	 */

	if (ptr[0] >= ptr[1])
		buffer[bufsize - 1] = '\0';
	else
		ptr[0][0] = '\0';

	/*
	 * Return the number of characters...
	 */

	return (ptr[0] - buffer);
}

/*
 * 'mxmlSetErrorCallback()' - Set the error message callback.
 */

void mxmlSetErrorCallback(void (*cb)(const char *))
/* I - Error callback function */
{
	mxml_error_cb = cb;
}

/*
 * 'mxml_add_char()' - Add a character to a buffer, expanding as needed.
 */

static int /* O  - 0 on success, -1 on error */
mxml_add_char(int ch , /* I  - Character to add */
              char **bufptr , /* IO - Current position in buffer */
              char **buffer , /* IO - Current buffer */
              int *bufsize) /* IO - Current buffer size */
{
	char *newbuffer; /* New buffer value */
	//
	if (*bufptr >= (*buffer + *bufsize - 4))
	{
		/*
		 * Increase the size of the buffer...
		 */

		if (*bufsize < 1024)
			(*bufsize) *= 2;
		else
			(*bufsize) += 1024;

		if ((newbuffer = m_realloc(*buffer , *bufsize)) == NULL)
		{
			m_free(*buffer);

			mxml_error("Unable to expand string buffer to %d bytes!" , *bufsize);

			return (-1);
		}

		*bufptr = newbuffer + (*bufptr - *buffer);
		*buffer = newbuffer;
	}

	if (ch < 128)
	{
		/*
		 * Single byte ASCII...
		 */

		*(*bufptr)++ = ch;
	}
	else
		if (ch < 2048)
		{
			/*
			 * Two-byte UTF-8...
			 */

			*(*bufptr)++ = 0xc0 | (ch >> 6);
			*(*bufptr)++ = 0x80 | (ch & 0x3f);
		}
		else
			if (ch < 65536)
			{
				/*
				 * Three-byte UTF-8...
				 */

				*(*bufptr)++ = 0xe0 | (ch >> 12);
				*(*bufptr)++ = 0x80 | ((ch >> 6) & 0x3f);
				*(*bufptr)++ = 0x80 | (ch & 0x3f);
			}
			else
			{
				/*
				 * Four-byte UTF-8...
				 */

				*(*bufptr)++ = 0xf0 | (ch >> 18);
				*(*bufptr)++ = 0x80 | ((ch >> 12) & 0x3f);
				*(*bufptr)++ = 0x80 | ((ch >> 6) & 0x3f);
				*(*bufptr)++ = 0x80 | (ch & 0x3f);
			}

	return (0);
}

/*
 * 'mxml_get_entity()' - Get the character corresponding to an entity...
 */

static int /* O - Character value or EOF on error */
mxml_get_entity(mxml_node_t *parent , /* I - Parent node */
                void *p , /* I - Pointer to source */
                int (*getc_cb)(void *)) /* I - Get character function */
{
	int ch; /* Current character */
	char entity[64], /* Entity string */
	*entptr; /* Pointer into entity */

	entptr = entity;
	//entity只能是有数字和#组成的 
	while ((ch = (*getc_cb)(p)) != EOF)
		if (!isalnum(ch) && ch != '#')//非数字 非# 
			break;
		else
			if (entptr < (entity + sizeof(entity) - 1))
				*entptr++ = ch;
			else
			{
				mxml_error("Entity name too long under parent <%s>!" , parent ? parent->value.element.name : "null");
				break;
			}

	*entptr = '\0';

	if (ch != ';')
	{
		mxml_error("Character entity \"%s\" not terminated under parent <%s>!" , entity ,
				parent ? parent->value.element.name : "null");
		return (EOF);
	}

	if (entity[1] == '#')
	{
		if (entity[2] == 'x')
			ch = strtol(entity + 3 , NULL , 16);
		else
			ch = strtol(entity + 2 , NULL , 10);
	}
	else
		if ((ch = mxmlEntityGetValue(entity)) < 0)
			mxml_error("Entity name \"%s;\" not supported under parent <%s>!" , entity ,
					parent ? parent->value.element.name : "null");

	return (ch);
}

/*
 * 'mxml_file_getc()' - Get a character from a file.
 */

static int /* O - Character or EOF */
mxml_file_getc(void *p) /* I - Pointer to file */
{
	int ch, /* Character from file */
	temp; /* Temporary character */
	djyfs_file *fp; /* Pointer to file */

	/*
	 * Read a character from the file and see if it is EOF or ASCII...
	 */

	fp = (djyfs_file *) p;
	ch = djy_getc(fp);
	//EOF或者小于0x80(不是ascii字符) 
	if (ch == EOF || !(ch & 0x80))
		return (ch);

	/*
	 * Got a UTF-8 character; convert UTF-8 to Unicode and return...
	 */

	if ((ch & 0xe0) == 0xc0)
	{
		/*
		 * Two-byte value...
		 */

		if ((temp = djy_getc(fp)) == EOF || (temp & 0xc0) != 0x80)
			return (EOF);

		ch = ((ch & 0x1f) << 6) | (temp & 0x3f);
	}
	else
		if ((ch & 0xf0) == 0xe0)
		{
			/*
			 * Three-byte value...
			 */

			if ((temp = djy_getc(fp)) == EOF || (temp & 0xc0) != 0x80)
				return (EOF);

			ch = ((ch & 0x0f) << 6) | (temp & 0x3f);

			if ((temp = djy_getc(fp)) == EOF || (temp & 0xc0) != 0x80)
				return (EOF);

			ch = (ch << 6) | (temp & 0x3f);
		}
		else
			if ((ch & 0xf8) == 0xf0)
			{
				/*
				 * Four-byte value...
				 */

				if ((temp = djy_getc(fp)) == EOF || (temp & 0xc0) != 0x80)
					return (EOF);

				ch = ((ch & 0x07) << 6) | (temp & 0x3f);

				if ((temp = djy_getc(fp)) == EOF || (temp & 0xc0) != 0x80)
					return (EOF);

				ch = (ch << 6) | (temp & 0x3f);

				if ((temp = djy_getc(fp)) == EOF || (temp & 0xc0) != 0x80)
					return (EOF);

				ch = (ch << 6) | (temp & 0x3f);
			}
			else
				return (EOF);

	return (ch);
}

/*
 * 'mxml_file_putc()' - Write a character to a file.
 */

static int /* O - 0 on success, -1 on failure */
mxml_file_putc(int ch , /* I - Character to write */
               void *p) /* I - Pointer to file */
{
	if (ch < 128)
		return (djy_putc(ch, (djyfs_file *)p));
	else
		if (ch < 2048)
		{
			/*
			 * Two-byte UTF-8 character...
			 */

			if (djy_putc(0xc0 | (ch >> 6), (djyfs_file *)p) < 0) //????
				return (-1);
			else
				return (djy_putc(0x80 | (ch & 0x3f), (djyfs_file *)p));
		}
		else
			if (ch < 65536)
			{
				/*
				 * Three-byte UTF-8 character...
				 */

				if (djy_putc(0xe0 | (ch >> 12), (djyfs_file *)p) < 0)
					return (-1);
				else
					if (djy_putc(0x80 | ((ch >> 6) & 0x3f), (djyfs_file *)p) < 0)
						return (-1);
					else
						return (djy_putc(0x80 | (ch & 0x3f), (djyfs_file *)p));
			}
			else
			{
				/*
				 * Four-byte UTF-8 character...
				 */

				if (djy_putc(0xf0 | (ch >> 18), (djyfs_file *)p) < 0)
					return (-1);
				else
					if (djy_putc(0x80 | ((ch >> 12) & 0x3f), (djyfs_file *)p) < 0)
						return (-1);
					else
						if (djy_putc(0x80 | ((ch >> 6) & 0x3f), (djyfs_file *)p) < 0)
							return (-1);
						else
							return (djy_putc(0x80 | (ch & 0x3f), (djyfs_file *)p));
			}
}

/*
 * 'mxml_load_data()' - Load data into an XML node tree.
 */

static mxml_node_t * /* O - First node or NULL if the file could not be read. */
mxml_load_data(mxml_node_t *top , /* I - Top node */
               void *p , /* I - Pointer to data */
               mxml_type_t (*cb)(mxml_node_t *) ,/* I - Callback function or MXML_NO_CALLBACK */
               int (*getc_cb)(void *))/* I - Read function */
{
	mxml_node_t *node, /* Current node */
	            *parent; /* Current parent node */
	int ch, /* Character from file */
	    whitespace; /* Non-zero if whitespace seen */
	char *buffer, /* String buffer */
	     *bufptr; /* Pointer into buffer */
	int bufsize; /* Size of buffer */
	mxml_type_t type; /* Current node type */

	/*
	 * Read elements and other nodes from the file...
	 */

	if ((buffer = m_malloc(64, 0)) == NULL)
	{
		mxml_error("Unable to allocate string buffer!");
		return (NULL);
	}

	bufsize = 64;
	bufptr = buffer;
	parent = top;
	whitespace = 0;
	//节点类型的判断 
	if (cb && parent)
		type = (*cb)(parent);
	else
		type = MXML_TEXT;

	while ((ch = (*getc_cb)(p)) != EOF)
	{   //(< 或者 空格且非不透明) 且  bufptr > buffer
		if ((ch == '<' || (isspace(ch) && type != MXML_OPAQUE)) && bufptr > buffer)
		{
			/*
			 * Add a new value node...
			 */

			*bufptr = '\0';

			switch (type)
			{
			case MXML_INTEGER :
				node = mxmlNewInteger(parent , strtol(buffer , &bufptr , 0));
				break;

			case MXML_OPAQUE :
				node = mxmlNewOpaque(parent , buffer);
				break;

			case MXML_REAL :
				node = mxmlNewReal(parent , strtod(buffer , &bufptr));
				break;

			case MXML_TEXT :
				node = mxmlNewText(parent , whitespace , buffer);
				break;

			default: /* Should never happen... */
				node = NULL;
				break;
			}
			//数字字符串中有字母字符,为错误的标志 
			if (*bufptr)
			{
				/*
				 * Bad integer/real number value...
				 */

				mxml_error("Bad %s value '%s' in parent <%s>!" , 
				                type == MXML_INTEGER ? "integer" : "real" , 
				                buffer ,
						        parent ? parent->value.element.name : "null");
				break;
			}

			bufptr = buffer;
			whitespace = isspace(ch) && type == MXML_TEXT;

			if (!node)
			{
				/*
				 * Just print error for now...
				 */

				mxml_error("Unable to add value node of type %d to parent <%s>!" , type ,
						parent ? parent->value.element.name : "null");
				break;
			}
		}
		else
			if (isspace(ch) && type == MXML_TEXT)
				whitespace = 1;

		/*
		 * Add lone whitespace node if we have an element and existing
		 * whitespace...
		 */

		if (ch == '<' && whitespace && type == MXML_TEXT)
		{
			mxmlNewText(parent , whitespace , "");
			whitespace = 0;
		}

		if (ch == '<')
		{
			/*
			 * Start of open/close tag...
			 */

			bufptr = buffer;

			while ((ch = (*getc_cb)(p)) != EOF)
				if (isspace(ch) || ch == '>' || (ch == '/' && bufptr > buffer))
					break;
				else
					if (ch == '&')
					{
						if ((ch = mxml_get_entity(parent , p , getc_cb)) == EOF)
							goto error;

						if (mxml_add_char(ch , &bufptr , &buffer , &bufsize))
							goto error;
					}
					else
						if (mxml_add_char(ch , &bufptr , &buffer , &bufsize))
							goto error;
						else
							if ((bufptr - buffer) == 3 && !strncmp(buffer , "!--" , 3))
								break;

			*bufptr = '\0';

			if (!strcmp(buffer , "!--"))
			{
				/*
				 * Gather rest of comment...
				 */

				while ((ch = (*getc_cb)(p)) != EOF)
				{
					if (ch == '>' && bufptr > (buffer + 4) && !strncmp(bufptr - 2 , "--" , 2))
						break;
					else
					{
						if (ch == '&')
							if ((ch = mxml_get_entity(parent , p , getc_cb)) == EOF)
								goto error;

						if (mxml_add_char(ch , &bufptr , &buffer , &bufsize))
							goto error;
					}
				}

				/*
				 * Error out if we didn't get the whole comment...
				 */

				if (ch != '>')
					break;

				/*
				 * Otherwise add this as an element under the current parent...
				 */

				*bufptr = '\0';

				if (!mxmlNewElement(parent , buffer))
				{
					/*
					 * Just print error for now...
					 */

					mxml_error("Unable to add comment node to parent <%s>!" ,
							parent ? parent->value.element.name : "null");
					break;
				}
			}
			else
				if (buffer[0] == '!')
				{
					/*
					 * Gather rest of declaration...
					 */

					do
					{
						if (ch == '>')
							break;
						else
						{
							if (ch == '&')
								if ((ch = mxml_get_entity(parent , p , getc_cb)) == EOF)
									goto error;

							if (mxml_add_char(ch , &bufptr , &buffer , &bufsize))
								goto error;
						}
					}
					while ((ch = (*getc_cb)(p)) != EOF);

					/*
					 * Error out if we didn't get the whole declaration...
					 */

					if (ch != '>')
						break;

					/*
					 * Otherwise add this as an element under the current parent...
					 */

					*bufptr = '\0';

					node = mxmlNewElement(parent , buffer);
					if (!node)
					{
						/*
						 * Just print error for now...
						 */

						mxml_error("Unable to add declaration node to parent <%s>!" ,
								parent ? parent->value.element.name : "null");
						break;
					}

					/*
					 * Descend into this node, setting the value type as needed...
					 */

					parent = node;

					if (cb && parent)
						type = (*cb)(parent);
				}
				else
					if (buffer[0] == '/')
					{
						/*
						 * Handle close tag...
						 */

						if (!parent || strcmp(buffer + 1 , parent->value.element.name))
						{
							/*
							 * Close tag doesn't match tree; print an error for now...
							 */

							mxml_error("Mismatched close tag <%s> under parent <%s>!" , buffer ,
									parent->value.element.name);
							break;
						}

						/*
						 * Keep reading until we see >...
						 */

						while (ch != '>' && ch != EOF)
							ch = (*getc_cb)(p);

						/*
						 * Ascend into the parent and set the value type as needed...
						 */

						parent = parent->parent;

						if (cb && parent)
							type = (*cb)(parent);
					}
					else
					{
						/*
						 * Handle open tag...
						 */

						node = mxmlNewElement(parent , buffer);

						if (!node)
						{
							/*
							 * Just print error for now...
							 */

							mxml_error("Unable to add element node to parent <%s>!" ,
									parent ? parent->value.element.name : "null");
							break;
						}

						if (isspace(ch))
							ch = mxml_parse_element(node , p , getc_cb);
						else
							if (ch == '/')
							{
								if ((ch = (*getc_cb)(p)) != '>')
								{
									mxml_error("Expected > but got '%c' instead for element <%s/>!" , ch , buffer);
									break;
								}

								ch = '/';
							}

						if (ch == EOF)
							break;

						if (ch != '/')
						{
							/*
							 * Descend into this node, setting the value type as needed...
							 */

							parent = node;

							if (cb && parent)
								type = (*cb)(parent);
						}
					}

			bufptr = buffer;
		}
		else
			if (ch == '&')
			{
				/*
				 * Add character entity to current buffer...
				 */

				if ((ch = mxml_get_entity(parent , p , getc_cb)) == EOF)
					goto error;

				if (mxml_add_char(ch , &bufptr , &buffer , &bufsize))
					goto error;
			}
			else
				if (type == MXML_OPAQUE || !isspace(ch))
				{
					/*
					 * Add character to current buffer...
					 */

					if (mxml_add_char(ch , &bufptr , &buffer , &bufsize))
						goto error;
				}
	}

	/*
	 * Free the string buffer - we don't need it anymore...
	 */

	m_free(buffer);

	/*
	 * Find the top element and return it...
	 */

	if (parent)
	{
		while (parent->parent != top && parent->parent)
			parent = parent->parent;
	}

	return (parent);

	/*
	 * Common error return...
	 */

	error:

	m_free(buffer);

	return (NULL);
}

/*
 * 'mxml_parse_element()' - Parse an element for any attributes...
 */

static int /* O - Terminating character */
mxml_parse_element(mxml_node_t *node , /* I - Element node */
                   void *p , /* I - Data to read from */
                   int (*getc_cb)(void *))
/* I - Data callback */
{
	int ch, /* Current character in file */
	quote; /* Quoting character */
	char *name, /* Attribute name */
	*value, /* Attribute value */
	*ptr; /* Pointer into name/value */
	int namesize, /* Size of name string */
	valsize; /* Size of value string */

	/*
	 * Initialize the name and value buffers...
	 */

	if ((name = m_malloc(64, 0)) == NULL)
	{
		mxml_error("Unable to allocate memory for name!");
		return (EOF);
	}

	namesize = 64;

	if ((value = malloc(64)) == NULL)
	{
		m_free(name);
		mxml_error("Unable to allocate memory for value!");
		return (EOF);
	}

	valsize = 64;

	/*
	 * Loop until we hit a >, /, ?, or EOF...
	 */

	while ((ch = (*getc_cb)(p)) != EOF)
	{
#if DEBUG > 1
		djy_printf("parse_element: ch='%c'\r\n", ch);
#endif /* DEBUG > 1 */

		/*
		 * Skip leading whitespace...
		 */

		if (isspace(ch))
			continue;

		/*
		 * Stop at /, ?, or >...
		 */

		if (ch == '/' || ch == '?')
		{
			/*
			 * Grab the > character and print an error if it isn't there...
			 */

			quote = (*getc_cb)(p);

			if (quote != '>')
			{
				mxml_error("Expected '>' after '%c' for element %s, but got '%c'!" , ch , node->value.element.name ,
						quote);
				ch = EOF;
			}

			break;
		}
		else
			if (ch == '>')
				break;

		/*
		 * Read the attribute name...
		 */

		name[0] = ch;
		ptr = name + 1;

		if (ch == '\"' || ch == '\'')
		{
			/*
			 * Name is in quotes, so get a quoted string...
			 */

			quote = ch;

			while ((ch = (*getc_cb)(p)) != EOF)
			{
				if (ch == '&')
					if ((ch = mxml_get_entity(node , p , getc_cb)) == EOF)
						goto error;

				if (mxml_add_char(ch , &ptr , &name , &namesize))
					goto error;

				if (ch == quote)
					break;
			}
		}
		else
		{
			/*
			 * Grab an normal, non-quoted name...
			 */

			while ((ch = (*getc_cb)(p)) != EOF)
				if (isspace(ch) || ch == '=' || ch == '/' || ch == '>' || ch == '?')
					break;
				else
				{
					if (ch == '&')
						if ((ch = mxml_get_entity(node , p , getc_cb)) == EOF)
							goto error;

					if (mxml_add_char(ch , &ptr , &name , &namesize))
						goto error;
				}
		}

		*ptr = '\0';

		if (ch == '=')
		{
			/*
			 * Read the attribute value...
			 */

			if ((ch = (*getc_cb)(p)) == EOF)
			{
				mxml_error("Missing value for attribute '%s' in element %s!" , name , node->value.element.name);
				return (EOF);
			}

			if (ch == '\'' || ch == '\"')
			{
				/*
				 * Read quoted value...
				 */

				quote = ch;
				ptr = value;

				while ((ch = (*getc_cb)(p)) != EOF)
					if (ch == quote)
						break;
					else
					{
						if (ch == '&')
							if ((ch = mxml_get_entity(node , p , getc_cb)) == EOF)
								goto error;

						if (mxml_add_char(ch , &ptr , &value , &valsize))
							goto error;
					}

				*ptr = '\0';
			}
			else
			{
				/*
				 * Read unquoted value...
				 */

				value[0] = ch;
				ptr = value + 1;

				while ((ch = (*getc_cb)(p)) != EOF)
					if (isspace(ch) || ch == '=' || ch == '/' || ch == '>')
						break;
					else
					{
						if (ch == '&')
							if ((ch = mxml_get_entity(node , p , getc_cb)) == EOF)
								goto error;

						if (mxml_add_char(ch , &ptr , &value , &valsize))
							goto error;
					}

				*ptr = '\0';
			}

			/*
			 * Set the attribute with the given string value...
			 */

			mxmlElementSetAttr(node , name , value);
		}
		else
		{
			/*
			 * Set the attribute with a NULL value...
			 */

			mxmlElementSetAttr(node , name , NULL);
		}

		/*
		 * Check the end character...
		 */

		if (ch == '/' || ch == '?')
		{
			/*
			 * Grab the > character and print an error if it isn't there...
			 */

			quote = (*getc_cb)(p);

			if (quote != '>')
			{
				mxml_error("Expected '>' after '%c' for element %s, but got '%c'!" , ch , node->value.element.name ,
						quote);
				ch = EOF;
			}

			break;
		}
		else
			if (ch == '>')
				break;
	}

	/*
	 * Free the name and value buffers and return...
	 */

	m_free(name);
	m_free(value);

	return (ch);

	/*
	 * Common error return point...
	 */

	error:

	m_free(name);
	m_free(value);

	return (EOF);
}

/*
 * 'mxml_string_getc()' - Get a character from a string.
 */

static int /* O - Character or EOF */
mxml_string_getc(void *p) /* I - Pointer to file */
{
	int ch; /* Character */
	const char **s; /* Pointer to string pointer */

	s = (const char **) p;

	if ((ch = *s[0]) != 0)
	{
		/*
		 * Got character; convert UTF-8 to integer and return...
		 */

		(*s)++;

		if (!(ch & 0x80))
			return (ch);
		else
			if ((ch & 0xe0) == 0xc0)
			{
				/*
				 * Two-byte value...
				 */

				if ((*s[0] & 0xc0) != 0x80)
					return (EOF);

				ch = ((ch & 0x1f) << 6) | (*s[0] & 0x3f);

				(*s)++;

				return (ch);
			}
			else
				if ((ch & 0xf0) == 0xe0)
				{
					/*
					 * Three-byte value...
					 */

					if ((*s[0] & 0xc0) != 0x80 || (*s[1] & 0xc0) != 0x80)
						return (EOF);

					ch = ((((ch & 0x0f) << 6) | (*s[0] & 0x3f)) << 6) | (*s[1] & 0x3f);

					(*s) += 2;

					return (ch);
				}
				else
					if ((ch & 0xf8) == 0xf0)
					{
						/*
						 * Four-byte value...
						 */

						if ((*s[0] & 0xc0) != 0x80 || (*s[1] & 0xc0) != 0x80 || (*s[2] & 0xc0) != 0x80)
							return (EOF);

						ch = ((((((ch & 0x07) << 6) | (*s[0] & 0x3f)) << 6) | (*s[1] & 0x3f)) << 6) | (*s[2] & 0x3f);

						(*s) += 3;

						return (ch);
					}
					else
						return (EOF);
	}
	else
		return (EOF);
}

/*
 * 'mxml_string_putc()' - Write a character to a string.
 */

static int /* O - 0 on success, -1 on failure */
mxml_string_putc(int ch , /* I - Character to write */
                 void *p) /* I - Pointer to string pointers */
{
	char **pp; /* Pointer to string pointers */

	pp = (char **) p;

	if (ch < 128)
	{
		/*
		 * Plain ASCII doesn't need special encoding...
		 */

		if (pp[0] < pp[1])
			pp[0][0] = ch;

		pp[0]++;
	}
	else
		if (ch < 2048)
		{
			/*
			 * Two-byte UTF-8 character...
			 */

			if ((pp[0] + 1) < pp[1])
			{
				pp[0][0] = 0xc0 | (ch >> 6);
				pp[0][1] = 0x80 | (ch & 0x3f);
			}

			pp[0] += 2;
		}
		else
			if (ch < 65536)
			{
				/*
				 * Three-byte UTF-8 character...
				 */

				if ((pp[0] + 2) < pp[1])
				{
					pp[0][0] = 0xe0 | (ch >> 12);
					pp[0][1] = 0x80 | ((ch >> 6) & 0x3f);
					pp[0][2] = 0x80 | (ch & 0x3f);
				}

				pp[0] += 3;
			}
			else
			{
				/*
				 * Four-byte UTF-8 character...
				 */

				if ((pp[0] + 2) < pp[1])
				{
					pp[0][0] = 0xf0 | (ch >> 18);
					pp[0][1] = 0x80 | ((ch >> 12) & 0x3f);
					pp[0][2] = 0x80 | ((ch >> 6) & 0x3f);
					pp[0][3] = 0x80 | (ch & 0x3f);
				}

				pp[0] += 4;
			}

	return (0);
}

/*
 * 'mxml_write_name()' - Write a name string.
 */

static int /* O - 0 on success, -1 on failure */
mxml_write_name(const char *s , /* I - Name to write */
                void *p , /* I - Write pointer */
                int (*putc_cb)(int , void *))
/* I - Write callback */
{
	char quote; /* Quote character */
	const char *name; /* Entity name */

	if (*s == '\"' || *s == '\'')
	{
		/*
		 * Write a quoted name string...
		 */

		if ((*putc_cb)(*s , p) < 0)
			return (-1);

		quote = *s++;

		while (*s && *s != quote)
		{
			if ((name = mxmlEntityGetName(*s)) != NULL)
			{
				if ((*putc_cb)('&' , p) < 0)
					return (-1);

				while (*name)
				{
					if ((*putc_cb)(*name , p) < 0)
						return (-1);

					name++;
				}

				if ((*putc_cb)(';' , p) < 0)
					return (-1);
			}
			else
				if ((*putc_cb)(*s , p) < 0)
					return (-1);

			s++;
		}

		/*
		 * Write the end quote...
		 */

		if ((*putc_cb)(quote , p) < 0)
			return (-1);
	}
	else
	{
		/*
		 * Write a non-quoted name string...
		 */

		while (*s)
		{
			if ((*putc_cb)(*s , p) < 0)
				return (-1);

			s++;
		}
	}

	return (0);
}

/*
 * 'mxml_write_node()' - Save an XML node to a file.
 */

static int /* O - Column or -1 on error */
mxml_write_node(mxml_node_t *node , /* I - Node to write */
                void *p , /* I - File to write to */
                int (*cb)(mxml_node_t * , int) ,/* I - Whitespace callback */
                int col , /* I - Current column */
                int (*putc_cb)(int , void *))
{
	int i, /* Looping var */
	width; /* Width of attr + value */
	mxml_attr_t *attr; /* Current attribute */
	char s[255]; /* Temporary string */

	while (node != NULL)
	{
		/*
		 * Print the node value...
		 */

		switch (node->type)
		{
		case MXML_ELEMENT :
			col = mxml_write_ws(node , p , cb , MXML_WS_BEFORE_OPEN , col , putc_cb);

			if ((*putc_cb)('<' , p) < 0)
				return (-1);
			if (mxml_write_name(node->value.element.name , p , putc_cb) < 0)
				return (-1);

			col += strlen(node->value.element.name) + 1;

			for( i = node->value.element.num_attrs, attr = node->value.element.attrs; i > 0 ; i--, attr++ )
			{
				width = strlen(attr->name);

				if (attr->value)
					width += strlen(attr->value) + 3;

				if ((col + width) > MXML_WRAP)
				{
					if ((*putc_cb)('\n' , p) < 0)
						return (-1);

					col = 0;
				}
				else
				{
					if ((*putc_cb)(' ' , p) < 0)
						return (-1);

					col++;
				}

				if (mxml_write_name(attr->name , p , putc_cb) < 0)
					return (-1);

				if (attr->value)
				{
					if ((*putc_cb)('=' , p) < 0)
						return (-1);
					if ((*putc_cb)('\"' , p) < 0)
						return (-1);
					if (mxml_write_string(attr->value , p , putc_cb) < 0)
						return (-1);
					if ((*putc_cb)('\"' , p) < 0)
						return (-1);
				}

				col += width;
			}

			if (node->child)
			{
				/*
				 * The ? and ! elements are special-cases and have no end tags...
				 */

				if (node->value.element.name[0] == '?')
				{
					if ((*putc_cb)('?' , p) < 0)
						return (-1);
					if ((*putc_cb)('>' , p) < 0)
						return (-1);
					if ((*putc_cb)('\n' , p) < 0)
						return (-1);

					col = 0;
				}
				else
					if ((*putc_cb)('>' , p) < 0)
						return (-1);
					else
						col++;

				col = mxml_write_ws(node , p , cb , MXML_WS_AFTER_OPEN , col , putc_cb);

				if ((col = mxml_write_node(node->child , p , cb , col , putc_cb)) < 0)
					return (-1);

				if (node->value.element.name[0] != '?' && node->value.element.name[0] != '!')
				{
					col = mxml_write_ws(node , p , cb , MXML_WS_BEFORE_CLOSE , col , putc_cb);

					if ((*putc_cb)('<' , p) < 0)
						return (-1);
					if ((*putc_cb)('/' , p) < 0)
						return (-1);
					if (mxml_write_string(node->value.element.name , p , putc_cb) < 0)
						return (-1);
					if ((*putc_cb)('>' , p) < 0)
						return (-1);

					col += strlen(node->value.element.name) + 3;

					col = mxml_write_ws(node , p , cb , MXML_WS_AFTER_CLOSE , col , putc_cb);
				}
			}
			else
				if (node->value.element.name[0] == '!')
				{
					if ((*putc_cb)('>' , p) < 0)
						return (-1);
					else
						col++;

					col = mxml_write_ws(node , p , cb , MXML_WS_AFTER_OPEN , col , putc_cb);
				}
				else
				{
					if ((*putc_cb)('/' , p) < 0)
						return (-1);
					if ((*putc_cb)('>' , p) < 0)
						return (-1);

					col += 2;

					col = mxml_write_ws(node , p , cb , MXML_WS_AFTER_OPEN , col , putc_cb);
				}
			break;

		case MXML_INTEGER :
			if (node->prev)
			{
				if (col > MXML_WRAP)
				{
					if ((*putc_cb)('\n' , p) < 0)
						return (-1);

					col = 0;
				}
				else
					if ((*putc_cb)(' ' , p) < 0)
						return (-1);
					else
						col++;
			}

			sprintf(s , "%d" , node->value.integer);
			if (mxml_write_string(s , p , putc_cb) < 0)
				return (-1);

			col += strlen(s);
			break;

		case MXML_OPAQUE :
			if (mxml_write_string(node->value.opaque , p , putc_cb) < 0)
				return (-1);

			col += strlen(node->value.opaque);
			break;

		case MXML_REAL :
			if (node->prev)
			{
				if (col > MXML_WRAP)
				{
					if ((*putc_cb)('\n' , p) < 0)
						return (-1);

					col = 0;
				}
				else
					if ((*putc_cb)(' ' , p) < 0)
						return (-1);
					else
						col++;
			}

			sprintf(s , "%f" , node->value.real);
			if (mxml_write_string(s , p , putc_cb) < 0)
				return (-1);

			col += strlen(s);
			break;

		case MXML_TEXT :
			if (node->value.text.whitespace && col > 0)
			{
				if (col > MXML_WRAP)
				{
					if ((*putc_cb)('\n' , p) < 0)
						return (-1);

					col = 0;
				}
				else
					if ((*putc_cb)(' ' , p) < 0)
						return (-1);
					else
						col++;
			}

			if (mxml_write_string(node->value.text.string , p , putc_cb) < 0)
				return (-1);

			col += strlen(node->value.text.string);
			break;
		}

		/*
		 * Next node...
		 */

		node = node->next;
	}

	return (col);
}

/*
 * 'mxml_write_string()' - Write a string, escaping & and < as needed.
 */

static int /* O - 0 on success, -1 on failure */
mxml_write_string(const char *s , /* I - String to write */
                  void *p , /* I - Write pointer */
                  int (*putc_cb)(int , void *))/* I - Write callback */
{
	const char *name; /* Entity name, if any */

	while (*s)
	{
		if ((name = mxmlEntityGetName(*s)) != NULL)
		{
			if ((*putc_cb)('&' , p) < 0)
				return (-1);

			while (*name)
			{
				if ((*putc_cb)(*name , p) < 0)
					return (-1);
				name++;
			}

			if ((*putc_cb)(';' , p) < 0)
				return (-1);
		}
		else
			if ((*putc_cb)(*s , p) < 0)
				return (-1);

		s++;
	}

	return (0);
}

/*
 * 'mxml_write_ws()' - Do whitespace callback...
 */

static int /* O - New column */
mxml_write_ws(mxml_node_t *node , /* I - Current node */
              void *p , /* I - Write pointer */
              int (*cb)(mxml_node_t * , int) ,/* I - Callback function */
              int ws , /* I - Where value */
              int col , /* I - Current column */
              int (*putc_cb)(int , void *))/* I - Write callback */
{
	int ch; /* Whitespace character */

	if (cb && (ch = (*cb)(node , ws)) != 0)
	{
		if ((*putc_cb)(ch , p) < 0)
			return (-1);
		else
			if (ch == '\n')
				col = 0;
			else
				if (ch == '\t')
				{
					col += MXML_TAB;
					col = col - (col % MXML_TAB);
				}
				else
					col++;
	}

	return (col);
}

/*
 * End of "$Id: mxml-file.c,v 1.27 2003/12/21 15:01:15 mike Exp $".
 */
/************** End of mxml-file.c *****************************************/
/************** Begin file mxml-node.c ***************************************/
/*
 * "$Id: mxml-node.c,v 1.11 2003/12/04 04:55:57 mike Exp $"
 *
 * Node support code for mini-XML, a small XML-like file parsing library.
 *
 * Copyright 2003 by Michael Sweet.
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Library General Public
 * License as published by the Free Software Foundation; either
 * version 2, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * Contents:
 *
 *   mxmlAdd()        - Add a node to a tree.
 *   mxmlDelete()     - Delete a node and all of its children.
 *   mxmlNewElement() - Create a new element node.
 *   mxmlNewInteger() - Create a new integer node.
 *   mxmlNewOpaque()  - Create a new opaque string.
 *   mxmlNewReal()    - Create a new real number node.
 *   mxmlNewText()    - Create a new text fragment node.
 *   mxmlNewTextf()   - Create a new formatted text fragment node.
 *   mxmlRemove()     - Remove a node from its parent.
 *   mxml_new()       - Create a new node.
 */

/*
 * Include necessary headers...
 */

//#include "config.h"
//#include "mxml.h"

/*
 * Local functions...
 */

static mxml_node_t *mxml_new(mxml_node_t *parent , mxml_type_t type);

/*
 * 'mxmlAdd()' - Add a node to a tree.
 *
 * Adds the specified node to the parent. If the child argument is not
 * NULL, puts the new node before or after the specified child depending
 * on the value of the where argument. If the child argument is NULL,
 * puts the new node at the beginning of the child list (MXML_ADD_BEFORE)
 * or at the end of the child list (MXML_ADD_AFTER). The constant
 * MXML_ADD_TO_PARENT can be used to specify a NULL child pointer.
 */

void mxmlAdd(mxml_node_t *parent , /* I - Parent node */
             int where , /* I - Where to add, MXML_ADD_BEFORE or MXML_ADD_AFTER */
             mxml_node_t *child , /* I - Child node for where or MXML_ADD_TO_PARENT */
             mxml_node_t *node) /* I - Node to add */
{
#ifdef MINIXML_DEBUG
	djy_printf("mxmlAdd(parent=%p, where=%d, child=%p, node=%p)\r\n", parent,
			where, child, node);
#endif /* DEBUG */

	/*
	 * Range check input...
	 */

	if (!parent || !node)
		return;

#if MINIXML_DEBUG > 1
	djy_printf("    BEFORE: node->parent=%p\r\n", node->parent);
	if (parent)
	{
		djy_printf("    BEFORE: parent->child=%p\r\n", parent->child);
		djy_printf("    BEFORE: parent->last_child=%p\r\n", parent->last_child);
		djy_printf("    BEFORE: parent->prev=%p\r\n", parent->prev);
		djy_printf("    BEFORE: parent->next=%p\r\n", parent->next);
	}
#endif /* DEBUG > 1 */

	/*
	 * Remove the node from any existing parent...
	 */

	if (node->parent)
		mxmlRemove(node);

	/*
	 * Reset pointers...
	 */

	node->parent = parent;

	switch (where)
	{
	case MXML_ADD_BEFORE :
		if (!child || child == parent->child || child->parent != parent)
		{
			/*
			 * Insert as first node under parent...
			 */

			node->next = parent->child;

			if (parent->child)
				parent->child->prev = node;
			else
				parent->last_child = node;

			parent->child = node;
		}
		else
		{
			/*
			 * Insert node before this child...
			 */

			node->next = child;
			node->prev = child->prev;

			if (child->prev)
				child->prev->next = node;
			else
				parent->child = node;

			child->prev = node;
		}
		break;

	case MXML_ADD_AFTER :
		if (!child || child == parent->last_child || child->parent != parent)
		{
			/*
			 * Insert as last node under parent...
			 */

			node->parent = parent;
			node->prev = parent->last_child;

			if (parent->last_child)
				parent->last_child->next = node;
			else
				parent->child = node;

			parent->last_child = node;
		}
		else
		{
			/*
			 * Insert node after this child...
			 */

			node->prev = child;
			node->next = child->next;

			if (child->next)
				child->next->prev = node;
			else
				parent->last_child = node;

			child->next = node;
		}
		break;
	}

#if MINIXML_DEBUG > 1
	djy_printf("    AFTER: node->parent=%p\r\n", node->parent);
	if (parent)
	{
		djy_printf("    AFTER: parent->child=%p\r\n", parent->child);
		djy_printf("    AFTER: parent->last_child=%p\r\n", parent->last_child);
		djy_printf("    AFTER: parent->prev=%p\r\n", parent->prev);
		djy_printf("    AFTER: parent->next=%p\r\n", parent->next);
	}
#endif /* DEBUG > 1 */
}

/*
 * 'mxmlDelete()' - Delete a node and all of its children.
 *
 * If the specified node has a parent, this function first removes the
 * node from its parent using the mxmlRemove() function.
 */

void mxmlDelete(mxml_node_t *node) /* I - Node to delete */
{
	int i; /* Looping var */

#ifdef MINIXML_DEBUG
	djy_printf("mxmlDelete(node=%p)\r\n", node);
#endif /* DEBUG */

	/*
	 * Range check input...
	 */

	if (!node)
		return;

	/*
	 * Remove the node from its parent, if any...
	 */

	mxmlRemove(node);

	/*
	 * Delete children...
	 */

	while (node->child)
		mxmlDelete(node->child);

	/*
	 * Now delete any node data...
	 */

	switch (node->type)
	{
	case MXML_ELEMENT :
		if (node->value.element.name)
			free(node->value.element.name);

		if (node->value.element.num_attrs)
		{
			for( i = 0; i < node->value.element.num_attrs ; i++ )
			{
				if (node->value.element.attrs[i].name)
					free(node->value.element.attrs[i].name);
				if (node->value.element.attrs[i].value)
					free(node->value.element.attrs[i].value);
			}

			free(node->value.element.attrs);
		}
		break;
	case MXML_INTEGER :
		/* Nothing to do */
		break;
	case MXML_OPAQUE :
		if (node->value.opaque)
			free(node->value.opaque);
		break;
	case MXML_REAL :
		/* Nothing to do */
		break;
	case MXML_TEXT :
		if (node->value.text.string)
			free(node->value.text.string);
		break;
	}

	/*
	 * Free this node...
	 */

	m_free(node);
}

/*
 * 'mxmlNewElement()' - Create a new element node.
 *
 * The new element node is added to the end of the specified parent's child
 * list. The constant MXML_NO_PARENT can be used to specify that the new
 * element node has no parent.
 */

mxml_node_t * /* O - New node */
mxmlNewElement(mxml_node_t *parent , /* I - Parent node or MXML_NO_PARENT */
               const char *name) /* I - Name of element */
{
	mxml_node_t *node; /* New node */

#ifdef MINIXML_DEBUG
	djy_printf("mxmlNewElement(parent=%p, name=\"%s\")\r\n", parent,
			name ? name : "(null)");
#endif /* DEBUG */

	/*
	 * Range check input...
	 */

	if (!name)
		return (NULL);

	/*
	 * Create the node and set the element name...
	 */

	if ((node = mxml_new(parent , MXML_ELEMENT)) != NULL)
		node->value.element.name = strdup(name);

	return (node);
}

/*
 * 'mxmlNewInteger()' - Create a new integer node.
 *
 * The new integer node is added to the end of the specified parent's child
 * list. The constant MXML_NO_PARENT can be used to specify that the new
 * integer node has no parent.
 */

mxml_node_t * /* O - New node */
mxmlNewInteger(mxml_node_t *parent , /* I - Parent node or MXML_NO_PARENT */
               int integer) /* I - Integer value */
{
	mxml_node_t *node; /* New node */

#ifdef MINIXML_DEBUG
	djy_printf("mxmlNewInteger(parent=%p, integer=%d)\r\n", parent, integer);
#endif /* DEBUG */

	/*
	 * Range check input...
	 */

	if (!parent)
		return (NULL);

	/*
	 * Create the node and set the element name...
	 */

	if ((node = mxml_new(parent , MXML_INTEGER)) != NULL)
		node->value.integer = integer;

	return (node);
}

/*
 * 'mxmlNewOpaque()' - Create a new opaque string.
 *
 * The new opaque node is added to the end of the specified parent's child
 * list. The constant MXML_NO_PARENT can be used to specify that the new
 * opaque node has no parent. The opaque string must be nul-terminated and
 * is copied into the new node.
 */

mxml_node_t * /* O - New node */
mxmlNewOpaque(mxml_node_t *parent , /* I - Parent node or MXML_NO_PARENT */
              const char *opaque) /* I - Opaque string */
{
	mxml_node_t *node; /* New node */

#ifdef MINIXML_DEBUG
	djy_printf("mxmlNewOpaque(parent=%p, opaque=\"%s\")\r\n", parent,
			opaque ? opaque : "(null)");
#endif /* DEBUG */

	/*
	 * Range check input...
	 */

	if (!parent || !opaque)
		return (NULL);

	/*
	 * Create the node and set the element name...
	 */

	if ((node = mxml_new(parent , MXML_OPAQUE)) != NULL)
		node->value.opaque = strdup(opaque);

	return (node);
}

/*
 * 'mxmlNewReal()' - Create a new real number node.
 *
 * The new real number node is added to the end of the specified parent's
 * child list. The constant MXML_NO_PARENT can be used to specify that
 * the new real number node has no parent.
 */

mxml_node_t * /* O - New node */
mxmlNewReal(mxml_node_t *parent , /* I - Parent node or MXML_NO_PARENT */
            double real) /* I - Real number value */
{
	mxml_node_t *node; /* New node */

#ifdef MINIXML_DEBUG
	djy_printf("mxmlNewReal(parent=%p, real=%g)\r\n", parent, real);
#endif /* DEBUG */

	/*
	 * Range check input...
	 */

	if (!parent)
		return (NULL);

	/*
	 * Create the node and set the element name...
	 */

	if ((node = mxml_new(parent , MXML_REAL)) != NULL)
		node->value.real = real;

	return (node);
}

/*
 * 'mxmlNewText()' - Create a new text fragment node.
 *
 * The new text node is added to the end of the specified parent's child
 * list. The constant MXML_NO_PARENT can be used to specify that the new
 * text node has no parent. The whitespace parameter is used to specify
 * whether leading whitespace is present before the node. The text
 * string must be nul-terminated and is copied into the new node.  
 */

mxml_node_t * /* O - New node */
mxmlNewText(mxml_node_t *parent , /* I - Parent node or MXML_NO_PARENT */
            int whitespace , /* I - 1 = leading whitespace, 0 = no whitespace */
            const char *string) /* I - String */
{
	mxml_node_t *node; /* New node */

#ifdef MINIXML_DEBUG
	djy_printf("mxmlNewText(parent=%p, whitespace=%d, string=\"%s\")\r\n",
			parent, whitespace, string ? string : "(null)");
#endif /* DEBUG */

	/*
	 * Range check input...
	 */

	if (!parent || !string)
		return (NULL);

	/*
	 * Create the node and set the text value...
	 */

	if ((node = mxml_new(parent , MXML_TEXT)) != NULL)
	{
		node->value.text.whitespace = whitespace;
		node->value.text.string = strdup(string);
	}

	return (node);
}

/*
 * 'mxmlNewTextf()' - Create a new formatted text fragment node.
 *
 * The new text node is added to the end of the specified parent's child
 * list. The constant MXML_NO_PARENT can be used to specify that the new
 * text node has no parent. The whitespace parameter is used to specify
 * whether leading whitespace is present before the node. The format
 * string must be nul-terminated and is formatted into the new node.  
 */

mxml_node_t * /* O - New node */
mxmlNewTextf(mxml_node_t *parent , /* I - Parent node or MXML_NO_PARENT */
             int whitespace , /* I - 1 = leading whitespace, 0 = no whitespace */
             const char *format , /* I - Printf-style frmat string */
             ...) /* I - Additional args as needed */
{
	mxml_node_t *node; /* New node */
	va_list ap; /* Pointer to arguments */

#ifdef MINIXML_DEBUG
	djy_printf("mxmlNewTextf(parent=%p, whitespace=%d, format=\"%s\", ...)\r\n",
			parent, whitespace, format ? format : "(null)");
#endif /* DEBUG */

	/*
	 * Range check input...
	 */

	if (!parent || !format)
		return (NULL);

	/*
	 * Create the node and set the text value...
	 */

	if ((node = mxml_new(parent , MXML_TEXT)) != NULL)
	{
		va_start(ap , format);

		node->value.text.whitespace = whitespace;
		node->value.text.string = mxml_strdupf(format , ap);

		va_end(ap);
	}

	return (node);
}

/*
 * 'mxmlRemove()' - Remove a node from its parent.
 *
 * Does not free memory used by the node - use mxmlDelete() for that.
 * This function does nothing if the node has no parent.
 */

void mxmlRemove(mxml_node_t *node) /* I - Node to remove */
{
#ifdef MINIXML_DEBUG
	djy_printf("mxmlRemove(node=%p)\r\n", node);
#endif /* DEBUG */

	/*
	 * Range check input...
	 */

	if (!node || !node->parent)
		return;

	/*
	 * Remove from parent...
	 */

#if MINIXML_DEBUG > 1
	djy_printf("    BEFORE: node->parent=%p\r\n", node->parent);
	if (node->parent)
	{
		djy_printf("    BEFORE: node->parent->child=%p\r\n", node->parent->child);
		djy_printf("    BEFORE: node->parent->last_child=%p\r\n", node->parent->last_child);
	}
	djy_printf("    BEFORE: node->child=%p\r\n", node->child);
	djy_printf("    BEFORE: node->last_child=%p\r\n", node->last_child);
	djy_printf("    BEFORE: node->prev=%p\r\n", node->prev);
	djy_printf("    BEFORE: node->next=%p\r\n", node->next);
#endif /* DEBUG > 1 */

	if (node->prev)
		node->prev->next = node->next;
	else
		node->parent->child = node->next;

	if (node->next)
		node->next->prev = node->prev;
	else
		node->parent->last_child = node->prev;

	node->parent = NULL;
	node->prev = NULL;
	node->next = NULL;

#if MINIXML_DEBUG > 1
	djy_printf(stderr, "    AFTER: node->parent=%p\r\n", node->parent);
	if (node->parent)
	{
		djy_printf("    AFTER: node->parent->child=%p\r\n", node->parent->child);
		djy_printf("    AFTER: node->parent->last_child=%p\r\n", node->parent->last_child);
	}
	djy_printf("    AFTER: node->child=%p\r\n", node->child);
	djy_printf("    AFTER: node->last_child=%p\r\n", node->last_child);
	djy_printf("    AFTER: node->prev=%p\r\n", node->prev);
	djy_printf("    AFTER: node->next=%p\r\n", node->next);
#endif /* DEBUG > 1 */
}

/*
 * 'mxml_new()' - Create a new node.
 */

static mxml_node_t * /* O - New node */
mxml_new(mxml_node_t *parent , /* I - Parent node */
         mxml_type_t type) /* I - Node type */
{
	mxml_node_t *node; /* New node */

#if MINIXML_DEBUG > 1
	djy_printf("mxml_new(parent=%p, type=%d)\r\n", parent, type);
#endif /* DEBUG > 1 */

	/*
	 * Allocate memory for the node...
	 */

	if ((node = calloc(1 , sizeof(mxml_node_t))) == NULL)
	{
#if DEBUG > 1
		djy_printf("    returning NULL\r\n");
#endif /* DEBUG > 1 */

		return (NULL);
	}

#if DEBUG > 1
	djy_printf("    returning %p\r\n", node);
#endif /* DEBUG > 1 */

	/*
	 * Set the node type...
	 */

	node->type = type;

	/*
	 * Add to the parent if present...
	 */

	if (parent)
		mxmlAdd(parent , MXML_ADD_AFTER , MXML_ADD_TO_PARENT , node);

	/*
	 * Return the new node...
	 */

	return (node);
}

/*
 * End of "$Id: mxml-node.c,v 1.11 2003/12/04 04:55:57 mike Exp $".
 */
/************** End of mxml-node.c *****************************************/
/************** Begin file mxml-private.c ***************************************/
/*
 * "$Id: mxml-private.c,v 1.2 2003/12/03 03:59:04 mike Exp $"
 *
 * Private functions for mini-XML, a small XML-like file parsing library.
 *
 * Copyright 2003 by Michael Sweet.
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Library General Public
 * License as published by the Free Software Foundation; either
 * version 2, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * Contents:
 *
 *   mxml_error()      - Display an error message.
 *   mxml_integer_cb() - Default callback for integer values.
 *   mxml_opaque_cb()  - Default callback for opaque values.
 *   mxml_real_cb()    - Default callback for real number values.
 */

/*
 * Include necessary headers...
 */

//#include "config.h"
//#include "mxml.h"

/*
 * Error callback function...
 */

void (*mxml_error_cb)(const char *) = NULL;

/*
 * 'mxml_error()' - Display an error message.
 */

void mxml_error(const char *format , /* I - Printf-style format string */
                ...) /* I - Additional arguments as needed */
{
	va_list ap; /* Pointer to arguments */
	char *s; /* Message string */

	/*
	 * Range check input...
	 */

	if (!format)
		return;

	/*
	 * Format the error message string...
	 */

	va_start(ap , format);

	s = mxml_strdupf(format , ap);

	va_end(ap);

	/*
	 * And then display the error message...
	 */

	if (mxml_error_cb)
		(*mxml_error_cb)(s);
	else
	{
		djy_printf("mxml: ");
		djy_printf("%s" , s);
		djy_putc('\n' , stderr);
	}

	/*
	 * Free the string...
	 */

	m_free(s);
}

/*
 * 'mxml_integer_cb()' - Default callback for integer values.
 */

mxml_type_t /* O - Node type */
mxml_integer_cb(mxml_node_t *node) /* I - Current node */
{
	(void) node;

	return (MXML_INTEGER);
}

/*
 * 'mxml_opaque_cb()' - Default callback for opaque values.
 */

mxml_type_t /* O - Node type */
mxml_opaque_cb(mxml_node_t *node) /* I - Current node */
{
	(void) node;

	return (MXML_OPAQUE);
}

/*
 * 'mxml_real_cb()' - Default callback for real number values.
 */

mxml_type_t /* O - Node type */
mxml_real_cb(mxml_node_t *node) /* I - Current node */
{
	(void) node;

	return (MXML_REAL);
}

/*
 * End of "$Id: mxml-private.c,v 1.2 2003/12/03 03:59:04 mike Exp $".
 */
/************** End of mxml-private.c *****************************************/
/************** Begin file mxml-search.c ***************************************/
/*
 * "$Id: mxml-search.c,v 1.8 2003/07/27 23:11:40 mike Exp $"
 *
 * Search/navigation functions for mini-XML, a small XML-like file
 * parsing library.
 *
 * Copyright 2003 by Michael Sweet.
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Library General Public
 * License as published by the Free Software Foundation; either
 * version 2, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * Contents:
 *
 *   mxmlFindElement() - Find the named element.
 *   mxmlWalkNext()    - Walk to the next logical node in the tree.
 *   mxmlWalkPrev()    - Walk to the previous logical node in the tree.
 */

/*
 * Include necessary headers...
 */

//#include "config.h"
//#include "mxml.h"

/*
 * 'mxmlFindElement()' - Find the named element.
 *
 * The search is constrained by the name, attribute name, and value; any
 * NULL names or values are treated as wildcards, so different kinds of
 * searches can be implemented by looking for all elements of a given name
 * or all elements with a specific attribute. The descend argument determines
 * whether the search descends into child nodes; normally you will use
 * MXML_DESCEND_FIRST for the initial search and MXML_NO_DESCEND to find
 * additional direct descendents of the node. The top node argument
 * constrains the search to a particular node's children.
 */

mxml_node_t * /* O - Element node or NULL */
mxmlFindElement(mxml_node_t *node , /* I - Current node */
                mxml_node_t *top , /* I - Top node */
                const char *name , /* I - Element name or NULL for any */
                const char *attr , /* I - Attribute name, or NULL for none */
                const char *value , /* I - Attribute value, or NULL for any */
                int descend) /* I - Descend into tree - MXML_DESCEND, MXML_NO_DESCEND, or MXML_DESCEND_FIRST */
{
	const char *temp; /* Current attribute value */

	/*
	 * Range check input...
	 */

	if (!node || !top || (!attr && value))
		return (NULL);

	/*
	 * Start with the next node...
	 */

	node = mxmlWalkNext(node , top , descend);

	/*
	 * Loop until we find a matching element...
	 */

	while (node != NULL)
	{
		/*
		 * See if this node matches...
		 */

		if (node->type == MXML_ELEMENT && node->value.element.name
				&& (!name || !strcmp(node->value.element.name , name)))
		{
			/*
			 * See if we need to check for an attribute...
			 */

			if (!attr)
				return (node); /* No attribute search, return it... */

			/*
			 * Check for the attribute...
			 */

			if ((temp = mxmlElementGetAttr(node , attr)) != NULL)
			{
				/*
				 * OK, we have the attribute, does it match?
				 */

				if (!value || !strcmp(value , temp))
					return (node); /* Yes, return it... */
			}
		}

		/*
		 * No match, move on to the next node...
		 */

		if (descend == MXML_DESCEND)
			node = mxmlWalkNext(node , top , MXML_DESCEND);
		else
			node = node->next;
	}

	return (NULL);
}

/*
 * 'mxmlWalkNext()' - Walk to the next logical node in the tree.
 *
 * The descend argument controls whether the first child is considered
 * to be the next node. The top node argument constrains the walk to
 * the node's children.
 */

mxml_node_t * /* O - Next node or NULL */
mxmlWalkNext(mxml_node_t *node , /* I - Current node */
             mxml_node_t *top , /* I - Top node */
             int descend) /* I - Descend into tree - MXML_DESCEND, MXML_NO_DESCEND, or MXML_DESCEND_FIRST */
{
	if (!node)
		return (NULL);
	else
		if (node->child && descend)
			return (node->child);
		else
			if (node->next)
				return (node->next);
			else
				if (node->parent && node->parent != top)
				{
					node = node->parent;

					while (!node->next)
						if (node->parent == top || !node->parent)
							return (NULL);
						else
							node = node->parent;

					return (node->next);
				}
				else
					return (NULL);
}

/*
 * 'mxmlWalkPrev()' - Walk to the previous logical node in the tree.
 *
 * The descend argument controls whether the previous node's last child
 * is considered to be the previous node. The top node argument constrains
 * the walk to the node's children.
 */

mxml_node_t * /* O - Previous node or NULL */
mxmlWalkPrev(mxml_node_t *node , /* I - Current node */
             mxml_node_t *top , /* I - Top node */
             int descend) /* I - Descend into tree - MXML_DESCEND, MXML_NO_DESCEND, or MXML_DESCEND_FIRST */
{
	if (!node)
		return (NULL);
	else
		if (node->prev)
		{
			if (node->prev->last_child && descend)
			{
				/*
				 * Find the last child under the previous node...
				 */

				node = node->prev->last_child;

				while (node->last_child)
					node = node->last_child;

				return (node);
			}
			else
				return (node->prev);
		}
		else
			if (node->parent != top)
				return (node->parent);
			else
				return (NULL);
}

/*
 * End of "$Id: mxml-search.c,v 1.8 2003/07/27 23:11:40 mike Exp $".
 */
/************** End of mxml-search.c *****************************************/
/************** Begin file mxml-set.c ***************************************/
/*
 * "$Id: mxml-set.c,v 1.1 2003/09/28 21:09:04 mike Exp $"
 *
 * Node set functions for mini-XML, a small XML-like file parsing library.
 *
 * Copyright 2003 by Michael Sweet.
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Library General Public
 * License as published by the Free Software Foundation; either
 * version 2, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * Contents:
 *
 *   mxmlSetElement() - Set the name of an element node.
 *   mxmlSetInteger() - Set the value of an integer node.
 *   mxmlSetOpaque()  - Set the value of an opaque node.
 *   mxmlSetReal()    - Set the value of a real number node.
 *   mxmlSetText()    - Set the value of a text node.
 *   mxmlSetTextf()   - Set the value of a text node to a formatted string.
 */

/*
 * Include necessary headers...
 */

//#include "config.h"
//#include "mxml.h"

/*
 * 'mxmlSetElement()' - Set the name of an element node.
 *
 * The node is not changed if it is not an element node.
 */

int /* O - 0 on success, -1 on failure */
mxmlSetElement(mxml_node_t *node , /* I - Node to set */
               const char *name) /* I - New name string */
{
	/*
	 * Range check input...
	 */

	if (!node || node->type != MXML_ELEMENT || !name)
		return (-1);

	/*
	 * Free any old element value and set the new value...
	 */

	if (node->value.element.name)
		m_free(node->value.element.name);

	node->value.element.name = strdup(name);

	return (0);
}

/*
 * 'mxmlSetInteger()' - Set the value of an integer node.
 *
 * The node is not changed if it is not an integer node.
 */

int /* O - 0 on success, -1 on failure */
mxmlSetInteger(mxml_node_t *node , /* I - Node to set */
               int integer) /* I - Integer value */
{
	/*
	 * Range check input...
	 */

	if (!node || node->type != MXML_INTEGER)
		return (-1);

	/*
	 * Set the new value and return...
	 */

	node->value.integer = integer;

	return (0);
}

/*
 * 'mxmlSetOpaque()' - Set the value of an opaque node.
 *
 * The node is not changed if it is not an opaque node.
 */

int /* O - 0 on success, -1 on failure */
mxmlSetOpaque(mxml_node_t *node , /* I - Node to set */
              const char *opaque) /* I - Opaque string */
{
	/*
	 * Range check input...
	 */

	if (!node || node->type != MXML_OPAQUE || !opaque)
		return (-1);

	/*
	 * Free any old opaque value and set the new value...
	 */

	if (node->value.opaque)
		m_free(node->value.opaque);

	node->value.opaque = strdup(opaque);

	return (0);
}

/*
 * 'mxmlSetReal()' - Set the value of a real number node.
 *
 * The node is not changed if it is not a real number node.
 */

int /* O - 0 on success, -1 on failure */
mxmlSetReal(mxml_node_t *node , /* I - Node to set */
            double real) /* I - Real number value */
{
	/*
	 * Range check input...
	 */

	if (!node || node->type != MXML_REAL)
		return (-1);

	/*
	 * Set the new value and return...
	 */

	node->value.real = real;

	return (0);
}

/*
 * 'mxmlSetText()' - Set the value of a text node.
 *
 * The node is not changed if it is not a text node.
 */

int /* O - 0 on success, -1 on failure */
mxmlSetText(mxml_node_t *node , /* I - Node to set */
            int whitespace , /* I - 1 = leading whitespace, 0 = no whitespace */
            const char *string) /* I - String */
{
	/*
	 * Range check input...
	 */

	if (!node || node->type != MXML_TEXT || !string)
		return (-1);

	/*
	 * Free any old string value and set the new value...
	 */

	if (node->value.text.string)
		m_free(node->value.text.string);

	node->value.text.whitespace = whitespace;
	node->value.text.string = strdup(string);

	return (0);
}

/*
 * 'mxmlSetTextf()' - Set the value of a text node to a formatted string.
 *
 * The node is not changed if it is not a text node.
 */

int /* O - 0 on success, -1 on failure */
mxmlSetTextf(mxml_node_t *node , /* I - Node to set */
             int whitespace , /* I - 1 = leading whitespace, 0 = no whitespace */
             const char *format , /* I - Printf-style format string */
             ...) /* I - Additional arguments as needed */
{
	va_list ap; /* Pointer to arguments */

	/*
	 * Range check input...
	 */

	if (!node || node->type != MXML_TEXT || !format)
		return (-1);

	/*
	 * Free any old string value and set the new value...
	 */

	if (node->value.text.string)
		m_free(node->value.text.string);

	va_start(ap , format);

	node->value.text.whitespace = whitespace;
	node->value.text.string = mxml_strdupf(format , ap);

	va_end(ap);

	return (0);
}

/*
 * End of "$Id: mxml-set.c,v 1.1 2003/09/28 21:09:04 mike Exp $".
 */
/************** End of mxml-set.c *****************************************/
/************** Begin file mxml-string.c ***************************************/
/*
 * "$Id: mxml-string.c,v 1.2 2003/09/28 12:44:39 mike Exp $"
 *
 * String functions for mini-XML, a small XML-like file parsing library.
 *
 * Copyright 2003 by Michael Sweet.
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Library General Public
 * License as published by the Free Software Foundation; either
 * version 2, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * Contents:
 *
 *   mxml_strdup()    - Duplicate a string.
 *   mxml_vsnprintf() - Format a string into a fixed size buffer.
 */

/*
 * Include necessary headers...
 */

//#include "config.h"

/*
 * 'mxml_strdup()' - Duplicate a string.
 */

#ifndef HAVE_STRDUP
char * /* O - New string pointer */
mxml_strdup(const char *s) /* I - String to duplicate */
{
	char *t; /* New string pointer */

	if (s == NULL)
		return (NULL);

	if ((t = m_malloc(strlen(s) + 1, 0)) == NULL)
		return (NULL);

	return (strcpy(t , s));
}
#endif /* !HAVE_STRDUP */

/*
 * 'mxml_strdupf()' - Format and duplicate a string.
 */

char * /* O - New string pointer */
mxml_strdupf(const char *format , /* I - Printf-style format string */
             va_list ap) /* I - Pointer to additional arguments */
{
	int bytes; /* Number of bytes required */
	char *buffer, /* String buffer */
	temp[256]; /* Small buffer for first vsnprintf */

	/*
	 * First format with a tiny buffer; this will tell us how many bytes are
	 * needed...
	 */

	bytes = vsnprintf(temp , sizeof(temp) , format , ap);

	if (bytes < sizeof(temp))
	{
		/*
		 * Hey, the formatted string fits in the tiny buffer, so just dup that...
		 */

		return (strdup(temp));
	}

	/*
	 * Allocate memory for the whole thing and reformat to the new, larger
	 * buffer...
	 */

	if ((buffer = calloc(1 , bytes + 1)) != NULL)
		vsnprintf(buffer , bytes + 1 , format , ap);

	/*
	 * Return the new string...
	 */

	return (buffer);
}

#ifndef HAVE_VSNPRINTF
/*
 * 'mxml_vsnprintf()' - Format a string into a fixed size buffer.
 */

int /* O - Number of bytes formatted */
mxml_vsnprintf(char *buffer , /* O - Output buffer */
               size_t bufsize , /* O - Size of output buffer */
               const char *format , /* I - Printf-style format string */
               va_list ap) /* I - Pointer to additional arguments */
{
	char *bufptr, /* Pointer to position in buffer */
	*bufend, /* Pointer to end of buffer */
	sign, /* Sign of format width */
	size, /* Size character (h, l, L) */
	type; /* Format type character */
	const char *bufformat; /* Start of format */
	int width, /* Width of field */
	prec; /* Number of characters of precision */
	char tformat[100], /* Temporary format string for sprintf() */
	temp[1024]; /* Buffer for formatted numbers */
	char *s; /* Pointer to string */
	int slen; /* Length of string */
	int bytes; /* Total number of bytes needed */

	/*
	 * Loop through the format string, formatting as needed...
	 */

	bufptr = buffer;
	bufend = buffer + bufsize - 1;
	bytes = 0;

	while (*format)
	{
		if (*format == '%')
		{
			bufformat = format;
			format++;

			if (*format == '%')
			{
				*bufptr++ = *format++;
				continue;
			}
			else
				if (strchr(" -+#\'" , *format))
					sign = *format++;
				else
					sign = 0;

			width = 0;
			while (isdigit(*format))
				width = width * 10 + *format++ - '0';

			if (*format == '.')
			{
				format++;
				prec = 0;

				while (isdigit(*format))
					prec = prec * 10 + *format++ - '0';
			}
			else
				prec = -1;

			if (*format == 'l' && format[1] == 'l')
			{
				size = 'L';
				format += 2;
			}
			else
				if (*format == 'h' || *format == 'l' || *format == 'L')
					size = *format++;

			if (!*format)
				break;

			type = *format++;

			switch (type)
			{
			case 'E' : /* Floating point formats */
			case 'G' :
			case 'e' :
			case 'f' :
			case 'g' :
				if ((format - bufformat + 1) > sizeof(tformat) || (width + 2) > sizeof(temp))
					break;

				strncpy(tformat , bufformat , format - bufformat);
				tformat[format - bufformat] = '\0';

				sprintf(temp , tformat , va_arg(ap, double));

				bytes += strlen(temp);

				if (bufptr)
				{
					if ((bufptr + strlen(temp)) > bufend)
					{
						strncpy(bufptr , temp , bufend - bufptr);
						bufptr = bufend;
						break;
					}
					else
					{
						strcpy(bufptr , temp);
						bufptr += strlen(temp);
					}
				}
				break;

			case 'B' : /* Integer formats */
			case 'X' :
			case 'b' :
			case 'd' :
			case 'i' :
			case 'o' :
			case 'u' :
			case 'x' :
				if ((format - bufformat + 1) > sizeof(tformat) || (width + 2) > sizeof(temp))
					break;

				strncpy(tformat , bufformat , format - bufformat);
				tformat[format - bufformat] = '\0';

				sprintf(temp , tformat , va_arg(ap, int));

				bytes += strlen(temp);

				if (bufptr)
				{
					if ((bufptr + strlen(temp)) > bufend)
					{
						strncpy(bufptr , temp , bufend - bufptr);
						bufptr = bufend;
						break;
					}
					else
					{
						strcpy(bufptr , temp);
						bufptr += strlen(temp);
					}
				}
				break;

			case 'p' : /* Pointer value */
				if ((format - bufformat + 1) > sizeof(tformat) || (width + 2) > sizeof(temp))
					break;

				strncpy(tformat , bufformat , format - bufformat);
				tformat[format - bufformat] = '\0';

				sprintf(temp , tformat , va_arg(ap, void *));

				bytes += strlen(temp);

				if (bufptr)
				{
					if ((bufptr + strlen(temp)) > bufend)
					{
						strncpy(bufptr , temp , bufend - bufptr);
						bufptr = bufend;
						break;
					}
					else
					{
						strcpy(bufptr , temp);
						bufptr += strlen(temp);
					}
				}
				break;

			case 'c' : /* Character or character array */
				bytes += width;

				if (bufptr)
				{
					if (width <= 1)
						*bufptr++ = va_arg(ap, int);
					else
					{
						if ((bufptr + width) > bufend)
							width = bufend - bufptr;

						memcpy(bufptr , va_arg(ap, char *) , width);
						bufptr += width;
					}
				}
				break;

			case 's' : /* String */
				if ((s = va_arg(ap, char *)) == NULL)
					s = "(null)";

				slen = strlen(s);
				if (slen > width && prec != width)
					width = slen;

				bytes += width;

				if (bufptr)
				{
					if ((bufptr + width) > bufend)
						width = bufend - bufptr;

					if (slen > width)
						slen = width;

					if (sign == '-')
					{
						strncpy(bufptr , s , slen);
						memset(bufptr + slen , ' ' , width - slen);
					}
					else
					{
						memset(bufptr , ' ' , width - slen);
						strncpy(bufptr + width - slen , s , slen);
					}

					bufptr += width;
				}
				break;

			case 'n' : /* Output number of chars so far */
				if ((format - bufformat + 1) > sizeof(tformat) || (width + 2) > sizeof(temp))
					break;

				strncpy(tformat , bufformat , format - bufformat);
				tformat[format - bufformat] = '\0';

				sprintf(temp , tformat , va_arg(ap, int));

				bytes += strlen(temp);

				if (bufptr)
				{
					if ((bufptr + strlen(temp)) > bufend)
					{
						strncpy(bufptr , temp , bufend - bufptr);
						bufptr = bufend;
						break;
					}
					else
					{
						strcpy(bufptr , temp);
						bufptr += strlen(temp);
					}
				}
				break;
			}
		}
		else
		{
			bytes++;

			if (bufptr && bufptr < bufend)
				*bufptr++ = *format++;
		}
	}

	/*
	 * Nul-terminate the string and return the number of characters needed.
	 */

	*bufptr = '\0';

	return (bytes);
}
#endif /* !HAVE_VSNPRINTF */

/*
 * End of "$Id: mxml-string.c,v 1.2 2003/09/28 12:44:39 mike Exp $".
 */
/************** End of mxml-string.c *****************************************/
/*
 * "$Id: testmxml.c,v 1.13 2003/12/13 16:32:42 mike Exp $"
 *
 * Test program for mini-XML, a small XML-like file parsing library.
 *
 * Copyright 2003 by Michael Sweet.
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Library General Public
 * License as published by the Free Software Foundation; either
 * version 2, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * Contents:
 *
 *   main()          - Main entry for test program.
 *   type_cb()       - XML data type callback for mxmlLoadFile()...
 *   whitespace_cb() - Let the mxmlSaveFile() function know when to insert
 *                     newlines and tabs...
 */

/*
 * Include necessary headers...
 */

//#include "config.h"
//#include "mxml.h"
//回调函数
mxml_type_t
type_cb(mxml_node_t *node)
{
	const char *type;
	
	/*
	* 你可以查看属性和/或使用XML元素名，所在层次，等等
	*/
	
	type = mxmlElementGetAttr(node, "type");
	if (type == NULL)
	type = node->value.element.name;
	
	if (!strcmp(type, "integer"))
		return (MXML_INTEGER);
	else if (!strcmp(type, "opaque"))
		return (MXML_OPAQUE);
	else if (!strcmp(type, "real"))
		return (MXML_REAL);
	else
		return (MXML_TEXT);
} 
/*
 * Local functions...
 */
bool_t minixml(char *para)
{
	mxml_node_t *xml; /* <?xml ... ?> */
	mxml_node_t *data; /* <data> */
	mxml_node_t *node; /* <node> */
	mxml_node_t *group; /* <group> */
	djyfs_file *fp; 
	xml = mxmlNewElement(NULL , "?xml");
//	xml = mxmlNewXML("1.0"); //较新的版本,可以这样定义xml根节点 

	data = mxmlNewElement(xml , "data");


	node = mxmlNewElement(data , "node");
	mxmlNewText(node , 0 , "val1");
	node = mxmlNewElement(data , "node");
	mxmlNewText(node , 0 , "val2");
	node = mxmlNewElement(data , "node");
	mxmlNewText(node , 0 , "val3");

	group = mxmlNewElement(data , "group");

	node = mxmlNewElement(group , "node");
	mxmlNewText(node , 0 , "val4");
	node = mxmlNewElement(group , "node");
	mxmlNewText(node , 0 , "val5");
	node = mxmlNewElement(group , "node");
	mxmlNewText(node , 0 , "val6");

	node = mxmlNewElement(data , "node");
	mxmlNewText(node , 0 , "val7");
	node = mxmlNewElement(data , "node");
	mxmlNewText(node , 0 , "val8");
	djy_printf("%s come from minixml\r\n" , para);
	djy_printf("%s\n", node->value.text.string);
	fp = djyfs_fopen(para, "w");
	mxmlSaveFile(xml, fp, MXML_NO_CALLBACK);
	//mxmlSaveFile(xml, fp, type_cb);
	djyfs_fclose(fp);
	return true;
}
