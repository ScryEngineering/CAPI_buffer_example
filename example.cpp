#include "Python.h"
#include <algorithm>
#include <vector>

std::vector<int> reverse_it_impl(std::vector<int> in){
	return std::reverse(in) 
}


/* Reverse a sequence and extract the data with buffer interface */
static PyObject *
reverse_it_fast(PyObject *self, PyObject *args){
	PyObject* incoming;
	if(!PyArg_ParseTuple(args, "O", &incoming)){
		return NULL;
	}
	if(!PyObject_CheckBuffer(incoming)){
		/* TODO: perhaps raise BufferError via PyExc_BufferError? */
		return NULL;
	}
	Py_buffer* view;
	int buffer_acquisition_result;
	buffer_acquisition_result = PyObject_GetBuffer(incoming, view);

    /* Fill the std::vector with the buffer contents */
	std::vector<int> original_data;
	original_data.assign(view.buf, view.buf + view.len);
	std::vector reversed = reverse_it_impl(original_data);
	PyBuffer_Release(view);
	
	//TODO: finish this
	return Py_BuildValue();
}

/* Reverse a sequence and extract the data with a copy */
static PyObject *
reverse_it(PyObject *self, PyObject *args){
	PyObject* incoming;
	if(!PyArg_ParseTuple(args, "O", &incoming)){
		return NULL;
	}
	//TODO: finish this
}

/* Methods for our reverser object*/
static PyMethodDef ReverserMethods[] = {
    {"reverse_it_fast", reverse_it_fast, METH_VARARGS, "Reverses the array using buffer interface"},
    {"reverse_it", reverse_it, METH_VARARGS, "Reverses the array"},
    {NULL, NULL, 0, NULL} /* End of methods sential value */
};

/* define the module */
static struct PyModuleDef reverser_module = {
    PyModuleDef_HEAD_INIT,
    "reverser_module",
    -1,
    ReverserMethods
};

/* Init code */
PyMODINIT_FUNC
PyInit_demo_module(void){
    return PyModule_Create(&reverser_module);
}
