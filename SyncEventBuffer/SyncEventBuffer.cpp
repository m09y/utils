#include <iostream> 
#include "SyncEventBuffer.h"
#include <thread> 
using namespace std; 

const int FAILURE = -1;
const int SUCCESS = 0;

int SyncEventBuffer::synced_push(BUFFER_DATA_TYPE data)
{
   int ret_status = FAILURE;
   std::unique_lock<std::mutex> locker(mutex(),std::defer_lock);
   int max_try_count = 3; 

   auto push_data_and_unlock = [&](BUFFER_DATA_TYPE data)
   {
      UNQ_PTR_DATA_TYPE buffer_data = UNQ_PTR_DATA_TYPE(new BUFFER_DATA_TYPE(data));
      
      //Add data to buffer               
      push(std::move(buffer_data));
      cout<< "Data added to buffer.." <<data<<endl;

      locker.unlock();             
      get_cond_var().notify_all();

      ret_status = SUCCESS; 
   };

   try
   {
      //Try to acquire mutex
      if(true == locker.try_lock())
      {
         push_data_and_unlock(data);
      }
      else
      {
         cout<<" Attempt to acquire lock failed.. "<< max_try_count<<endl;
         std::thread push_data_thread ([&](BUFFER_DATA_TYPE data)
         {
            cout<<"Internal thread to push data started"<<endl;
            locker.lock();
            push_data_and_unlock(data);
         },data);

         push_data_thread.join();
      }
   }
   catch(exception& e)    
   {      
      cout<<" Exception occured during push!! " << e.what() <<endl;   
   } 
   return ret_status; 
}

SyncEventBuffer::UNQ_PTR_DATA_TYPE SyncEventBuffer::synced_pop()
{
   int ret_status = FAILURE; 
   std::unique_lock<std::mutex> locker(mutex(),std::defer_lock);

   //Acquire lock - Blocking call 
   locker.lock();
   //wait on CV if "queue empty" until signal 
   if(get_buffer().empty()) 
   { 
      cout<<" Buffer Empty!! Wait until new data arrives..." <<endl; 
      /*At the moment of blocking the thread, 
      the function automatically calls lck.unlock(),
      allowing other locked threads to continue.*/ 
      get_cond_var().wait(locker);
   } 

   if (locker.owns_lock()) 
   { 
      UNQ_PTR_DATA_TYPE data = std::move(front()); 
      pop();

      //After data access done, release the lock
      locker.unlock(); 
      ret_status = SUCCESS; 
      return std::move(data); 
   }

   return nullptr;  
} 

void SyncEventBuffer::continuous_pop()
{
   auto process_data = [](auto &data)
   {
      cout<< "Value : " <<*(data.get())<<endl;
   };

   std::thread continuos_pop_thread([this, &process_data]()
   {
      while (is_enabled())
      {
         auto data  = synced_pop();
         process_data(data);
         
      }
   });

   continuos_pop_thread.join();

}