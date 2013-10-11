/**
 * Copyright 2013 Truphone
 */
#include "include/Buffer.h"

#include <QTimer>
#include <QQueue>

typedef void(timeoutCallBack)(void);

namespace truphone
{
namespace test
{
namespace cascades
{
    /*!
     * \brief The BufferPrivate class is the internal implementation of the cached buffers
     *
     * @since test-cascades 1.0.0
     */
    class BufferPrivate
    {
    public:
        /*!
         * \brief cached A static cache of buffers
         *
         * @since test-cascades 1.0.0
         */
        static QQueue<BufferPrivate*> cached;
        /*!
         * \brief BufferPrivate Constructor
         *
         * \param bufferSize The size of the buffer
         *
         * @since test-cascades 1.0.0
         */
        explicit BufferPrivate(const int bufferSize = 1024) :
            bufferSize(bufferSize),
            buffer(new char[bufferSize])
        {
        }
        /*!
         * \brief ~BufferPrivate Destructor
         *
         * @since test-cascades 1.0.0
         */
        virtual ~BufferPrivate()
        {
            if (this->buffer)
            {
                delete this->buffer;
            }
        }
        /*!
         * \brief data Get a pointer to the data
         *
         * \return Pointer to the buffer
         *
         * @since test-cascades 1.0.0
         */
        inline char * data() const
        {
            return this->buffer;
        }
        /*!
         * \brief length Return the length (available) of the buffer
         *
         * \return The maximum length of the buffer in bytes
         *
         * @since test-cascades 1.0.0
         */
        inline size_t length() const
        {
            return ((this->buffer) ? bufferSize : 0);
        }
    protected:
    private:
        /*!
         * \brief bufferSize The size of the buffer
         */
        const int bufferSize;
        /*!
         * \brief buffer Pointer to the heap buffer
         */
        char * const buffer;
        /*!
         * \brief BufferPrivate Hidden copy constructor
         */
        BufferPrivate(const BufferPrivate&) :
            bufferSize(0),
            buffer(NULL)
        {throw 0;}
    };

    QQueue<BufferPrivate*> BufferPrivate::cached;

    void Buffer::init()
    {
        if (BufferPrivate::cached.isEmpty())
        {
            this->pvt = new BufferPrivate();
        }
        else
        {
            this->pvt = BufferPrivate::cached.dequeue();
        }
        this->clean();
    }

    Buffer::Buffer()
    {
        init();
    }

    Buffer::Buffer(const char * v)
    {
        init();
        if (v)
        {
            strncpy(data(), v, length());
        }
    }

    Buffer::~Buffer()
    {
        BufferPrivate::cached.enqueue(this->pvt);
    }

    char * Buffer::data() const
    {
        return this->pvt->data();
    }

    const char * Buffer::cdata() const
    {
        return this->pvt->data();
    }

    size_t Buffer::length() const
    {
        return this->pvt->length();
    }
}  // namespace cascades
}  // namespace test
}  // namespace truphone
