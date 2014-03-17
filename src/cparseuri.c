#include <Python.h>
#include <ctype.h>

static const char uri_chars[256] = {
  /* 0 */
  0, 0, 0, 0, 0, 0, 0, 0,   0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0,   0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0,   0, 0, 0, 0, 0, 1, 1, 1,
  1, 1, 1, 1, 1, 1, 1, 1,   1, 1, 0, 0, 0, 0, 0, 0,
  /* 64 */
  0, 1, 1, 1, 1, 1, 1, 1,   1, 1, 1, 1, 1, 1, 1, 1,
  1, 1, 1, 1, 1, 1, 1, 1,   1, 1, 1, 0, 0, 0, 0, 1,
  0, 1, 1, 1, 1, 1, 1, 1,   1, 1, 1, 1, 1, 1, 1, 1,
  1, 1, 1, 1, 1, 1, 1, 1,   1, 1, 1, 0, 0, 0, 0, 0,
  /* 128 */
  0, 0, 0, 0, 0, 0, 0, 0,   0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0,   0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0,   0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0,   0, 0, 0, 0, 0, 0, 0, 0,
  /* 192 */
  0, 0, 0, 0, 0, 0, 0, 0,   0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0,   0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0,   0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0,   0, 0, 0, 0, 0, 0, 0, 0,
};

char *hex_chars = "0123456789ABCDEF";

static PyObject *
decode(PyObject *self, PyObject *args)
{
  const char *uri;

  if (!PyArg_ParseTuple(args, "s", &uri))
    return NULL;

  char *p, *result;
  int i = 0;
  PyObject *ret;

  result = malloc(strlen(uri) + 1);
  if (!result)
      return NULL;

  for (p = (char *)uri; *p != '\0'; p++) {
    if (*p == '%' && isxdigit(*(p+1)) && isxdigit(*(p+2))) {
      // Percent encoded hex: %xx
      char tmp[] = { *(p+1), *(p+2), '\0' };
      result[i++] = (char)strtol(tmp, NULL, 16);
      p += 2;
    } else {
      result[i++] = *p;
    }
  }
  result[i] = '\0';
  ret = Py_BuildValue("s", result);
  free(result);

  return ret;
}

static PyObject *
encode(PyObject *self, PyObject *args)
{
  const char *uri;

  if (!PyArg_ParseTuple(args, "s", &uri))
    return NULL;

  char *p, *result;
  int i=0;
  PyObject *ret;

  result = malloc(strlen(uri) * 3 + 1);
  if (!result)
      return NULL;

  for (p = (char *)uri; *p != '\0'; p++) {
    if (uri_chars[(unsigned char)(*p)]) {
      result[i++]=*p;
    } else {
      result[i] = '%';
      result[i+1] = hex_chars[(unsigned char)(*p) >> 4];
      result[i+2] = hex_chars[(unsigned char)(*p) & 0x0f];
      i+=3;
    }
  }
  result[i]='\0';
  ret = Py_BuildValue("s", result);
  free(result);
  return ret;
}

static PyMethodDef CParseURIMethods[] = {
  {"decode", decode, METH_VARARGS, "Decode URI"},
  {"encode", encode, METH_VARARGS, "Encode URI"},
  {NULL, NULL, 0, NULL}
};

PyMODINIT_FUNC
initcparseuri(void)
{
  (void) Py_InitModule("cparseuri", CParseURIMethods);
}

