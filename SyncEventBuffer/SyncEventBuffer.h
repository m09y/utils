#ifndef SyncBuffer_H_ 
#define SyncBuffer_H_ 

#include <queue> 
#include <mutex>
#include <condition_variable>
#include <chrono>

class SyncEventBuffer
{
   using BUFFER_DATA_TYPE = int;
   using UNQ_PTR_DATA_TYPE = std::unique_ptr<BUFFER_DATA_TYPE>;
   using INTERNAL_DATA_STRUCTURE = std::queue<UNQ_PTR_DATA_TYPE>;

   using BUFFER = INTERNAL_DATA_STRUCTURE;
   BUFFER internal_buffer;

   std::mutex m_mutex;
   std::condition_variable m_cv;
   bool m_enabled;

public:

   std::mutex& mutex()
   {
      return m_mutex;
   }

   std::condition_variable& get_cond_var()
   {
      return m_cv;
   }

   void start()
   {
      m_enabled = true;
   }

   void stop()
   {
      get_cond_var().notify_all();
   }

   bool is_enabled()
   {
      return m_enabled;
   }

   BUFFER& get_buffer()
   {
      return internal_buffer;
   }

   void push(UNQ_PTR_DATA_TYPE data)
   {
      internal_buffer.push(std::move(data));
   }

   UNQ_PTR_DATA_TYPE front()
   {
      return std::move(internal_buffer.front());
   } 
   
   void pop()
   {
      internal_buffer.pop();
   }

   int synced_push(BUFFER_DATA_TYPE data); 
   UNQ_PTR_DATA_TYPE  synced_pop();

   void continuous_pop();
};

#endif 