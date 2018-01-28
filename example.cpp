#include "Python.h"
#include <algorithm>
#include <vector>

template<typename T>
std::vector<T> reverse_it_impl(std::vector<T> in){
    return std::reverse(in.begin(), in.end());
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
    original_data.assign(&view.buf, &view.buf + &view.len);
    PyBuffer_Release(&view);

    std::vector<char> reversed_data = reverse_it_impl(original_data);

    return Py_BuildValue("y#", reversed_data.data(), reversed_data.size());
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
    size_t len = PySequence_Size(incoming);
    for (size_t i = 0; i < len; i++) {
        original_data.push_back(PySequence_Fast_GET_ITEM(seq, i));
    }
    Py_DECREF(seq);

    std::vector<char> reversed_data = reverse_it_impl(original_data);
    return Py_BuildValue("y#", reversed_data.data(), reversed_data.size());
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
