#ifndef C_CUDA_SHARED_PTR_H
#define C_CUDA_SHARED_PTR_H

//****************************************************************************************************
//Класс разделяемого умного указателя в CUDA
//****************************************************************************************************

#include <stdio.h>
#include <stdint.h>

#include "handle_error.cu.h"
#include "../system/system.h"

//****************************************************************************************************
//подключаемые библиотеки
//****************************************************************************************************

//****************************************************************************************************
//макроопределения
//****************************************************************************************************

//****************************************************************************************************
//константы
//****************************************************************************************************

//****************************************************************************************************
//глобальные переменные
//****************************************************************************************************
static size_t Memory=0;
static size_t MaxMemory=0;

//****************************************************************************************************
//предварительные объявления
//****************************************************************************************************

//****************************************************************************************************
//Класс разделяемого умного указателя в CUDA
//****************************************************************************************************
template<class type_t>
class CCUDASharedPtr
{
 //-дружественные функции-------------------------------------------------------------------------------
 public:
  //-перечисления---------------------------------------------------------------------------------------
  //-структуры------------------------------------------------------------------------------------------
  //-константы------------------------------------------------------------------------------------------
 private:
  //-переменные-----------------------------------------------------------------------------------------
  size_t *CounterPtr;//указатель на переменную-счётчик
  type_t *DataPtr;//указатель на данные
  size_t DataSize;//размер массива в элементах
  public:
  //-конструктор----------------------------------------------------------------------------------------
  __host__ CCUDASharedPtr(void);
  //-конструктор копирования----------------------------------------------------------------------------
  __host__ CCUDASharedPtr(const CCUDASharedPtr<type_t> &cCUDASharedPtr);
  //-деструктор-----------------------------------------------------------------------------------------
  __host__ ~CCUDASharedPtr();
 public:
  //-открытые функции-----------------------------------------------------------------------------------
  __host__ void Release(void);//освободить память
  __host__ void Create(size_t unit_amount);//создать указатель
  __device__ __host__ type_t* Get(void);//получить указатель
  __device__ __host__ CCUDASharedPtr<type_t>& operator=(const CCUDASharedPtr<type_t> &cCUDASharedPtr);//оператор "="
 private:
  //-закрытые функции-----------------------------------------------------------------------------------
};
//****************************************************************************************************
//конструктор и деструктор класса
//****************************************************************************************************

//----------------------------------------------------------------------------------------------------
//конструктор
//----------------------------------------------------------------------------------------------------
template<class type_t>
__host__ CCUDASharedPtr<type_t>::CCUDASharedPtr(void)
{
 CounterPtr=NULL;
 DataPtr=NULL;
 DataSize=0;
}

//----------------------------------------------------------------------------------------------------
//конструктор копирования
//----------------------------------------------------------------------------------------------------
template<class type_t>
__host__ CCUDASharedPtr<type_t>::CCUDASharedPtr(const CCUDASharedPtr<type_t> &cCUDASharedPtr)
{
 if (&cCUDASharedPtr==this) return;
 CounterPtr=cCUDASharedPtr.CounterPtr;
 DataPtr=cCUDASharedPtr.DataPtr;
 DataSize=cCUDASharedPtr.DataSize;
 if (CounterPtr!=NULL) (*CounterPtr)++;
}
//----------------------------------------------------------------------------------------------------
//деструктор
//----------------------------------------------------------------------------------------------------
template<class type_t>
__host__ CCUDASharedPtr<type_t>::~CCUDASharedPtr()
{
 Release();
}
//****************************************************************************************************
//закрытые функции
//****************************************************************************************************


//****************************************************************************************************
//открытые функции класса
//****************************************************************************************************

//----------------------------------------------------------------------------------------------------
//освободить память
//----------------------------------------------------------------------------------------------------
template<class type_t>
__host__ void CCUDASharedPtr<type_t>::Release(void)
{
 if (CounterPtr==NULL) return;
 if (*CounterPtr>0)
 {
  (*CounterPtr)--;
  if (*CounterPtr==0)
  {
   HANDLE_ERROR(cudaFree(DataPtr));
   Memory-=sizeof(type_t)*DataSize;
   /*
   FILE *file=fopen("memory.txt","ab");
   fprintf(file,"MemoryFree:%i\r\n",Memory);
   fclose(file);
   */
   delete(CounterPtr);
  }
 }
 DataPtr=NULL;
 CounterPtr=NULL;
 DataSize=0;
}

//----------------------------------------------------------------------------------------------------
//создать указатель
//----------------------------------------------------------------------------------------------------
template<class type_t>
__host__ void CCUDASharedPtr<type_t>::Create(size_t unit_amount)
{
 Release();
 CounterPtr=new size_t;
 *CounterPtr=1;
 DataSize=unit_amount;
 Memory+=sizeof(type_t)*DataSize;
 if (MaxMemory<Memory)
 {
  MaxMemory=Memory;
  printf("Максимальный расход памяти обновлён:%i\r\n",MaxMemory);
 }
 /*
 FILE *file=fopen("memory.txt","ab");
 fprintf(file,"MemoryMalloc:%i\r\n",Memory);
 fclose(file);
 */
 HANDLE_ERROR(cudaMalloc((void**)&DataPtr,sizeof(type_t)*unit_amount));
}
//----------------------------------------------------------------------------------------------------
//получить указатель
//----------------------------------------------------------------------------------------------------
template<class type_t>
__device__ __host__ type_t* CCUDASharedPtr<type_t>::Get(void)
{
 return(DataPtr);
}

//----------------------------------------------------------------------------------------------------
//оператор "="
//----------------------------------------------------------------------------------------------------
template<class type_t>
__device__ __host__ CCUDASharedPtr<type_t>& CCUDASharedPtr<type_t>::operator=(const CCUDASharedPtr<type_t> &cCUDASharedPtr)
{
 if (this!=&cCUDASharedPtr)
 {
  CounterPtr=cCUDASharedPtr.CounterPtr;
  DataPtr=cCUDASharedPtr.DataPtr;
  DataSize=cCUDASharedPtr.DataSize;
  if (CounterPtr!=NULL) (*CounterPtr)++;
 }
 return(*this);
}

#endif