#include "Python.h"
#include <algorithm>
#include <vector>

template<T>
std::vector<T> reverse_it_impl(std::vector<T> in){
    return std::reverse(in); 
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
    buffer_acquisition_result = PyObject_GetBuffer(incoming, &view);

    /* Fill the std::vector with the buffer contents */
    std::vector<char> original_data;
    original_data.assign(&views->buf, &view->buf + &view->len);
    PyBuffer_Release(&view);

    std::vector reversed = reverse_it_impl(original_data);

    return Py_BuildValue("y", reversed.data, reversed.size);
}

/* Reverse a sequence and extract the data with a copy */
static PyObject *
reverse_it(PyObject *self, PyObject *args){
    PyObject* incoming;
    PyObject* seq;
    if(!PyArg_ParseTuple(args, "O", &incoming)){
        return NULL;
    }
    std::vector<char> original_data;
    seq = PySequence_Fast(incoming, "expected a sequence");
    len = PySequence_Size(incoming);
    for (i = 0; i < len; i++) {
        original_data.push_back(PySequence_Fast_GET_ITEM(seq, i));
    }
    Py_DECREF(seq);

    std::vector<char> reversed = reverse_it(original_data);
    return Py_BuildValue("y", reversed.data, reversed.size);
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
    -1,
    ReverserMethods
};

/* Init code */
PyMODINIT_FUNC
PyInit_demo_module(void){
    return PyModule_Create(&reverser_module);
}
