#ifndef C_CUDA_TIME_SPENT_H
#define C_CUDA_TIME_SPENT_H

//****************************************************************************************************
//класс измерения времени выполнения
//****************************************************************************************************

#include <stdio.h>
#include <stdint.h>

#include "handle_error.cu.h"

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
//класс измерения времени выполнения
//****************************************************************************************************
class CCUDATimeSpent
{
 public:
  //-перечисления---------------------------------------------------------------------------------------
  //-структуры------------------------------------------------------------------------------------------
  //-константы------------------------------------------------------------------------------------------
 public:
  //-переменные-----------------------------------------------------------------------------------------
  cudaEvent_t StartEvent;
  cudaEvent_t StopEvent;
 private:
  public:
  //-конструктор----------------------------------------------------------------------------------------
  __host__ CCUDATimeSpent(void);
  //-деструктор-----------------------------------------------------------------------------------------
  __host__ ~CCUDATimeSpent();
 public:
  //-открытые функции-----------------------------------------------------------------------------------
  void Start(void);//начать измерение времени
  float Stop(void);//завершить измерение времени и получить результат в миллисекундах
 private:
  //-закрытые функции-----------------------------------------------------------------------------------
};
//****************************************************************************************************
//конструктор и деструктор класса
//****************************************************************************************************

//----------------------------------------------------------------------------------------------------
//конструктор
//----------------------------------------------------------------------------------------------------
__host__ CCUDATimeSpent::CCUDATimeSpent(void)
{
 cudaEventCreate(&StartEvent);
 cudaEventCreate(&StopEvent);


}
//----------------------------------------------------------------------------------------------------
//деструктор
//----------------------------------------------------------------------------------------------------
__host__ CCUDATimeSpent::~CCUDATimeSpent()
{
 cudaEventDestroy(StartEvent);
 cudaEventDestroy(StopEvent);
}
//****************************************************************************************************
//закрытые функции
//****************************************************************************************************


//****************************************************************************************************
//открытые функции класса
//****************************************************************************************************

//----------------------------------------------------------------------------------------------------
//начать измерение времени
//----------------------------------------------------------------------------------------------------
void CCUDATimeSpent::Start(void)
{
 cudaEventRecord(StartEvent,0);
}
//----------------------------------------------------------------------------------------------------
//завершить измерение времени и получить результат в миллисекундах
//----------------------------------------------------------------------------------------------------
float CCUDATimeSpent::Stop(void)
{
 cudaEventRecord(StopEvent,0);
 cudaEventSynchronize(StopEvent);
 float gpu_time;
 cudaEventElapsedTime(&gpu_time,StartEvent,StopEvent);
 return(gpu_time);
}

#endif
