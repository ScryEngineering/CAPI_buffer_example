#include "Python.h"
#include <algorithm>
#include <vector>

template<typename T>
void reverse_it_impl(std::vector<T>& in){
    std::reverse(in.begin(), in.end());
}


/* Reverse a bytes array and extract the data with buffer interface */
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
    Py_buffer view;
    int buffer_acquisition_result;
    buffer_acquisition_result = PyObject_GetBuffer(incoming, &view, PyBUF_SIMPLE);

    /* Fill the std::vector with the buffer contents */
    std::vector<char> original_data;
	char* buffer_start = static_cast<char*>((&view.buf)[0]);
    original_data.assign(buffer_start, buffer_start + view.len);
    PyBuffer_Release(&view);

    reverse_it_impl(original_data);

    return Py_BuildValue("y#", original_data.data(), original_data.size());
}

/* Reverse a sequence and extract the data with a copy */
static PyObject *
reverse_it(PyObject *self, PyObject *args){
    PyObject* incoming;
    if(!PyArg_ParseTuple(args, "O", &incoming)){
        return NULL;
    }
	char* data;
	Py_ssize_t len;
	PyBytes_AsStringAndSize(incoming, &data, &len);
    std::vector<char> original_data(data, data+len);

    reverse_it_impl(original_data);
    return Py_BuildValue("y#", original_data.data(), original_data.size());
}

/* Methods for our reverser object*/
static PyMethodDef ReverserMethods[] = {
    {"reverse_it_fast", reverse_it_fast, METH_VARARGS, "Reverses the bytes array using buffer interface"},
    {"reverse_it", reverse_it, METH_VARARGS, "Reverses the bytes array"},
    {NULL, NULL, 0, NULL} /* End of methods sential value */
};

/* define the module */
static struct PyModuleDef reverser_module = {
    PyModuleDef_HEAD_INIT,
    "reverser_module",
    "Reverser module, written in c++",
    -1,
    ReverserMethods,
    NULL,
    NULL,
    NULL,
    NULL
};

/* Init code */
PyMODINIT_FUNC
PyInit_demo_module(void){
    return PyModule_Create(&reverser_module);
}
