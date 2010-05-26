//  FileName    : ictclas.cpp 
//  Author      : Feather.et.ELF <andelf@gmail.com> 
//  Created     : Tue May 25 22:28:47 2010 by Feather.et.ELF 
//  Copyright   : Feather Workshop (c) 2010 
//  Description : a ICTCLAS python binding 
//  Time-stamp: <2010-05-26 09:07:38 andelf> 


#include <Python.h>
#include "ICTCLAS30.h"


static PyObject *result_t_to_PyList(const result_t *resList, Py_ssize_t nCount,
                                    char *para=NULL)
{
    PyObject *res, *rt, *tmp;
    res = PyList_New(nCount);
    for (Py_ssize_t i=0; i< nCount; i++)
    {
        rt = PyTuple_New(6);    // return tuple list
        if (para!= NULL) {
            tmp = PyString_FromStringAndSize(para + resList[i].start,
                                             resList[i].length);
            PyTuple_SET_ITEM(rt, 0, PyString_AsDecodedObject(tmp, "gbk", "ignore"));
            Py_DECREF(tmp);
        }
        else {
            PyTuple_SET_ITEM(rt, 0, Py_BuildValue("(ii)", resList[i].start,
                                                  resList[i].length));
        }
        PyTuple_SET_ITEM(rt, 1, Py_BuildValue("s", resList[i].sPOS));
        PyTuple_SET_ITEM(rt, 2, Py_BuildValue("i", resList[i].iPOS));
        PyTuple_SET_ITEM(rt, 3, Py_BuildValue("i", resList[i].word_ID));
        PyTuple_SET_ITEM(rt, 4, Py_BuildValue("i", resList[i].word_type));
        PyTuple_SET_ITEM(rt, 5, Py_BuildValue("i", resList[i].weight));
        
        PyList_SET_ITEM(res, i, rt);
    }
    return res;
}



static PyObject *init_wrapper(PyObject *self, PyObject *args)
{
    // ICTCLAS_Init(const char * sInitDirPath=0);
    char *dir_path = NULL;
    int result;
    if (!PyArg_ParseTuple(args, "|s", &dir_path))
        return NULL;
    if (dir_path== NULL)
        result = ICTCLAS_Init();
    else
        result = ICTCLAS_Init(dir_path);
    if (result)
        return Py_True;
    else
        return Py_False;
}

static PyObject *exit_wrapper(PyObject *self, PyObject *args)
{
    //bool ICTCLAS_Exit();
    static int exited = 0;
    if (exited)
        return Py_True;         // ICTCLAS_Exit() twice will crash
    if (ICTCLAS_Exit()) {
        exited = 1;
        return Py_True;
    }
    else
        return Py_False;
}

static PyObject *import_user_dict_wrapper(PyObject *self, PyObject *args)
{
    // unsigned int ICTCLAS_ImportUserDict(const char *sFilename);
    char *filename;
    long result;
    if (!PyArg_ParseTuple(args, "s", &filename))
        return NULL;
    result = (long)ICTCLAS_ImportUserDict(filename);
    return PyInt_FromLong(result);
}

static PyObject *process_paragraph_wrapper(PyObject *self, PyObject *args)
{
    // const char * ICTCLAS_ParagraphProcess(const char *sParagraph,
                                // int bPOStagged=1);
    char *paragraph;
    int POStagged = 1;
    const char *result;
    PyObject *ret, *tmp;
    if(!PyArg_ParseTuple(args, "et|i", "gbk", &paragraph, &POStagged))
        return NULL;
    result = ICTCLAS_ParagraphProcess(paragraph, POStagged);
    tmp = PyString_FromString(result);
    ret = PyString_AsDecodedObject(tmp, "gbk", "ignore");
    PyMem_Free(paragraph);
    Py_DECREF(tmp);
    free((void *)result);
    return ret;
}

static PyObject *para_proc_a_wrapper(PyObject *self, PyObject *args)
{
    // result_t * ICTCLAS_ParagraphProcessA(const char *sParagraph,int *pResultCount);
    char *paragraph;
    int resCount;
    const result_t *resList;
    PyObject *res;
    if(!PyArg_ParseTuple(args, "et", "gbk", &paragraph))
        return NULL;
    resList = ICTCLAS_ParagraphProcessA(paragraph, &resCount);
    res = result_t_to_PyList(resList, resCount, paragraph);
    PyMem_Free(paragraph);
    return res;
}

static PyObject *file_proc_wrapper(PyObject *self, PyObject *args)
{
// bool ICTCLAS_FileProcess(const char *sSourceFilename,
//                          const char *sResultFilename,
//                          int bPOStagged=1);
    char *srcfn, *dstfn;
    int POStagged=1;
    if(!PyArg_ParseTuple(args, "ss|i",
                         &srcfn, &dstfn, &POStagged))
        return NULL;
    if (ICTCLAS_FileProcess(srcfn, dstfn, POStagged))
        return Py_True;
    else
        return Py_False;
}

static PyObject *para_word_count_wrapper(PyObject *self, PyObject *args)
{
    // int ICTCLAS_GetParagraphProcessAWordCount(const char *sParagraph);
    char *paragraph;
    long result;
    if(!PyArg_ParseTuple(args, "et", "gbk", &paragraph))
        return NULL;
    result = (long)ICTCLAS_GetParagraphProcessAWordCount(paragraph);
    return PyInt_FromLong(result);
}

static PyObject *add_user_word_wrapper(PyObject *self, PyObject *args)
{
    // int ICTCLAS_AddUserWord(const char *sWord);
    char *word;
    if(!PyArg_ParseTuple(args, "s", &word))
        return NULL;
    if (ICTCLAS_AddUserWord(word))
        return Py_True;
    else
        return Py_False;
}

static PyObject *save_user_dict_wrapper(PyObject *self, PyObject *args)
{
    // int ICTCLAS_SaveTheUsrDic();
    if (ICTCLAS_SaveTheUsrDic())
        return Py_True;
    else
        return Py_False;
}

static PyObject *del_user_word_wrapper(PyObject *self, PyObject *args)
{
    // int ICTCLAS_DelUsrWord(const char *sWord);
    char *word;
    if(!PyArg_ParseTuple(args, "et", "gbk", &word))
        return NULL;
    if (ICTCLAS_DelUsrWord(word)!= -1)
        return Py_True;
    else
        return Py_False;        // not exists in dict
}

// (para=NULL, keyCount=0) -> [KeyWords]
static PyObject *keyword_wrapper(PyObject *self, PyObject *args, PyObject *kwargs)
{
    //int ICTCLAS_KeyWord(result_t * resultKey, int &nCountKey);
    // ICTCLAS_ParagraphProcessAW or ICTCLAS_ParagraphProcessA 
    // must excute before ICTCLAS_KeyWord .
    char *para=NULL;
    result_t *resList;
    int keyCount=0;
    int result;
    PyObject *ret;
    static char *kwlist[] = {"para", "count", NULL};
    // 关键词提取，须在ICTCLAS_ParagraphProcessAW函数执行完后执行
    if (!PyArg_ParseTupleAndKeywords(args, kwargs, "|eti", kwlist,
                                     "gbk", &para, &keyCount))
        return NULL; 
    if (para== NULL) {
        if (keyCount== 0) return NULL;
        resList = (result_t*)malloc(sizeof(result_t)*keyCount);
        result = ICTCLAS_KeyWord(resList, keyCount);
    }
    else {
        ICTCLAS_ParagraphProcessA(para, &result);
        if (keyCount<= 0) keyCount = result;
        resList = (result_t*)malloc(sizeof(result_t)*keyCount);
        result = ICTCLAS_KeyWord(resList, keyCount);
    }
    if (!result)                // failed
        return NULL;            // FIXME: throw errors.
    else {
        ret = result_t_to_PyList(resList, keyCount, para);
        PyMem_Free((void *)para);
        return ret;
    }
}


static PyObject *finger_print_wrapper(PyObject *self, PyObject *args)
{
    // Extract a finger print from the paragraph .
    // unsigned long ICTCLAS_FingerPrint();
    // ICTCLAS_ParagraphProcessAW or ICTCLAS_ParagraphProcessA 
    // must excute before ICTCLAS_FingerPrint.
    int count=0;
    long result;
    char *para=NULL;

    if (!PyArg_ParseTuple(args, "|s", &para))
        return NULL;
    if (para!= NULL)
        ICTCLAS_ParagraphProcessA(para, &count); // DUMMY
    result = ICTCLAS_FingerPrint();
    return PyInt_FromLong(result);
}


static PyObject *set_pos_map_wrapper(PyObject *self, PyObject *args)
{
    // int ICTCLAS_SetPOSmap(int nPOSmap);
    int posMap;
    if (!PyArg_ParseTuple(args, "i", &posMap))
        return NULL;
    if (ICTCLAS_SetPOSmap(posMap))
        return Py_True;
    else
        return Py_False;
}

    
static PyMethodDef ictclasMethods[] = {
    {"init",  init_wrapper, METH_VARARGS,
     "(path=\"./\") -> bool\n"
     "Init the analyzer and prepare necessary"
     " data for ICTCLAS according the configure file."},
    {"exit", exit_wrapper, METH_NOARGS,
     "() -> bool\n"
     "Exit the program and free all resources "
     "and destroy all working buffer used in ICTCLAS."},
    {"import_user_dict", import_user_dict_wrapper, METH_VARARGS,
     "(dict_filename,) -> int [HOW MANY IMPORTED]\n"
     "Import user-defined dictionary from a text file."},
    {"para_proc", process_paragraph_wrapper, METH_VARARGS,
     "(para,) -> unicode\n"
     "Process a paragraph."},
    {"para_proc_a", para_proc_a_wrapper, METH_VARARGS,
     "(para,) -> [(uword, sPOS, iPOS, word_id, word_type, weight)]"},
    {"para_word_count", para_word_count_wrapper, METH_VARARGS,
     "(para,) -> int\n"
     "Word count of a paragraph."},
    {"add_user_word", add_user_word_wrapper, METH_VARARGS,
     "(word,) -> bool\n"
     "Add a word to the user dictionary."},
    {"save_user_dict", save_user_dict_wrapper, METH_NOARGS,
     "() -> bool\n"
     "Save the user dictionary to disk."},
    {"del_user_word", del_user_word_wrapper, METH_VARARGS,
     "(word,) -> bool\n"
     "Delete a word from the  user dictionary."},
    {"keyword", (PyCFunction)keyword_wrapper,
     METH_VARARGS | METH_KEYWORDS,
     "(para, count=0) -> [(uword, sPOS, iPOS, word_id, word_type, weight)]\n"
     "(count=0,) -> [((start,length), sPOS, iPOS, word_id, word_type, weight)]\n"
     "Extract keyword from paragraph."},
    {"finger_print", finger_print_wrapper, METH_VARARGS,
     "(para={last_processed},) -> int\n"
     "Extract a finger print from the paragraph."},
    {"set_pos_map", set_pos_map_wrapper, METH_VARARGS,
     "(pos_map,) -> bool\n"
     "Select which pos map will use. One of:\n"
     "ICT_POS_MAP_FIRST  ICT_POS_MAP_SECOND\n"
     "PKU_POS_MAP_FIRST  PKU_POS_MAP_SECOND"},
    {"file_proc", file_proc_wrapper, METH_VARARGS,
     "(src_filename, dst_filename, is_pos_tagged=1) -> bool\n"
     "Process a text file"},
    {NULL, NULL, 0, NULL}
};


PyMODINIT_FUNC
init_ictclas(void)
{
    PyObject *m;
    m = Py_InitModule("_ictclas", ictclasMethods);
    if (m== NULL)
        return;
    PyModule_AddIntConstant(m, "ICT_POS_MAP_FIRST", ICT_POS_MAP_FIRST);
    PyModule_AddIntConstant(m, "ICT_POS_MAP_SECOND", ICT_POS_MAP_SECOND);
    PyModule_AddIntConstant(m, "PKU_POS_MAP_SECOND", PKU_POS_MAP_SECOND);
    PyModule_AddIntConstant(m, "PKU_POS_MAP_FIRST", PKU_POS_MAP_FIRST);
    PyModule_AddStringConstant(m, "version", "3.0.0");
}
