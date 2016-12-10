#ifndef BUFFERINDEX_H
#define BUFFERINDEX_H


class BufferIndex
{
public:
    BufferIndex();

    int i_;
    int head_;
    int buffer_length_;

    void increment(int num);
    void decrement(int num);
};

#endif // BUFFERINDEX_H
