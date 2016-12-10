#include "bufferindex.h"

BufferIndex::BufferIndex()
{
    i_ = 0;
    buffer_length_ = 100000;
    head_ = 0;
}

void BufferIndex::increment(int num)
{
    if(i_ > head_)
    {
        i_ = (i_ + num)%(buffer_length_-1);
    }
    else
    {
        i_ = i_ + num;

        if(i_ > head_)
        {
            i_ = head_;
        }
    }
}

void BufferIndex::decrement(int num)
{
    i_ = i_ - 1;
    if(i_ < 0)
    {
        i_ = buffer_length_ - 1;
    }

    return;
}
