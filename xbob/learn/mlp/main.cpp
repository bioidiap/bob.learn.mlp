/**
 * @author Andre Anjos <andre.anjos@idiap.ch>
 * @date Fri 13 Dec 2013 12:35:59 CET
 *
 * @brief Bindings to bob::machine
 */

#define XBOB_LEARN_MLP_MODULE
#include <xbob.learn.mlp/api.h>

#ifdef NO_IMPORT_ARRAY
#undef NO_IMPORT_ARRAY
#endif
#include <xbob.blitz/capi.h>
#include <xbob.blitz/cleanup.h>
#include <xbob.io/api.h>
#include <xbob.learn.activation/api.h>
#include <xbob.core/random.h>

static PyMethodDef module_methods[] = {
    {0}  /* Sentinel */
};

PyDoc_STRVAR(module_docstr, "bob's multi-layer perceptron machine and trainers");

int PyXbobLearnMLP_APIVersion = XBOB_LEARN_MLP_API_VERSION;

#if PY_VERSION_HEX >= 0x03000000
static PyModuleDef module_definition = {
  PyModuleDef_HEAD_INIT,
  XBOB_EXT_MODULE_NAME,
  module_docstr,
  -1,
  module_methods,
  0, 0, 0, 0
};
#endif

static PyObject* create_module (void) {

  PyBobLearnMLPMachine_Type.tp_new = PyType_GenericNew;
  if (PyType_Ready(&PyBobLearnMLPMachine_Type) < 0) return 0;

  PyBobLearnCost_Type.tp_new = PyType_GenericNew;
  if (PyType_Ready(&PyBobLearnCost_Type) < 0) return 0;

  PyBobLearnSquareError_Type.tp_base = &PyBobLearnCost_Type;
  if (PyType_Ready(&PyBobLearnSquareError_Type) < 0) return 0;

  PyBobLearnCrossEntropyLoss_Type.tp_base = &PyBobLearnCost_Type;
  if (PyType_Ready(&PyBobLearnCrossEntropyLoss_Type) < 0) return 0;

  PyBobLearnDataShuffler_Type.tp_new = PyType_GenericNew;
  if (PyType_Ready(&PyBobLearnDataShuffler_Type) < 0) return 0;

  PyBobLearnMLPTrainer_Type.tp_new = PyType_GenericNew;
  if (PyType_Ready(&PyBobLearnMLPTrainer_Type) < 0) return 0;

# if PY_VERSION_HEX >= 0x03000000
  PyObject* m = PyModule_Create(&module_definition);
# else
  PyObject* m = Py_InitModule3(XBOB_EXT_MODULE_NAME, module_methods, module_docstr);
# endif
  if (!m) return 0;
  auto m_ = make_safe(m);

  /* register some constants */
  if (PyModule_AddIntConstant(m, "__api_version__", XBOB_LEARN_MLP_API_VERSION) < 0) return 0;
  if (PyModule_AddStringConstant(m, "__version__", XBOB_EXT_MODULE_VERSION) < 0) return 0;

  /* register the types to python */
  Py_INCREF(&PyBobLearnMLPMachine_Type);
  if (PyModule_AddObject(m, "Machine", (PyObject *)&PyBobLearnMLPMachine_Type) < 0) return 0;

  Py_INCREF(&PyBobLearnCost_Type);
  if (PyModule_AddObject(m, "Cost", (PyObject *)&PyBobLearnCost_Type) < 0) return 0;

  Py_INCREF(&PyBobLearnSquareError_Type);
  if (PyModule_AddObject(m, "SquareError", (PyObject *)&PyBobLearnSquareError_Type) < 0) return 0;

  Py_INCREF(&PyBobLearnCrossEntropyLoss_Type);
  if (PyModule_AddObject(m, "CrossEntropyLoss", (PyObject *)&PyBobLearnCrossEntropyLoss_Type) < 0) return 0;

  Py_INCREF(&PyBobLearnDataShuffler_Type);
  if (PyModule_AddObject(m, "DataShuffler", (PyObject *)&PyBobLearnDataShuffler_Type) < 0) return 0;

  Py_INCREF(&PyBobLearnMLPTrainer_Type);
  if (PyModule_AddObject(m, "Trainer", (PyObject *)&PyBobLearnMLPTrainer_Type) < 0) return 0;

  static void* PyXbobLearnMLP_API[PyXbobLearnMLP_API_pointers];

  /* exhaustive list of C APIs */

  /**************
   * Versioning *
   **************/

  PyXbobLearnMLP_API[PyXbobLearnMLP_APIVersion_NUM] = (void *)&PyXbobLearnMLP_APIVersion;

  /***************************************
   * Bindings for xbob.learn.mlp.Machine *
   ***************************************/

  PyXbobLearnMLP_API[PyBobLearnMLPMachine_Type_NUM] = (void *)&PyBobLearnMLPMachine_Type;

  PyXbobLearnMLP_API[PyBobLearnMLPMachine_Check_NUM] = (void *)&PyBobLearnMLPMachine_Check;

  PyXbobLearnMLP_API[PyBobLearnMLPMachine_NewFromSize_NUM] = (void *)&PyBobLearnMLPMachine_NewFromSize;

  /************************************
   * Bindings for xbob.learn.mlp.Cost *
   ************************************/

  PyXbobLearnMLP_API[PyBobLearnCost_Type_NUM] = (void *)&PyBobLearnCost_Type;

  PyXbobLearnMLP_API[PyBobLearnCost_Check_NUM] = (void *)&PyBobLearnCost_Check;

  PyXbobLearnMLP_API[PyBobLearnSquareError_Type_NUM] = (void *)&PyBobLearnSquareError_Type;

  PyXbobLearnMLP_API[PyBobLearnCrossEntropyLoss_Type_NUM] = (void *)&PyBobLearnCrossEntropyLoss_Type;

  /********************************************
   * Bindings for xbob.learn.mlp.DataShuffler *
   ********************************************/

  PyXbobLearnMLP_API[PyBobLearnDataShuffler_Type_NUM] = (void *)&PyBobLearnDataShuffler_Type;

  PyXbobLearnMLP_API[PyBobLearnDataShuffler_Check_NUM] = (void *)&PyBobLearnDataShuffler_Check;

  /***************************************
   * Bindings for xbob.learn.mlp.Trainer *
   ***************************************/

  PyXbobLearnMLP_API[PyBobLearnMLPTrainer_Type_NUM] = (void *)&PyBobLearnMLPTrainer_Type;

  PyXbobLearnMLP_API[PyBobLearnMLPTrainer_Check_NUM] = (void *)&PyBobLearnMLPTrainer_Check;

#if PY_VERSION_HEX >= 0x02070000

  /* defines the PyCapsule */

  PyObject* c_api_object = PyCapsule_New((void *)PyXbobLearnMLP_API,
      XBOB_EXT_MODULE_PREFIX "." XBOB_EXT_MODULE_NAME "._C_API", 0);

#else

  PyObject* c_api_object = PyCObject_FromVoidPtr((void *)PyXbobLearnMLP_API, 0);

#endif

  if (c_api_object) PyModule_AddObject(m, "_C_API", c_api_object);

  /* imports dependencies */
  if (import_xbob_blitz() < 0) {
    PyErr_Print();
    PyErr_Format(PyExc_ImportError, "cannot import `%s'", XBOB_EXT_MODULE_NAME);
    return 0;
  }

  if (import_xbob_io() < 0) {
    PyErr_Print();
    PyErr_Format(PyExc_ImportError, "cannot import `%s'", XBOB_EXT_MODULE_NAME);
    return 0;
  }

  if (import_xbob_learn_activation() < 0) {
    PyErr_Print();
    PyErr_Format(PyExc_ImportError, "cannot import `%s'", XBOB_EXT_MODULE_NAME);
    return 0;
  }

  if (import_xbob_core_random() < 0) {
    PyErr_Print();
    PyErr_Format(PyExc_ImportError, "cannot import `%s'", XBOB_EXT_MODULE_NAME);
    return 0;
  }

  Py_INCREF(m);
  return m;

}

PyMODINIT_FUNC XBOB_EXT_ENTRY_NAME (void) {
# if PY_VERSION_HEX >= 0x03000000
  return
# endif
    create_module();
}
